/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_COMM_MODELS_STOCHASTIC_COMM_MODEL_H
#define __SHAWN_SYS_COMM_MODELS_STOCHASTIC_COMM_MODEL_H

#include "sys/misc/random/uniform_random_variable.h"
#include "sys/communication_model.h"
#include "sys/misc/dynamic_node_array.h"
#include "sys/misc/node_change_listener.h"
#include <set>


namespace shawn
{

   class StochasticCommunicationModel
      : public CommunicationModel
   {
   public:

      ///@name construction / destruction
      ///@{
      ///
      StochasticCommunicationModel( double max_probability, double smooth_factor );
      ///
      virtual ~StochasticCommunicationModel();
      ///
      virtual void init( void ) throw();
      ///@}

      
      ///@name communication range
      ///@{
      ///
      virtual void set_transmission_range( double ) throw();
      ///
      virtual double transmission_range( void ) const throw();
      ///@}

      ///@name CommunicationModel interface
      ///@{
      ///
      virtual bool can_communicate_bidi( const Node&, const Node& ) const throw();
      
      ///
      virtual bool can_communicate_uni( const Node&, const Node& ) const throw();
      
      /// returns whether communication_upper_bound() returns a useful value
      virtual bool exists_communication_upper_bound( void ) const throw();
      
      /** if exists_communication_upper_bound(), nodes whose euclidean
       *  distance exceeds communication_upper_bound() can never communicate
       *  in any direction
       */
      virtual double communication_upper_bound( void ) const throw();
      
      ///
      virtual bool is_status_available_on_construction( void ) const throw();
   
      ///@}

   private:
      double range_;
	  double max_probability_;
	  double smooth_factor_;
	  bool support_mobility_;
	  UniformRandomVariable urv;
	  DynamicNodeArray< std::set<const Node*> >*  connectivity_;

	  ///
	  void calculate_connectivity();

	  ///
	  bool has_connection( const Node& u, const Node& v ) const throw() ;
   public:
	  double communication_probability(double distance) const;
	  
   };
   

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/stochastic_comm_model.h,v $
 * Version $Revision: 298 $
 * Date    $Date: 2008-12-04 17:53:45 +0100 (Thu, 04 Dec 2008) $
 *-----------------------------------------------------------------------
 * $Log: stochastic_comm_model.h,v $
 *-----------------------------------------------------------------------*/
