/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/messages/position/localization_dls_messages.h"


namespace localization
{

   LocalizationDLSInitMessage::
	   LocalizationDLSInitMessage( SimpleMatrix<double>* matrix_a,SimpleMatrix<double>* vector_r,SimpleMatrix<double>* covariance,shawn::Node* lt, std::vector<shawn::Node*>* beacons )
   : vector_r_(vector_r),linearization_tool_(lt),matrix_a_(matrix_a),beacons_(beacons),covariance_(covariance)
   {}
   // ----------------------------------------------------------------------
   LocalizationDLSInitMessage::
   LocalizationDLSInitMessage(const LocalizationDLSInitMessage* dls_message )
   : vector_r_(dls_message->vector_R()),linearization_tool_(dls_message->linearization_tool()),matrix_a_(dls_message->matrix_A()),beacons_(dls_message->beacons()),covariance_(dls_message->covariance())
   {}
   // ----------------------------------------------------------------------
   LocalizationDLSInitMessage::
   ~LocalizationDLSInitMessage()
   {}
   // ----------------------------------------------------------------------

   SimpleMatrix<double>*
	   LocalizationDLSInitMessage::
	   matrix_A( void )
	   const throw(){
		   return matrix_a_;
	}
      SimpleMatrix<double>*
		  LocalizationDLSInitMessage::
		  vector_R( void )
		  const throw(){
			  return vector_r_;
	  }

      shawn::Node*
		  LocalizationDLSInitMessage::
		  linearization_tool(void)
		  const throw(){
			  return linearization_tool_;
	  }
	  std::vector<shawn::Node*>*
		  LocalizationDLSInitMessage::
		  beacons(void)
		  const throw(){
			  return beacons_;
	  }
	   SimpleMatrix<double>*
		  LocalizationDLSInitMessage::
		  covariance( void )
		  const throw(){
			  return covariance_;
	  }

	   LocalizationDLSIterMessage::
		 LocalizationDLSIterMessage(const shawn::Vec* new_pos){
		new_pos_=new_pos;
	 }
	 LocalizationDLSIterMessage::
		~LocalizationDLSIterMessage()
	 {}

	   LocalizationDLSIterMessage::
		   LocalizationDLSIterMessage( const LocalizationDLSIterMessage* msg ){
		 new_pos_ = msg->new_pos();

	 }
	  const shawn::Vec*
		   LocalizationDLSIterMessage::
		   new_pos( void ) const throw(){
			   return new_pos_;
	   }
}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/position/localization_gpsfree_ncs_messages.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: localization_gpsfree_ncs_messages.cpp,v $
 *-----------------------------------------------------------------------*/
