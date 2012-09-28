/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MODULES_POSITION_DLS_MODULE_H
#define __SHAWN_APPS_LOCALIZATION_MODULES_POSITION_DLS_MODULE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/localization_module.h"
#include "apps/localization/math/localization_simple_matrix.h"


namespace localization
{

	///
	/**
	*/
	class LocalizationDLSModule
		: public LocalizationModule
	{

	public:

		///@name construction / destruction
		///@{
		///
		LocalizationDLSModule();
		///
		virtual ~LocalizationDLSModule();
		///@}

		virtual void boot( void ) throw();
		///@name standard methods startup/simulation steps
		///@{
		/**
		*/
		virtual void work( void ) throw();
		///@}

		///@name module status info
		///@{
		/** \return \c true
		*/
		virtual bool finished( void ) throw();
		///@}
		virtual void rollback( void ) throw();
		virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
		virtual void estimate_position( void ) throw();


		virtual void initialize( void ) throw();
		virtual SimpleMatrix<double>* getMatrix( void ) throw();
		virtual SimpleMatrix<double>* getVector( void ) throw();
		virtual SimpleMatrix<double>* getCovariance( void ) throw();
		virtual std::vector<shawn::Node*>* getBeacons( void ) throw();
		virtual shawn::Node* getLinearizationTool( void ) throw();

	private:

		enum DLSState
		{ dls_wait,
		dls_work,
		dls_finished
		};

		DLSState state_;

		SimpleMatrix<double>* matrix_a_;
		SimpleMatrix<double>* vector_r_;
		SimpleMatrix<double>* covariance_a_;
		shawn::Node* linearization_tool_;
		std::vector<shawn::Node*>* beacons_;
	};

}// namespace localization
#endif
#endif
