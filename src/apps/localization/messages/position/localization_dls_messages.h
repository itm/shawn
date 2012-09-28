/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MESSAGES_DLS_MESSAGES_H
#define __SHAWN_APPS_LOCALIZATION_MESSAGES_DLS_MESSAGES_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION
#include <vector>

#include "sys/node.h"
#include "sys/message.h"
#include "sys/vec.h"
#include "apps/localization/util/localization_defutils.h"
#include "apps/localization/math/localization_simple_matrix.h"
//#include "apps/localization/neighborhood/localization_local_coordinate_system.h"


namespace localization
{

   class LocalizationDLSInitMessage
      : public shawn::Message
   {

   public:

	   LocalizationDLSInitMessage(SimpleMatrix<double>*,SimpleMatrix<double>*,SimpleMatrix<double>*,shawn::Node*,std::vector<shawn::Node*>*);
      LocalizationDLSInitMessage( const LocalizationDLSInitMessage* );
      virtual ~LocalizationDLSInitMessage();

      ///
      virtual SimpleMatrix<double>* matrix_A( void ) const throw();
      virtual SimpleMatrix<double>* vector_R( void ) const throw();
      virtual shawn::Node* linearization_tool(void) const throw();
	  virtual std::vector<shawn::Node*>* beacons( void ) const throw();
	  virtual SimpleMatrix <double>* covariance(void) const throw();
   private:

      SimpleMatrix <double>* vector_r_;
      shawn::Node* linearization_tool_;
      SimpleMatrix <double>* matrix_a_;
	  std::vector<shawn::Node*>* beacons_;
	SimpleMatrix <double>* covariance_;
   };

class LocalizationDLSIterMessage
      : public shawn::Message
   {

   public:

	   LocalizationDLSIterMessage(const shawn::Vec* new_pos);
      LocalizationDLSIterMessage( const LocalizationDLSIterMessage* );
      virtual ~LocalizationDLSIterMessage();

	  const shawn::Vec* new_pos( void ) const throw();	
   private:

	  const shawn::Vec* new_pos_;
   };



}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/position/localization_gpsfree_ncs_messages.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: localization_gpsfree_ncs_messages.h,v $
 *-----------------------------------------------------------------------*/
