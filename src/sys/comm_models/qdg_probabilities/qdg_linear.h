/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_COMM_MODELS_QDG_LINEAR_PROBABILITY_H
#define __SHAWN_SYS_COMM_MODELS_QDG_LINEAR_PROBABILITY_H

#include "sys/comm_models/qdg_probabilities/qdg_probability.h"

namespace shawn
{


   class QDGLinearProbability
      : public QDGProbability
   {
   public:

      ///@name construction / destruction
      ///@{
      ///
      QDGLinearProbability( double, double );
      ///
      virtual ~QDGLinearProbability();
      ///@}

      
      ///@name Access
      ///@{
      ///
      virtual double inner_range( void ) const throw();
      ///
      virtual double outer_range( void ) const throw();
      ///
      virtual double probability( double ) const throw();
      ///@}

   private:
      double range1_;
      double range2_;
   };
   

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/qdg_probabilities/qdg_linear.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: qdg_linear.h,v $
 *-----------------------------------------------------------------------*/
