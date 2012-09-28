/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include "sys/node_movements/playback/ns2_node_movement_creator.h"
#include "sys/node_movements/playback/movement_controller.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/jump_movement.h"
#include "sys/node_movements/linear_movement.h"
#include "sys/node_movement.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/util/string_conv.h"

using namespace std;

namespace shawn
{
	NS2NodeMovementCreator::NS2NodeMovementCreator(SimulationController& sc) : 
    sc_(&sc),
	//node_movement_(NULL),
	node_(NULL),
	movement_info_(NULL),
	inputfile_(NULL),
    sstream_(NULL),
	istream_(NULL),
	sbuf_(NULL),
	line_nr_(0),
	urgency_(MovementInfo::Delayed)
    {
        //sc_ = &sc;
        string path = sc_->environment_w().required_string_param("path");
        inputfile_ = new ifstream(path.c_str());
        //sstream_ = new stringstream(stringstream::in | stringstream::out);
    }
    // ----------------------------------------------------------------------
    NS2NodeMovementCreator:: ~NS2NodeMovementCreator()
    {

        if (inputfile_ != NULL)
        {
			if (inputfile_->is_open()) 
				inputfile_->close();
            delete inputfile_;
            inputfile_ = NULL;
        }
		if (sstream_ != NULL)
		{
			sstream_->clear();
			delete sstream_;
			sstream_ = NULL;
		}
		if (sbuf_ != NULL){
			delete sbuf_;
			sbuf_ = NULL;
		}
    }
    // ----------------------------------------------------------------------
    MovementInfo* NS2NodeMovementCreator::next_movement()
    {
        char buffer[250];
        movement_info_ = new MovementInfo();

        if (! inputfile_->is_open())
        {
            WARN(sc_->logger(), "The File containing the movement orders could not be opened");
			// NEW
			assert(movement_info_);
			delete movement_info_;
			movement_info_ = NULL;
			// NEW
			abort();
            return NULL;
        }
        else
            if (!inputfile_->eof())
            {
                line_nr_++;
                inputfile_->getline (buffer,250);
                if (!(parse_line(string(buffer))))
                {
                    //WARN(sc_->logger(), "Line "<<line_nr_<<" could not be parsed");
                    if (sbuf_)
                    {
                       delete sbuf_;
                       sbuf_ = NULL;
                    } 
                    if (sstream_)
                    {
                       delete sstream_;
                       sstream_ = NULL;
                    }
					// NEW
					assert(movement_info_);
					delete movement_info_;
					movement_info_ = NULL;
					// NEW
                    return next_movement();
                }
                else
                {
                    if (sbuf_)
                    {
                        delete sbuf_;
                        sbuf_ = NULL;
                    }
                    if (sstream_)
                    {
                       delete sstream_;
                       sstream_ = NULL;
                    }
                    return movement_info_;
                }
            }
            else
            {
                DEBUG(sc_->logger(), "Finished reading file");
                cout<<inputfile_->tellg()<<endl;
				// NEW 
				assert(movement_info_);
				delete movement_info_;
				movement_info_ = NULL;
				// NEW
                return NULL;
            }

    }

    // ----------------------------------------------------------------------
    bool NS2NodeMovementCreator::parse_scheduled()
    {
        string str;
        int nodenumber = -1;
        string nodelabel;
        double time = 0.0f;

        *sstream_ >> str;

        if (strcmp(str.c_str(), "at") == 0)
        {
            // *sstream_ >> time;

            *sstream_ >> str;

            try {
                time = conv_string_to_double(str);
            } catch ( std::runtime_error& )
            {
                WARN(sc_->logger(),"Line " << line_nr_ << " contains a corrupt double value");
                return false;
            }

            sstream_->ignore(10,' ');

            if (sstream_->peek()=='"')
            {
                sstream_->get();
            }

            if (sstream_->peek() == '$')
            {
                sstream_->ignore(7,EOF);
                *sstream_ >> nodenumber;
                node_ = sc_->world_w().find_node_by_id_w(nodenumber);
                *sstream_ >> str;
                if (node_ == NULL)
                {
                    WARN(sc_->logger(), "Line " << line_nr_ << " nodelabel starting with $ (forbidden) or invalid nodenumber");
                    return false;
                }
            }
            else
            {
                *sstream_ >> nodelabel;
                node_ = sc_->world_w().find_node_by_label_w(nodelabel);
                if (node_ == NULL)
                {
                    WARN(sc_->logger(), "Line " << line_nr_ << " invalid nodelabel or scrambled line");
                    return false;
                }
            }

            return parse_rest(nodenumber, nodelabel, time);
        }
        else
        {
            WARN(sc_->logger(), "Line " << line_nr_ << " incorrect syntax");
            return false;
        }
    }

    // ----------------------------------------------------------------------

    bool NS2NodeMovementCreator::parse_instant(std::string str)
    {
        int nodenumber = - 1;
        string nodelabel;

        stringstream sstream(stringstream::in | stringstream::out);
        sstream << str;

        if (sstream.peek() == '$')
        {
            sstream.ignore(7,EOF);
            sstream >> nodenumber;
            node_ = sc_->world_w().find_node_by_id_w(nodenumber);
            sstream >> str;
            if (node_ == NULL)
            {
                WARN(sc_->logger(), "Line " << line_nr_ << " nodelabel starting with $ (forbidden) or invalid nodenumber");
                return false;
            }

        }
        else
        {
            sstream >> nodelabel;
            node_ = sc_->world_w().find_node_by_label_w(nodelabel);
            if (node_== NULL)
            {
                WARN(sc_->logger(), "Line " << line_nr_ << " : " << nodelabel << " invalid nodelabel or scrambled line");
                return false;
            }
        }

        return parse_rest(nodenumber, nodelabel, sc_->world_w().current_time());

    }

