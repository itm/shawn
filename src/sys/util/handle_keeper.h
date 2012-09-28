/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_UTIL_HANDLE_KEEPER_H
#define __SHAWN_SYS_UTIL_HANDLE_KEEPER_H

#include <map>
#include <string>
#include <stdexcept>
#include <cstdlib>

#include "sys/util/handle_keeper_base.h"
#include "sys/logging/logger.h"



namespace shawn
{


   template<typename T>
   class HandleKeeper
      : public HandleKeeperBase
   {
   public:
      typedef RefcntPointer<T> Handle;
      typedef RefcntPointer<const T> ConstHandle;
      typedef std::map<std::string,Handle> HandleMapType;

      HandleKeeper( const std::string& s, const std::string& group )
         : self_name_( s ),
            group_( group )
      {}
      virtual ~HandleKeeper()
      {}

      virtual void add( const Handle& h )
         throw( std::runtime_error )
      {
         std::string nam = h->name();
         if( handle_map_.find(nam) != handle_map_.end() )
            throw std::runtime_error( self_name_ +
                                      std::string(": register duplicate name '") +
                                      nam +
                                      std::string("'") );

#ifdef ENABLE_LOGGING
         Logger* logger = dynamic_cast<Logger*>( h.get() );
         if ( logger != NULL )
         {
            logger->set_logger_name( group_ + "." + nam );
            logger = 0;
         }
#endif

         handle_map_[nam]=h;
      }

      virtual void print_contents( std::ostream& os,
                                   bool verbose )
         const throw()
      {
         for( typename HandleMapType::const_iterator
                 it    = handle_map_.begin(),
                 endit = handle_map_.end();
              it != endit; ++it )
            if( verbose )
               os << it->first << ": " << it->second->description() << std::endl;
            else
               os << it->first << std::endl;
      }

      virtual ConstHandle find( const std::string& n )
         const throw( std::runtime_error )
      {
         typename HandleMapType::const_iterator ent = handle_map_.find(n);
         if( ent==handle_map_.end() )
            throw std::runtime_error( self_name_ +
                                      std::string(":'") +
                                      n +
                                      std::string("' not registered.") );

         return ConstHandle(ent->second);
      }


      virtual const Handle& find_w( const std::string& n )
         throw( std::runtime_error )
      {
         typename HandleMapType::iterator ent = handle_map_.find(n);
         if( ent==handle_map_.end() )
            throw std::runtime_error( self_name_ +
                                      std::string(":'") +
                                      n +
                                      std::string("' not registered.") );

         return ent->second;
      }

      /** Returns whether a given name (identifier) already exists in the set of handles
       */
      virtual bool has( const std::string& n )
         throw( )
      {
    	  for( typename HandleMapType::const_iterator it = handle_map_.begin(), endit = handle_map_.end(); it != endit; ++it )
    		  if( it->first == n )
    			  return true;
    	  return false;
      }
      
      
	  virtual typename HandleMapType::iterator begin_handles_w(void)
		  throw()
	  {
		  return handle_map_.begin();
	  }

	  virtual typename HandleMapType::iterator end_handles_w(void)
		  throw()
	  {
		  return handle_map_.end();
	  }


      virtual ConstKeeperManagedHandle find_managed( const std::string& n )
         const throw( std::runtime_error )
      { return ConstKeeperManagedHandle(find(n).get()); }
      virtual KeeperManagedHandle 
      find_managed_w( const std::string& n ) throw( std::runtime_error )
      { return KeeperManagedHandle(find_w(n).get()); }

      virtual const std::string& name( void ) const throw()
      { return self_name_; }
      virtual const std::string& group( void ) const throw()
      { return group_; }

		 
   private:
	  HandleMapType handle_map_;
      HandleKeeper( const HandleKeeper<T>& ) { abort(); }

      
      std::string self_name_;
      std::string group_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/util/handle_keeper.h,v $
 * Version $Revision: 203 $
 * Date    $Date: 2008-05-30 15:10:41 +0200 (Fri, 30 May 2008) $
 *-----------------------------------------------------------------------
 * $Log: handle_keeper.h,v $
 *-----------------------------------------------------------------------*/
