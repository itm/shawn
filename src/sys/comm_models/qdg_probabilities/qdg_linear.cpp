/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/qdg_probabilities/qdg_linear.h"

namespace shawn
{

   QDGLinearProbability::
   QDGLinearProbability( double r1, double r2 )
      : range1_(r1),
        range2_(r2)
   {}
   // ----------------------------------------------------------------------
   QDGLinearProbability::
   ~QDGLinearProbability()
   {}
   // ----------------------------------------------------------------------
   double
   QDGLinearProbability::
   inner_range( void )
      const throw()
   {
      return range1_;
   }
   // ----------------------------------------------------------------------
   double 
   QDGLinearProbability::
   outer_range( void )
      const throw()
   {
      return range2_;
   }
   // ----------------------------------------------------------------------
   double
   QDGLinearProbability::
   probability( double r )
      const throw()
   {
      if( r<=range1_ ) return 1.0;
      if( r>range2_ ) return 0.0;
      return (range2_-r)/(range2_-range1_);
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/qdg_probabilities/qdg_linear.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: qdg_linear.cpp,v $
 *-----------------------------------------------------------------------*/
