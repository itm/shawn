/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/random/random_seed.h"

using namespace std;

namespace shawn {

  // SimulationTaskRandomSeed ------------------------------------
  //
  //+------------------------------------------------------------+
  //|              HOW TO USE:                                   |
  //+------------------------------------------------------------+
  // Writing such a line in the simulator's config file:
  //
  // random_seed action=create filename=random.seed
  //
  // will generate a new (pseudo)random number generator's seed
  // from system time and store it in the 
  // file "random.seed", in the current directory. The newly 
  // generated seed is immediately active. 
  // 
  // To successively retrieve an old seed from a file
  // and use it in another simulation, just type:
  //
  // random_seed action=load filename="file_containing_the_seed" 
  //
  // in the config file. The seed is loaded and the generator
  // initialized.
  //
  // IMPORTANT: in order to use the seed in another task, 
  // for example rect_world, random_seed has to be called 
  // (obviously) before that task
  // 
  //-------------------------------------------------------------


  SimulationTaskRandomSeed::SimulationTaskRandomSeed()
  {
  }
  
  SimulationTaskRandomSeed::~SimulationTaskRandomSeed()
  {
  }

  string SimulationTaskRandomSeed::name( void ) 
    const throw()
  {
    return "random_seed";
  }

  string SimulationTaskRandomSeed::description( void ) 
    const throw()
  {
    return "manages the random number generator's seed";
  }

  void SimulationTaskRandomSeed::run( SimulationController& sc) 
    throw(runtime_error)
  {
    
    const string error_open("Unable to open: ");
    const string error_write("Unable to write: ");
    const string error_read("Unable to read: ");

    const string action(sc.environment().required_string_param("action"));

    if (action == "load") {
	 
	 const string fname(sc.environment().required_string_param("filename"));
	 ifstream ifs;
	 ifs.open(fname.c_str());
	 if (!ifs.is_open() || !ifs.good()) 
	   throw runtime_error(error_open + fname);

	 ifs >> seed_;	
	 if (ifs.fail()) 
	   throw runtime_error(error_read + fname);
	 
	 ifs.close();
	 
    } else if (action == "create") {
	 
	 const string fname(sc.environment().required_string_param("filename"));
	 ofstream ofs;
	 ofs.open(fname.c_str());
	 if (!ofs.is_open() || !ofs.good()) 
	   throw runtime_error(error_open + fname);

	 seed_ = time(NULL);	
	 ofs << seed_;
	 if (ofs.fail()) 
	   throw runtime_error(error_write + fname);

	 ofs.close();	 
	
    } else if (action == "set") {
		 seed_ = sc.environment().required_int_param("seed");

    } else throw runtime_error("Invalid value for variable 'action'");

    //#ifdef WIN32
	 srand((int)seed_);
// #else
//     	 srand48(seed_);
// #endif
    
  }

  long int SimulationTaskRandomSeed::seed( void )
    const throw()
  {
    return seed_;
  }

} // namespace shawn

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/random_seed.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: random_seed.cpp,v $
 *-----------------------------------------------------------------------*/
