/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <string>
#include <cstdlib>
#include "sys/simulation/simulation_environment.h"
#include "sys/util/string_conv.h"

namespace shawn
{

   SimulationEnvironment::ParameterSet::
   ParameterSet()
   {}
   // ----------------------------------------------------------------------
   SimulationEnvironment::ParameterSet::
   ~ParameterSet()
   {}
   // ----------------------------------------------------------------------
   void
   SimulationEnvironment::ParameterSet::
   add( const std::string& k,
        const std::string& v )
      throw()
   {
      parms_[k] = v;
   }
   // ----------------------------------------------------------------------
   void
   SimulationEnvironment::ParameterSet::
   add_set( const SimulationEnvironment::ParameterSet& ps )
      throw()
   {
      for( DataMap::const_iterator
              it    = ps.parms_.begin(),
              endit = ps.parms_.end();
           it != endit; ++it )
         parms_[ it->first ] = it->second;
   }
   // ----------------------------------------------------------------------
   bool
   SimulationEnvironment::ParameterSet::
   get( const std::string&  k,
        const std::string*& v )
      const throw()
   {
      DataMap::const_iterator ent = parms_.find(k);
      if( ent == parms_.end() )
         return false;
      else
         {
            v=&(ent->second);
            return true;
         }
   }








   SimulationEnvironment::
   SimulationEnvironment()
   {}
   // ----------------------------------------------------------------------
   SimulationEnvironment::
   ~SimulationEnvironment()
   {}
   // ----------------------------------------------------------------------
   SimulationEnvironment::
   SimulationEnvironment( const SimulationEnvironment& )
   { abort(); }
   // ----------------------------------------------------------------------
   void
   SimulationEnvironment::
   push_parameters( const ParameterSetHandle& psh ) 
      throw()
   {
      stack_parms_.push_front(psh);
   }
   // ----------------------------------------------------------------------
   void
   SimulationEnvironment::
   pop_parameters()
      throw()
   {
      assert( !stack_parms_.empty() );
      stack_parms_.pop_front();
   }
   // ----------------------------------------------------------------------
   void
   SimulationEnvironment::
   add_parameters_low_prio( const ParameterSetHandle& psh ) 
      throw()
   {
      lo_prio_parms_.add_set( *psh );
   }
   // ----------------------------------------------------------------------
   void
   SimulationEnvironment::
   add_parameters_high_prio( const ParameterSetHandle& psh )
      throw()
   {
      hi_prio_parms_.add_set( *psh );
   }
   // ----------------------------------------------------------------------
   void
   SimulationEnvironment::
   add_parameter_low_prio( const std::string& k,
                           const std::string& v )
      throw()
   {
      lo_prio_parms_.add(k,v);
   }
   // ----------------------------------------------------------------------
   void
   SimulationEnvironment::
   add_parameter_high_prio( const std::string& k,
                            const std::string& v )
      throw()
   {
      hi_prio_parms_.add(k,v);
   }
   // ----------------------------------------------------------------------
   const std::string&
   SimulationEnvironment::
   required_string_param( const std::string& key )
      const throw( std::runtime_error )
   {
      const std::string* val;

      if( find_param(key,val) )
         return *val;
      else
         throw std::runtime_error(std::string("No value provided for required parameter '") +
                                  key +
                                  std::string("'!") );
   }
   // ----------------------------------------------------------------------
   const std::string&
   SimulationEnvironment::
   optional_string_param( const std::string& key,
                          const std::string& defval,
                          bool* is_set )
      const throw( std::runtime_error )
   {
      const std::string* val;

      if( find_param(key,val) )
         {
            if( is_set != NULL ) *is_set=true;
            return *val;
         }
      else
         {
            if( is_set != NULL ) *is_set=false;
            return defval;
         }
   }
   // ----------------------------------------------------------------------
   int
   SimulationEnvironment::
   required_int_param( const std::string& key )
      const throw( std::runtime_error )
   {
      return string2int( key, required_string_param(key) );
   }
   // ----------------------------------------------------------------------
   int
   SimulationEnvironment::
   optional_int_param( const std::string& key,
                       int defval,
                       bool* is_set )
      const throw( std::runtime_error )
   {
      const std::string* val;

      if( find_param(key,val) )
         {
            if( is_set != NULL ) *is_set=true;
            return string2int(key,*val);
         }
      else
         {
            if( is_set != NULL ) *is_set=false;
            return defval;
         }
   }
   // ----------------------------------------------------------------------
   bool
   SimulationEnvironment::
   required_bool_param( const std::string& key )
      const throw( std::runtime_error )
   {
      return string2bool( key, required_string_param(key) );
   }
   // ----------------------------------------------------------------------
   bool
   SimulationEnvironment::
   optional_bool_param( const std::string& key,
                        bool defval,
                        bool* is_set  )
      const throw( std::runtime_error )
   {
      const std::string* val;

      if( find_param(key,val) )
         {
            if( is_set != NULL ) *is_set=true;
            return string2bool(key,*val);
         }
      else
         {
            if( is_set != NULL ) *is_set=false;
            return defval;
         }
   }
   // ----------------------------------------------------------------------
   double
   SimulationEnvironment::
   required_double_param( const std::string& key )
      const throw( std::runtime_error )
   {
      return string2double( key, required_string_param(key) );
   }
   // ----------------------------------------------------------------------
   double
   SimulationEnvironment::
   optional_double_param( const std::string& key, double defval,
                          bool* is_set  )
      const throw( std::runtime_error )
   {
      const std::string* val;

      if( find_param(key,val) )
         {
            if( is_set != NULL ) *is_set=true;
            return string2double(key,*val);
         }
      else
         {
            if( is_set != NULL ) *is_set=false;
            return defval;
         }
   }
   // ----------------------------------------------------------------------
   int
   SimulationEnvironment::
   string2int( const std::string& key,
               const std::string& value )
      const throw( std::runtime_error )
   {
      try {
         return conv_string_to_int(value);
      }
      catch( std::runtime_error& ) {
         throw std::runtime_error( std::string("Value '") +
                                   value +
                                   std::string("' for parameter '") +
                                   key +
                                   std::string("' is no integer.") );
      }
      // impossible to reach
      abort();
      return 0;
   }
   // ----------------------------------------------------------------------
   double
   SimulationEnvironment::
   string2double( const std::string& key,
                  const std::string& value )
      const throw( std::runtime_error )
   {
      try {
         return conv_string_to_double(value);
      }
      catch( std::runtime_error& ) {
         throw std::runtime_error( std::string("Value '") +
                                   value +
                                   std::string("' for parameter '") +
                                   key +
                                   std::string("' is not numeric.") );
      }
      // impossible to reach
      abort();
      return 0.0;
   }
   // ----------------------------------------------------------------------
   bool
   SimulationEnvironment::
   find_param( const std::string&  k,
               const std::string*& v )
      const throw()
   {
      if( hi_prio_parms_.get(k,v) ) return true;

      for( std::deque<ParameterSetHandle>::const_iterator
              it    = stack_parms_.begin(),
              endit = stack_parms_.end();
           it != endit; ++it )
         if( (**it).get(k,v) ) return true;

      return lo_prio_parms_.get(k,v);
   }
   // ----------------------------------------------------------------------
   bool
   SimulationEnvironment::
   string2bool( const std::string& key,
                const std::string& value )
      const throw( std::runtime_error )
   {
      try {
         return conv_string_to_bool(value);
      }
      catch( std::runtime_error& ) {
         throw std::runtime_error( std::string("Value '") +
                                   value +
                                std::string("' for parameter '") +
                                   key +
                                   std::string("' is no bool.") );
      }
      // impossible to reach
      abort();
      return false;
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/simulation_environment.cpp,v $
 * Version $Revision: 182 $
 * Date    $Date: 2008-04-07 15:14:38 +0200 (Mon, 07 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: simulation_environment.cpp,v $
 *-----------------------------------------------------------------------*/