    // ----------------------------------------------------------------------
    bool NS2NodeMovementCreator::parse_rest(int nodenumber, std::string nodelabel, double time)
    {
        stringstream sstream(stringstream::in | stringstream::out);

        string str;
        *sstream_ >> str;

        if (strcmp(str.c_str(), "set") == 0)
        {
            JumpMovement * jump_movement;
            string dimension;
            double position;
            jump_movement = new JumpMovement();
            
            *sstream_ >> str;
            dimension = str.substr(0,1);
            *sstream_ >> str;
            *sstream_ >> str;

            str = str.substr(0,str.find_last_of('"'));

            try{
                position = conv_string_to_double(str);
            } catch ( std::runtime_error& )
            {
                WARN(sc_->logger(),"Line " << line_nr_ << " contains a corrupt double value");
                return false;
            }

            if (dimension == "X")
            {
                jump_movement->set_dimension(JumpMovement::X, position);
            }
            else
                if (dimension == "Y")
                {
                    jump_movement->set_dimension(JumpMovement::Y, position);
                }
                else
                    if (dimension == "Z")
                    {
                        jump_movement->set_dimension(JumpMovement::Z, position);
                    }
                    else
                    {
                        WARN(sc_->logger(), "Line " << line_nr_ << " has an incorrect dimension");
                        return false;
                    }

                    movement_info_->set_urgency(urgency_);
                    movement_info_->set_dispatch_time(time);
                    movement_info_->set_node(node_);
                    movement_info_->set_node_movement(jump_movement);
                    
                    DEBUG(sc_->logger(), "jumpmovement: " << "nodenumber: " << nodenumber << " , dimension: " << dimension << " , position: " << position << " , at " << time);
                    return true;
        }
        else
            if (strcmp(str.c_str(), "setdest") == 0)
            {
                LinearMovement * linear_movement;
                double xposition, yposition, velocity;
                linear_movement = new LinearMovement();

                //*sstream_ >> xposition;
                //*sstream_ >> yposition;
                //*sstream_ >> velocity;

                try
                {
                    *sstream_ >> str;
                    xposition = conv_string_to_double(str);
                    *sstream_ >> str;
                    yposition = conv_string_to_double(str);
                    *sstream_ >> str;
                    str = str.substr(0,str.find_last_of('"'));
                    velocity = conv_string_to_double(str);

                } catch ( std::runtime_error& )
                {
                    WARN(sc_->logger(),"Line " << line_nr_ << " contains a corrupt double value");
					// NEW
					assert(linear_movement);
					delete linear_movement;
					linear_movement = NULL;
					// NEW
                    return false;
                }

                if (velocity<0)
                {
                    WARN(sc_->logger(),"Line " << line_nr_ << " has a negative velocity");
					// NEW
					assert(linear_movement);
					delete linear_movement;
					linear_movement = NULL;
					// NEW
                    return false;
                }

                linear_movement->set_parameters(velocity, Vec(xposition, yposition, 0), sc_->world_w());
                movement_info_->set_urgency(urgency_);
                movement_info_->set_dispatch_time(time);
                movement_info_->set_node(node_);
                movement_info_->set_node_movement(linear_movement);
                DEBUG(sc_->logger(), "linearmovement: " << "time: " << time << " , nodenumber: " << nodenumber <<  " , xposition: " << xposition << " , yposition: " << yposition << " , velocity: " << velocity);
                return true;
            }
            else
            {
                return false;
            }
    }

    // ----------------------------------------------------------------------
    bool NS2NodeMovementCreator::parse_line(std::string buffer)
    {
        string str;
//         stringbuf * sbuf_ = new stringbuf (buffer, ios_base::in);
        sbuf_ = new stringbuf (buffer, ios_base::in);
        sstream_ = new istream(sbuf_);

        *sstream_ >> str;

        if (strcmp(str.c_str(), "") == 0)
        {
            WARN(sc_->logger(),"Line " << line_nr_ << " is empty");
            return false;
        }
        else
            if (strcmp((str.c_str()), "#") == 0)
            {
                return false;
            }
            else
                if (strcmp(str.c_str(), "$ns_") == 0)
                {
                    DEBUG(sc_->logger(),"Line " << line_nr_ << " contains " << buffer);
                    urgency_ = MovementInfo::Delayed;
                    return parse_scheduled();
                }
                else
                {
                    DEBUG(sc_->logger(),"Line " << line_nr_ << " contains " << buffer);
                    urgency_ = MovementInfo::Immediately;
                    return parse_instant(str);
                }
    }
    // ----------------------------------------------------------------------
    void NS2NodeMovementCreator::reset()
    {
        inputfile_->clear();
        inputfile_->seekg (0, ios::beg);
        line_nr_ = 0;
    }
}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/ns2_node_movement_creator.cpp,v $
* Version $Revision: 248 $
* Date    $Date: 2008-09-23 16:28:10 +0200 (Tue, 23 Sep 2008) $
*-----------------------------------------------------------------------
* $Log: ns2_node_movement_creator.cpp,v $
 *-----------------------------------------------------------------------*/
