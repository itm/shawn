/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_SIMULATION_SIMULATION_ENVIRONMENT_H
#define __SHAWN_SYS_SIMULATION_SIMULATION_ENVIRONMENT_H

#include "shawn_config.h"
#include "sys/util/refcnt_pointable.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"
#include "sys/tag_container.h"

#include <map>
#include <deque>
#include <stdexcept>

namespace shawn
{

   /// Access to configuration variables \todo use string_utils!
   class SimulationEnvironment
      : public TagContainer
   {
   public:
      DECLARE_HANDLES(ParameterSet);
      class ParameterSet
         : public RefcntPointable
      {
      public:
         ParameterSet();
         virtual ~ParameterSet();

         void add( const std::string&,
                   const std::string& )
            throw();
         void add_set( const ParameterSet& )
            throw();
         bool get( const std::string&,
                   const std::string*& )
            const throw();
      private:
         typedef
         std::map<std::string,std::string>
         DataMap;

         DataMap parms_;
      };



      SimulationEnvironment();
      ~SimulationEnvironment();





      void push_parameters( const ParameterSetHandle& ) throw();   
      void pop_parameters() throw();
      void add_parameters_low_prio( const ParameterSetHandle& ) throw();
      void add_parameters_high_prio( const ParameterSetHandle& ) throw();
      void add_parameter_low_prio( const std::string&, const std::string& ) throw();
      void add_parameter_high_prio( const std::string&, const std::string& ) throw();
   
      const std::string& required_string_param( const std::string& ) const throw( std::runtime_error );
      const std::string& optional_string_param( const std::string&, const std::string&, bool* is_set=NULL ) const throw( std::runtime_error );
   
      int required_int_param( const std::string& ) const throw( std::runtime_error );
      int optional_int_param( const std::string&, int, bool* is_set=NULL ) const throw( std::runtime_error );

      double required_double_param( const std::string& ) const throw( std::runtime_error );
      double optional_double_param( const std::string&, double, bool* is_set=NULL ) const throw( std::runtime_error );

      bool required_bool_param( const std::string& ) const throw( std::runtime_error );
      bool optional_bool_param( const std::string&, bool, bool* is_set=NULL ) const throw( std::runtime_error );

      bool find_param( const std::string&, const std::string*& )
         const throw();
   
   protected:
      int string2int( const std::string& key,
                      const std::string& value ) const throw( std::runtime_error );
      double string2double( const std::string& key,
                            const std::string& value ) const throw( std::runtime_error );
      bool string2bool( const std::string& key,
                        const std::string& value ) const throw( std::runtime_error );

   private:
      ParameterSet hi_prio_parms_;
      ParameterSet lo_prio_parms_;
      std::deque<ParameterSetHandle> stack_parms_;

      
      SimulationEnvironment( const SimulationEnvironment& );
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/simulation_environment.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: simulation_environment.h,v $
 *-----------------------------------------------------------------------*/
