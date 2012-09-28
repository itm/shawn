/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/refinement/localization_iter_dls_module.h"
#include "apps/localization/math/localization_triangulation.h"
#include "apps/localization/util/localization_defutils.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/misc/random/basic_random.h"


using namespace shawn;

namespace localization
{

	LocalizationiDLSModule::
		LocalizationiDLSModule()
		: state_               ( il_init ),
		last_useful_msg_(0)
	{}
	// ----------------------------------------------------------------------
	LocalizationiDLSModule::
		~LocalizationiDLSModule()
	{}
	// ----------------------------------------------------------------------
	void
		LocalizationiDLSModule::
		boot( void )
		throw()
	{
//		const SimulationEnvironment& se = node().world().simulation_controller().environment();
		iter_positions_ = new std::vector<const shawn::Vec*>();
		neighbor_cnt_ = owner().owner().get_adjacent_nodes().size();
	}
	// ----------------------------------------------------------------------
	bool
		LocalizationiDLSModule::
		process_message( const ConstMessageHandle& mh )
		throw()
	{
		if ( dynamic_cast<const LocalizationDLSIterMessage*>( mh.get() ) != NULL )
		{
			if(owner().proc_type() == owner().unknown)
			return process_iter_dls_message(
				*dynamic_cast<const LocalizationDLSIterMessage*>( mh.get() ));
			return true;
		}

		return LocalizationModule::process_message( mh );
	}
	// ----------------------------------------------------------------------
	void
		LocalizationiDLSModule::
		work( void )
		throw()
	{
		if( simulation_round() - last_useful_msg_ > observer().idle_time() )
			state_ = il_work;

		if ( state_ == il_finished )
			return;
		if ( state_ == il_init && owner().pos_module_->finished() )
		{
			if ( node().has_est_position() && owner().proc_type() == owner().unknown)
				{
					LocalizationDLSModule* ldlsm = dynamic_cast<LocalizationDLSModule*>(owner_w().pos_module_);
					if(ldlsm != NULL){
						covariance_a_ = new SimpleMatrix<double>(*(ldlsm->getCovariance()));
						linearization_tool_ = ldlsm->getLinearizationTool();
						state_ = il_wait;
					}
					send( new LocalizationDLSIterMessage(&node().est_position() ) );
				}
		if( state_ == il_work )
			iter_step();
		}
		if(owner().proc_type() != owner().unknown)
				state_ = il_finished;

	}
	// ----------------------------------------------------------------------
	bool
		LocalizationiDLSModule::
		process_iter_dls_message( const LocalizationDLSIterMessage& lidlsm )
		throw()
	{
		iter_positions_->push_back(lidlsm.new_pos());
		if(iter_positions_->size() ==neighbor_cnt_){
			iter_step();
			state_ = il_finished;
		}
		return true;

	}
	// ----------------------------------------------------------------------
	void
		LocalizationiDLSModule::
		iter_step( void )
		throw()
	{
		SimpleMatrix<double>* w = new SimpleMatrix<double>(3,1);
		SimpleMatrix<double>* x = new SimpleMatrix<double>(3,1);
		x->at(0,0) = node().est_position().x();
		x->at(1,0) = node().est_position().y();
		x->at(2,0) = node().est_position().z();
		SimpleMatrix<double>* temp1 = new SimpleMatrix<double>(*x);
		for(unsigned int i = 0; i< iter_positions_->size(); i++){
			w->at(0,0) = iter_positions_->at(i)->x();
			w->at(1,0) = iter_positions_->at(i)->y();
			w->at(2,0) = iter_positions_->at(i)->z();
			SimpleMatrix<double>* u = new SimpleMatrix<double>(*covariance_a_ * *w);
			SimpleMatrix<double>* temp = new SimpleMatrix<double>(w->transposed() * *u);
			*u= 1/(1+ temp->at(0,0)) * *u * u->transposed();
			*covariance_a_ = *covariance_a_ - *u;

			temp->at(0,0) = euclidean_distance( *iter_positions_->at(i), linearization_tool_->est_position());
			SimpleMatrix<double>* temp2 = new SimpleMatrix<double>(*covariance_a_ * *w);
			SimpleMatrix<double>* temp3 = new SimpleMatrix<double>(*temp - (w->transposed()* *x));
			*temp1=*x;
			*temp2 = temp3->at(0,0) * *temp2;
			*temp1 = *x + *temp2;
			*x=*temp1;
		}
		Vec erg(x->at(0,0),x->at(1,0),x->at(2,0));
		node_w().set_est_position(erg);
		state_ = il_finished;
	}
	// ----------------------------------------------------------------------
	void
		LocalizationiDLSModule::
		initialize(void) throw(){

	}
	// ----------------------------------------------------------------------
	bool
		LocalizationiDLSModule::
		finished( void )
		throw()
	{
		return state_ == il_finished;
	}
	// ----------------------------------------------------------------------
	void
		LocalizationiDLSModule::
		rollback( void )
		throw()
	{
		state_ = il_init;
	}

}// namespace localization

#endif
