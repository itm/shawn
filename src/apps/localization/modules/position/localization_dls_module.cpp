/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/position/localization_dls_module.h"
#include "apps/localization/math/localization_triangulation.h"
#include "apps/localization/messages/position/localization_dls_messages.h"
#include <cmath>
#include<limits>


namespace localization
{

	LocalizationDLSModule::
		LocalizationDLSModule()
		: state_( dls_wait )
	{

	}
	// ----------------------------------------------------------------------
	LocalizationDLSModule::
		~LocalizationDLSModule()
	{
		if(owner().owner().is_special_node())	
			delete beacons_;
	}
	void
		LocalizationDLSModule::
		boot( void )
		throw()
	{
		if(owner().owner().is_special_node()){
			owner_w().set_proc_type(owner().server);
			initialize();
		}
	}
	// ----------------------------------------------------------------------
	void
		LocalizationDLSModule::
		work( void )
		throw()
	{
		if ( state_ == dls_finished )
			return;

		if(state_ == dls_work && owner().dist_module_->finished() && owner().proc_type() == owner().unknown ){
			estimate_position();	
			state_ = dls_finished;			
		}
		if(owner().proc_type() != owner().unknown)
			state_ = dls_finished;
	}
	// ----------------------------------------------------------------------
	bool
		LocalizationDLSModule::
		finished( void )
		throw()
	{
		return state_ == dls_finished;
	}
	// ----------------------------------------------------------------------
	void
		LocalizationDLSModule::
		rollback( void )
		throw()
	{
		state_ = dls_wait;
	}
	// ----------------------------------------------------------------------
	bool 
		LocalizationDLSModule::
		process_message( const shawn::ConstMessageHandle& mh) throw(){
			const LocalizationDLSInitMessage* msg = dynamic_cast<const LocalizationDLSInitMessage*>(mh.get());
			if(msg!=NULL){
				if(owner().proc_type()!=owner().unknown 
					|| state_ != dls_wait)
					return true;
				else{		
					matrix_a_ = msg->matrix_A();
					vector_r_ = new SimpleMatrix<double>(*(msg->vector_R()));
					covariance_a_= msg->covariance();
					linearization_tool_= msg->linearization_tool();
					beacons_=msg->beacons();
					state_ = dls_work;
					send( new LocalizationDLSInitMessage(msg) );
				}
				return true;
			}
			return LocalizationModule::process_message(mh);
	}
	// ----------------------------------------------------------------------
	void 
		LocalizationDLSModule::
		initialize( void ) 
		throw(){
			shawn::Node* linearizationTool;

			shawn::Vec* tool;
			beacons_= new std::vector<shawn::Node*>();
			{
				shawn::World::node_iterator iter = owner_w().owner_w().world_w().begin_nodes_w();
				shawn::World::node_iterator end = owner_w().owner_w().world_w().end_nodes_w();
				for( ; iter != end ; iter++ ){
					LocalizationProcessor* temp= iter->get_processor_of_type_w<LocalizationProcessor>();
					if(temp!=NULL){
						if(temp->is_anchor() && !temp->is_server() ){
							beacons_->push_back(&temp->owner_w());
						}
					}
				}
			}
			linearizationTool=beacons_->front();
			if(linearizationTool->has_est_position()){
				shawn::Vec est_pos(linearizationTool->est_position());
				tool = &est_pos;
			}
			else
			{
				shawn::Vec real_pos(linearizationTool->real_position());
				tool = &real_pos;
			}

			beacons_->erase(beacons_->begin());
			matrix_a_ = new SimpleMatrix<double>(beacons_->size(),3); 
			vector_r_ =new SimpleMatrix<double>(beacons_->size(),1);

			std::vector<shawn::Node*>::iterator iter=beacons_->begin();
			std::vector<shawn::Node*>::iterator end = beacons_->end();
			for(int count =0; iter!= end; iter++,count++){
				shawn::Vec* tmp_pos;
				if((*iter)->has_est_position())
				{
					shawn::Vec est_pos((*iter)->est_position());
					tmp_pos=&est_pos;
				}
				else
				{
					shawn::Vec real_pos((*iter)->real_position());
					tmp_pos=&real_pos;
				}
				shawn::Vec pos( *tmp_pos - *tool );
				matrix_a_->at(count,0)=pos.x();
				matrix_a_->at(count,1)=pos.y();
				matrix_a_->at(count,2)=pos.z();
				vector_r_->at(count,0)=(pos.x()*pos.x()) + (pos.y()*pos.y()) +(pos.z()*pos.z()); //pos.euclidean_norm();
			}
			covariance_a_ = new SimpleMatrix<double>(matrix_a_->covariance());
			*matrix_a_ = matrix_a_->transposed();
			*matrix_a_= *covariance_a_ * *matrix_a_;
			send( new LocalizationDLSInitMessage(matrix_a_,vector_r_,covariance_a_,linearizationTool,beacons_));
	}
	// ----------------------------------------------------------------------
	void 
		LocalizationDLSModule::
		estimate_position( void ) throw(){

			shawn::Vec* est_pos;
			double distance_r1,distance_r2;
			int count =0;
			distance_r1 = owner().estimate_distance(node(), *linearization_tool_);
			distance_r1 *=distance_r1;
			NeighborInfoList neighbors;
			collect_neighbors( neighborhood(), lat_anchors, neighbors );

			for( std::vector<shawn::Node*>::iterator iter = beacons_->begin(); iter!=beacons_->end();iter++,count++){
				for( ConstNeighborInfoListIterator iter1 = neighbors.begin(); iter1!=neighbors.end(); iter1++){
					if((*iter)->id() == (*iter1)->node().id() ){
						distance_r2 = (*iter1)->distance();
						//distance_r2 = owner().estimate_distance(node(), **iter);

						if(distance_r2 == std::numeric_limits<double>::max() ||distance_r2 == std::numeric_limits<double>::min())
							vector_r_->at(count,0) = 0;
						else    
							vector_r_->at(count,0) = 0.5*( distance_r1 - (distance_r2*distance_r2) + (vector_r_->at(count,0)));
						break;
					}
				}		
			}	
			SimpleMatrix<double>*  temp = new SimpleMatrix<double>(3,1);	
			*temp= *matrix_a_ * *vector_r_;
			est_pos = new shawn::Vec(temp->at(0,0),temp->at(1,0),temp->at(2,0));
			*est_pos += (linearization_tool_->has_est_position())?
				(linearization_tool_->est_position()):(linearization_tool_->real_position());
			node_w().set_est_position( *est_pos );
			delete temp;
	}
	// ----------------------------------------------------------------------
	SimpleMatrix<double>* 
		LocalizationDLSModule::
		getMatrix( void ) throw()
	{
		return matrix_a_;
	}
	// ----------------------------------------------------------------------
	SimpleMatrix<double>*
		LocalizationDLSModule::
		getVector( void ) throw()
	{
		return vector_r_;
	}
	// ----------------------------------------------------------------------
	SimpleMatrix<double>* 
		LocalizationDLSModule::
		getCovariance( void ) throw()
	{
		return covariance_a_;
	}
	// ----------------------------------------------------------------------
	std::vector<shawn::Node*>* 
		LocalizationDLSModule::
		getBeacons( void ) throw()
	{
		return beacons_;
	}
	// ----------------------------------------------------------------------
	shawn::Node* 
		LocalizationDLSModule::
		getLinearizationTool( void ) throw(){
			return linearization_tool_;
	}
}// namespace localization
#endif
