/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_COMM_MODELS_MULTIPLE_CD_MODEL_H
#define __SHAWN_SYS_COMM_MODELS_MULTIPLE_CD_MODEL_H

#include "sys/communication_model.h"

#include <vector>

namespace shawn
{
	/**
	  *
	  */
	class MultipleCommunicationModel
		: public CommunicationModel
	{
	private:
		typedef std::vector<CommunicationModel*> CommModelVec;
		typedef CommModelVec::const_iterator CommModelVecIterator;

		CommModelVec cds_;
		CommunicationModel* default_;

	public:

		///@name construction / destruction
		///@{
    
		///
		MultipleCommunicationModel();
    
		///
		virtual ~MultipleCommunicationModel();
    
		///
		virtual void init( void ) throw();
    
		///@}


		///@name 
		///@{
    
		///
		void add_comm_model(CommunicationModel&) throw();
	
        ///
    	void set_default_comm_model(CommunicationModel&) throw();
    
		///@}

		///@name CommunicationModel interface
		///@{
		
        ///
		virtual bool can_communicate_bidi( const Node&, const Node& ) const throw();
		
        ///
		virtual bool can_communicate_uni( const Node&, const Node& ) const throw();
		
        /// Returns whether communication_upper_bound() returns a useful value
		virtual bool exists_communication_upper_bound( void ) const throw();
        
		/** If exists_communication_upper_bound(), nodes whose euclidean
		*  distance exceeds communication_upper_bound() can never communicate
		*  in any direction
		*/
		virtual double communication_upper_bound( void ) const throw();
        
		///
		virtual bool is_status_available_on_construction( void ) const throw();
        
		///@}

	};

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/multiple_communication_model.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: multiple_communication_model.h,v $
 *-----------------------------------------------------------------------*/

