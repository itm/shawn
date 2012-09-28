/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                **
 ** This part of Shawn is free software; you can redistribute it and/or**
 ** modify it under the terms of the BSD License. Refer to the		   **
 ** shawn-licence.txt file in the root of the Shawn source tree for	   **
 ** further details.												   **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
 ** \author Torsten Teubler <teubler@itm.uni-luebeck.de>			   **
 **                                                                    **
 ************************************************************************/

#ifndef __SHAWN_LEGACYAPPS_AUTOCASTAPP_PROCESSOR_H
#define __SHAWN_LEGACYAPPS_AUTOCASTAPP_PROCESSOR_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

#include <set>
#include "autocast_application.h"
#include "sys/processor.h"

namespace autocast
{

   class AutoCastAppProcessor
	   : public shawn::Processor, public AutoCastApplication
   {
   public:
      AutoCastAppProcessor();
      virtual ~AutoCastAppProcessor();

      virtual void boot( void ) throw();
	  virtual void special_boot() throw();
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      virtual void work( void ) throw();
	  virtual bool receive_DataUnit( const ConstDataUnitHandle& ) throw();
	  virtual const autocast::ConstDataUnitHandle& refresh( const ConstDataUnitHandle& ) throw();

   private:
	   void disseminate_DataUnit() throw();
	   double sending_point_x_;
	   double sending_point_y_;
	   static int dataUnit_id_counter_;
	   int disseminate_every_n_round_;
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
