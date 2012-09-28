/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/logging/logger.h"


namespace shawn
{

   Logger::
   Logger()
#ifdef ENABLE_LOGGING
      : logger_name_( LOG_DEFAULT_NAME ),
         logger_( __INIT_LOGGER_OBJECT( logger_name_ ) )
#endif    
   {}
   // ----------------------------------------------------------------------
#ifdef ENABLE_LOGGING   
   Logger::
   ~Logger()
   {}
#endif   
   // ----------------------------------------------------------------------
#ifdef ENABLE_LOGGING   
   __LOGGER_OBJECT&
   Logger::
   logger( void )
      throw()
   {
      return logger_;
   }
#endif   
   // ----------------------------------------------------------------------
#ifdef ENABLE_LOGGING   
   __LOGGER_OBJECT&
   Logger::
   logger( void )
      const throw()
   {
      return logger_;
   }
#endif   
   // ----------------------------------------------------------------------
#ifdef ENABLE_LOGGING   
   void
   Logger::
   set_logger_name( const std::string& name )
      throw()
   {
      logger_name_ = name;
      logger_ = __INIT_LOGGER_OBJECT( logger_name_ );
   }
#endif   
   // ----------------------------------------------------------------------
#ifdef ENABLE_LOGGING   
   const std::string&
   Logger::
   logger_name( void )
      const throw()
   {
      return logger_name_;
   }
#endif
}


/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/logging/logger.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: logger.cpp,v $
 *-----------------------------------------------------------------------*/
