/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_DISTANCE_ESTIMATES_NEIGHBORHOOD_INTERSECTION_DISTANCE_ESTIMATE_H
#define __SHAWN_SYS_DISTANCE_ESTIMATES_NEIGHBORHOOD_INTERSECTION_DISTANCE_ESTIMATE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_DISTEST


#include "sys/node_distance_estimate.h"
#include "sys/node.h"
#include <vector>
#include "sys/comm_models/rim_comm_model.h"

#define M_UNI  1
#define M_BIDI_AVG 2
#define M_BIDI_MIN 3
#define M_BIDI_MAX 4
#define M_BIDI_AVG2 5


using namespace std;

namespace shawn
{


	   class CommunicationModelFunction 
	   {
	   public:
		   CommunicationModelFunction( ) { min_x_ = min_y_ = -2.0; max_x_ = max_y_ = 2.0; }
		   virtual ~CommunicationModelFunction() {};
		   virtual double stdIntegral( double step );
		   virtual double multIntegral( double dist, double step );
           virtual double ThreeDFunction( double x, double y, double x_offset );
           virtual double TwoDFunction( double x ) = 0;
		   virtual std::string name() = 0;
		   virtual void gnuplot3D( std::string filename, bool mult, double dist );
		   virtual void gnuplot2D( std::string filename );
		  virtual vector<double> calculateEstimationTable(double step);
 
	   protected:
		   double min_x_, max_x_, min_y_, max_y_;

	   };


	   class LinearCommunicationModelFunction : public CommunicationModelFunction
	   {
	   public:
		   LinearCommunicationModelFunction( double smooth_factor ); 
           virtual double TwoDFunction( double x );
		   virtual std::string name() {return "linear";};
	   protected:
		   double smooth_factor_;
	   };

	   class CircleCommunicationModelFunction : public CommunicationModelFunction
	   {
	   public:
           virtual double TwoDFunction( double x );
		   virtual std::string name() {return "circle";};
	   };

	   class VectorCommunicationModelFunction : public CommunicationModelFunction
	   {
	   public:
		   VectorCommunicationModelFunction( ): ready_(false) {}; 
		   VectorCommunicationModelFunction( vector<double> v): ready_(true) {setVector(v);}; 
		   virtual double TwoDFunction( double x );
		   virtual void setVector( vector<double> v );
		   virtual std::string name() {return "vector";};
	   protected:
		   bool ready_;
		   vector<double> vec_;
	   };

	   class RimModelFunction : public CommunicationModelFunction
	   {
	   public:
		   RimModelFunction( RimCommModel& rim, int model_count ) :  rim_(rim), iterations_(model_count) {}; 
   		   virtual vector<double> calculateEstimationTable(double step);
		   virtual std::string name() {return "rim";};
		   virtual double TwoDFunction( double x ) {return 0;};

	   protected:
			RimCommModel& rim_ ;
			int iterations_;
	   };


   class NeighborhoodIntersectionDistanceEstimate
      : public NodeDistanceEstimate
   {
   public:
	  NeighborhoodIntersectionDistanceEstimate();
	  virtual ~NeighborhoodIntersectionDistanceEstimate();
      
      virtual bool estimate_distance( const Node& source,
                                      const Node& target,
                                      double& result ) const throw();
      virtual bool 		getFraction( const Node& source,
		const Node& target,
		double& intersection,
		double& sourceSize,
		double& targetSize)
		const throw();

      virtual double calcEstimate( double fraction, double radius )
         const throw();
	  virtual void set_mode(int newmode );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

	  virtual void setCommunicationModelFunction( CommunicationModelFunction* cmf );
	  virtual void writeEstimationTable( );
	  virtual void gnuplotEstimationTable( );
	  virtual vector<double> getEstimationTable() { return estimation_table_;};
   private:
	   int mode;
		double step_;
		CommunicationModelFunction* cmf_;
	   vector<double> estimation_table_;

   };


}

#endif // #ifdef ENABLE_DISTEST
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/distest/neighbor_intersection_distance_estimate.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: neighbor_intersection_distance_estimate.h,v $
 *-----------------------------------------------------------------------*/
