/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <cmath>
#include "apps/localization/modules/distance/localization_euclidean_module.h"
#include "apps/localization/math/localization_triangulation.h"
#include "apps/localization/math/localization_general_math.h"
#include "apps/localization/math/localization_statistic.h"
#include "apps/localization/util/localization_defutils.h"
#include "apps/localization/util/localization_utils.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include <sstream>
#include <iostream>

//#define EUCDEBUG(x) std::cout << "euclidean debug["<< owner().id() <<"]: " << x << std::endl
#define EUCDEBUG(x)

using namespace shawn;

namespace localization
{

   LocalizationEuclideanModule::
   LocalizationEuclideanModule()
      : state_            ( eu_init ),
         cc_std_          ( eu_cc_std_lax ),
         cc_nv_           ( eu_cc_nv_lax ),
         cc_cn_           ( eu_cc_cn_strict ),
         algo_            ( eu_algo_opt ),
         vote_            ( eu_vote_nvcn ),
         last_useful_msg_ ( 0 ),
         col_measure_     ( 0 )
   {}
   // ----------------------------------------------------------------------
   LocalizationEuclideanModule::
   ~LocalizationEuclideanModule()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationEuclideanModule::
   boot( void )
      throw()
   {
      init_parameters();
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationEuclideanModule::
   process_message( const ConstMessageHandle& mh )
      throw()
   {
      ///
      if ( dynamic_cast<const LocalizationEuclideanInitMessage*>( mh.get() ) != NULL )
         return process_euclidean_init_message(
            *dynamic_cast<const LocalizationEuclideanInitMessage*>( mh.get() ));
      ///
      else if ( dynamic_cast<const LocalizationEuclideanNeighborMessage*>( mh.get() ) != NULL )
         return process_euclidean_neighbor_message(
            *dynamic_cast<const LocalizationEuclideanNeighborMessage*>( mh.get() ));
      ///
      else if ( dynamic_cast<const LocalizationEuclideanAnchorMessage*>( mh.get() ) != NULL )
         return process_euclidean_anchor_message(
            *dynamic_cast<const LocalizationEuclideanAnchorMessage*>( mh.get() ));

      return LocalizationModule::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   LocalizationEuclideanModule::
   work( void )
      throw()
   {
      // send initial messages
      if ( state_ == eu_init )
      {
         if ( owner().is_anchor() )
            send( new LocalizationEuclideanInitMessage( LocalizationProcessor::anchor ) );
         else
            send( new LocalizationEuclideanInitMessage( LocalizationProcessor::unknown ) );

         state_ = eu_wait;
      }

      // after idle-time passed, initial messages of neighbors had already been
      // received and state is set to 'broadcast'.
      if ( state_ == eu_wait && simulation_round() - last_useful_msg_ > observer().idle_time() )
         state_ = eu_broadcast;

      // broadcast collected information
      if ( state_ == eu_broadcast )
         broadcast_neighborhood();
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationEuclideanModule::
   finished( void )
      throw()
   {
	   return (state_ == eu_finished);
   }

	void
	LocalizationEuclideanModule::
	rollback( void )
		throw()
	{
			state_ = eu_init;
         last_useful_msg_ = 0;
         col_measure_ = 0;
	}
   // ----------------------------------------------------------------------
   bool
   LocalizationEuclideanModule::
   process_euclidean_init_message( const LocalizationEuclideanInitMessage& leim )
      throw()
   {
      const Node& source = leim.source();
      Vec source_pos = (leim.source().has_est_position())? ( leim.source().est_position()) : ( leim.source().real_position() );
      double distance = estimate_distance( source, node() );
	 // if(distance ==UNKNOWN_DISTANCE)
	//	  return false;

      last_useful_msg_ = simulation_round();

      // add info of received message to own neighborhood
      if ( leim.proc_type() == LocalizationProcessor::anchor )
         neighborhood_w().update_anchor( source, source_pos, distance );
      else
         neighborhood_w().update_neighbor( source, distance );

	  //BugFix: One-hop to anchor
	  if ( neighborhood().valid_anchor_cnt() >= (int)observer().floodlimit() )
		  state_ = eu_finished;

      return true;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationEuclideanModule::
   broadcast_neighborhood( void )
      throw()
   {
      // send info about own neighborhood
      send( new LocalizationEuclideanNeighborMessage( neighborhood().neighbor_distance_map() ) );
      state_ = eu_work;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationEuclideanModule::
   process_euclidean_neighbor_message( const LocalizationEuclideanNeighborMessage& lenm )
      throw()
   {
      // set neighborhood of received node
      neighborhood_w().update_nneighbors( lenm.source(), lenm.neighbors() );

      // if source is valid anchor, send anchor-message

      ConstNeighborhoodIterator it = neighborhood().find( lenm.source() );
       if(it == neighborhood().end_neighborhood())
           std::cout << " Knoten nicht gefunden" << std::cout;
       else
           std::cout << "knoten gefunden" << std::endl;
      if(it->second->is_anchor())
        std::cout << "isAnchor" << std::endl;
      else
          std::cout << "Is noAnchor" << std::endl;
      if(it->second->is_valid())
          std::cout << "isValid" << std::endl;
      else
          std::cout << "is not valid" << std::endl;
      if ( it->second->is_anchor() && it->second->is_valid() )
      {
         send( new LocalizationEuclideanAnchorMessage( it->second->node(), it->second->distance() ) );
      }

      return true;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationEuclideanModule::
   process_euclidean_anchor_message( const LocalizationEuclideanAnchorMessage& leam )
      throw()
   {
     if ( state_ == eu_finished )
         return true;

      const Node& anchor = leam.anchor();
      Vec anchor_pos =
		  ( leam.anchor().has_est_position() )?
		  ( leam.anchor().est_position() ) : ( leam.anchor().real_position() );
      const Node& source = leam.source();

      // if anchor receives message about another anchor, the real distance
      // is calculated and send as new anchor-message
      if ( owner().is_anchor() )
      {
         if ( neighborhood().find( anchor ) == neighborhood().end_neighborhood() )
            neighborhood_w().update_anchor( anchor, anchor_pos );
         else
            return true;

		 double distance = euclidean_distance( anchor_pos, (node().has_est_position())?(node().est_position()):(node().real_position()) );
         send( new LocalizationEuclideanAnchorMessage( anchor, distance ) );

         return true;
      }

      neighborhood_w().update_anchor( anchor, anchor_pos );
      neighborhood_w().update_nneighbor( anchor, source, leam.distance() );

      execute_euclidean( anchor );

      return true;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationEuclideanModule::
   execute_euclidean( const Node& anchor )
      throw()
   {
      NeighborhoodIterator it = neighborhood_w().find_w( anchor );
      //In case that we have not yet estimated a distance to this anchor
	  if ( !it->second->is_valid() )
      {
         double distance = -1;

         switch ( algo_ )
         {
            case eu_algo_normal:
               distance = find_anchor_distance( it->second->node() );
               break;

            case eu_algo_opt:
               distance = find_anchor_distance_opt( it->second->node() );
               break;
         }

         if ( distance == -1 )
		 {
			 EUCDEBUG("Distance to anchor "<< anchor.id() <<" is -1");
			 return;
		 }

         it->second->set_distance( distance );
         send( new LocalizationEuclideanAnchorMessage( it->second->node(), it->second->distance() ) );

         // if floodlimit reached, finished
		 if ( neighborhood().valid_anchor_cnt() >= (int)observer().floodlimit() )
			 state_ = eu_finished;

      }
   }
   // ----------------------------------------------------------------------
   double
   LocalizationEuclideanModule::
   find_anchor_distance( const Node& anchor )
      const throw()
   {
      double distance = -1;
      bool leave = false;

      for ( ConstNeighborhoodIterator it1 = neighborhood().begin_neighborhood(); it1 != neighborhood().end_neighborhood(); ++it1 )
      {
         for ( ConstNeighborhoodIterator it2 = it1; it2 != neighborhood().end_neighborhood(); ++it2 )
         {
            if ( it1 == it2 )
               continue;

			EUCDEBUG("------------------------------------ n1["<< (*(it1->first)).id() <<"], n2["<< (*(it2->first)).id() <<"], anchor["<< anchor.id() <<"]");
			EUCDEBUG("::" << neighborhood().has_valid_neighbor( *it1->first ) << ", " <<
							 neighborhood().has_valid_neighbor( *it2->first ) << ", " <<
							 neighborhood().has_valid_nneighbor( *it1->first, *it2->first ) << ", " <<
							 neighborhood().has_valid_nneighbor( *it1->first, anchor ) << ", " <<
							 neighborhood().has_valid_nneighbor( *it2->first, anchor ));

            // check, that all needed distances exist
            if ( !neighborhood().has_valid_neighbor( *it1->first ) ||
                  !neighborhood().has_valid_neighbor( *it2->first ) ||
                  !neighborhood().has_valid_nneighbor( *it1->first, *it2->first ) ||
                  !neighborhood().has_valid_nneighbor( *it1->first, anchor ) ||
                  !neighborhood().has_valid_nneighbor( *it2->first, anchor ) )
               continue;

            double self_n1 = neighborhood().neighbor_distance( *it1->first );
            double self_n2 = neighborhood().neighbor_distance( *it2->first );
            double n1_n2 = neighborhood().nneighbor_distance( *it1->first, *it2->first );
            double n1_anchor = neighborhood().nneighbor_distance( *it1->first, anchor );
            double n2_anchor = neighborhood().nneighbor_distance( *it2->first, anchor );

            // check collinearity
            switch ( cc_std_ )
            {
               case eu_cc_std_strict:
                  if ( is_collinear( self_n1, self_n2, n1_n2, col_measure_ ) ||
                        is_collinear( n1_anchor, n2_anchor, n1_n2, col_measure_ ) )
                     continue;
                  break;

               case eu_cc_std_lax:
                  if ( is_collinear( self_n1, self_n2, n1_n2, col_measure_ ) &&
                        is_collinear( n1_anchor, n2_anchor, n1_n2, col_measure_ ) )
                     continue;
                  break;

               case eu_cc_std_none:
                  ;
            }

			EUCDEBUG("n1["<< (*(it1->first)).id() <<"], n2["<< (*(it2->first)).id() <<"], anchor["<< anchor.id() <<"]");

            DistancePair dp = trilateration_distance( self_n1, self_n2, n1_n2, n1_anchor, n2_anchor );
            if ( dp.first == -1 )
               continue;

            NodeList nl_nv = find_unique_neighbor_neighbors( anchor, *it1->first, *it2->first );
            NodeList nl_cn = find_common_neighbor_neighbors( anchor, *it1->first, *it2->first );
            double dist_nv = neighbor_vote( anchor, *it1->first, *it2->first, dp, nl_nv );
            double dist_cn = common_neighbor( anchor, *it1->first, *it2->first, dp, nl_cn );

			{
				std::ostringstream oss;
				for(NodeList::iterator lalait = nl_nv.begin(); lalait != nl_nv.end(); ++lalait)
					oss << (**lalait).id() << ", ";
				EUCDEBUG("Neighborlist[nv, "<< nl_nv.size() <<"]: " << oss.str());
			}
			{
				std::ostringstream oss;
				for(NodeList::iterator lalait = nl_cn.begin(); lalait != nl_cn.end(); ++lalait)
					oss << (**lalait).id() << ", ";
				EUCDEBUG("Neighborlist[cn, "<< nl_cn.size() <<"]: " << oss.str());
			}


            if ( dist_nv == -1 && dist_cn == -1 )
               continue;

            switch ( vote_)
            {
               case eu_vote_nv:
               {
                  if ( dist_nv != -1 ) distance = dist_nv;
                  else continue;

                  break;
               }
               case eu_vote_cn:
               {
                  if ( dist_cn != -1 ) distance = dist_cn;
                  else continue;

                  break;
               }
               case eu_vote_nvcn:
               {
                  if ( dist_cn != -1 ) distance = dist_cn;
                  if ( dist_nv != -1 ) distance = dist_nv;
                  else continue;

                  break;
               }
               case eu_vote_cnnv:
               {
                  if ( dist_nv != -1 ) distance = dist_nv;
                  if ( dist_cn != -1 ) distance = dist_cn;
                  else continue;

                  break;
               }
            }// switch vote_

            leave = true;
            break;
         }// for it2

         if ( leave ) break;
      }// for it1

      return distance;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationEuclideanModule::
   find_anchor_distance_opt( const Node& anchor )
      const throw()
   {
      // Following idea is from K. Langendoen and N. Reijers in their
      // implementation of ad-hoc positioning using OMNeT++.
      // See also:
      //    http://www.consensus.tudelft.nl/software.html
      //
      // Take neighbors with fewest collinear measure.

      double distance = -1;
      double best_nv = -1;
      double best_cn = -1;
      double max_measure_nv = std::numeric_limits<double>::min();
      double max_measure_cn = std::numeric_limits<double>::min();

      for ( ConstNeighborhoodIterator it1 = neighborhood().begin_neighborhood(); it1 != neighborhood().end_neighborhood(); ++it1 )
      {
         for ( ConstNeighborhoodIterator it2 = it1; it2 != neighborhood().end_neighborhood(); ++it2 )
         {
            if ( it1 == it2 )
               continue;

            // check, that all needed distance estimations to neighboring nodes exist
            if ( !neighborhood().has_valid_neighbor( *it1->first ) ||
                  !neighborhood().has_valid_neighbor( *it2->first ) ||
                  !neighborhood().has_valid_nneighbor( *it1->first, *it2->first ) ||
                  !neighborhood().has_valid_nneighbor( *it1->first, anchor ) ||
                  !neighborhood().has_valid_nneighbor( *it2->first, anchor ) )
               continue;

            double self_n1 = neighborhood().neighbor_distance( *it1->first );
            double self_n2 = neighborhood().neighbor_distance( *it2->first );
            double n1_anchor = neighborhood().nneighbor_distance( *it1->first, anchor );
            double n2_anchor = neighborhood().nneighbor_distance( *it2->first, anchor );
            double n1_n2 = neighborhood().nneighbor_distance( *it1->first, *it2->first );

			EUCDEBUG("-------");

            // check collinearity
            switch ( cc_std_ )
            {
               case eu_cc_std_strict:
                  if ( is_collinear( self_n1, self_n2, n1_n2, col_measure_ ) ||
                        is_collinear( n1_anchor, n2_anchor, n1_n2, col_measure_ ) )
                     continue;
                  break;

               case eu_cc_std_lax:
                  if ( is_collinear( self_n1, self_n2, n1_n2, col_measure_ ) &&
                        is_collinear( n1_anchor, n2_anchor, n1_n2, col_measure_ ) )
                     continue;
                  break;

               case eu_cc_std_none:
                  ;
            }

			EUCDEBUG("n1["<< (*(it1->first)).id() <<"], n2["<< (*(it2->first)).id() <<"], anchor["<< anchor.id() <<"]");

            DistancePair dp = localization::trilateration_distance( self_n1, self_n2, n1_n2, n1_anchor, n2_anchor );

			if ( dp.first > -1.00000000001 && dp.first < -0.999999999999 )
			{
				EUCDEBUG("trilateration failed "<< anchor.id());
				continue;
			}
/*			std::string filename= "euclideanDist.txt";
			std::ofstream out((char*)(filename.c_str()), std::ios::app);
			double real = (node().real_position() - anchor.real_position()).euclidean_norm();*/
/*			out << " real dist: " << (node().real_position() - anchor.real_position()).euclidean_norm()
				<< " dist1: " << dp.first << " dist2: " << dp.second << std::endl;
		if(( ((dp.first- real) < -0.0000000000001) || ((dp.first- real) > 0.0000000000001 ))
				&&
				( ((dp.second- real) < -0.0000000000001) || ((dp.second- real) > 0.0000000000001 ))
				)
				out << " real dist: " << real
					<< " dist1: " << dp.first << " dist2: " << dp.second << " error: " <<((fabs(dp.first-real)<(fabs(dp.second-real)))?(dp.first-real):(dp.second-real)) <<
				" node id" <<node().id() << " anchor id " << anchor.id() <<
				" self_n1 "<< self_n1<< " self_n2 " <<self_n2 <<" n1_n2 " << n1_n2 <<
				" n1_anchor " << n1_anchor << " n2_anchor " << n2_anchor << std::endl;*/
            double measure;
            NodeList nl = find_common_neighbor_neighbors_opt( anchor, *it1->first, *it2->first, measure );

	/*		std::ostringstream oss;
			for(NodeList::iterator lalait = nl.begin(); lalait != nl.end(); ++lalait)
				oss << (**lalait).id() << ", ";
			EUCDEBUG("Neighborlist: " << oss.str());
*/


            double dist_nv = neighbor_vote( anchor, *it1->first, *it2->first, dp, nl );
            double dist_cn = common_neighbor( anchor, *it1->first, *it2->first, dp, nl );

            if ( dist_cn > -1.000000001 && dist_cn < -0.9999999999 && max_measure_cn < measure && measure > 0 )
            {
               max_measure_cn = measure;
               best_cn = dist_cn;
            }

            if ( dist_nv > -1.000000001 && dist_nv < -0.9999999999 && max_measure_nv < measure && measure > 0 )
            {
               max_measure_nv = measure;
               best_nv = dist_nv;
            }
         }// for it2
      }// for it1

      switch ( vote_)
      {
         case eu_vote_nv:
         {
            if ( best_nv > -1.000000001 && best_nv < -0.9999999999 ) distance = best_nv;
            break;
         }
         case eu_vote_cn:
         {
            if ( best_cn > -1.000000001 && best_cn < -0.9999999999 ) distance = best_cn;
            break;
         }
         case eu_vote_nvcn:
         {
            if ( best_cn > -1.000000001 && best_cn < -0.9999999999 ) distance = best_cn;
            if (best_nv > -1.000000001 && best_nv < -0.9999999999 ) distance = best_nv;
            break;
         }
         case eu_vote_cnnv:
         {
            if ( best_nv > -1.0000000001 && best_nv < -0.9999999999 -1 ) distance = best_nv;
            if ( best_cn > -1.0000000001 && best_cn < -0.9999999999 ) distance = best_cn;
            break;
         }
      }// switch vote_

	  if (distance > -1.00000000001 && distance < -0.999999999999)
	  {
		EUCDEBUG("voting resulted in -1 for anchor "<< anchor.id());
	  }

      return distance;
   }
   // ----------------------------------------------------------------------
   const NodeList
   LocalizationEuclideanModule::
   find_unique_neighbor_neighbors( const Node& anchor, const Node& n1, const Node& n2 )
      const throw()
   {
      NodeList temp;

      for ( ConstNeighborhoodIterator
               it = neighborhood().begin_neighborhood();
               it != neighborhood().end_neighborhood();
               ++it )
      {
         // node is not n1 or n2, and connected to self and anchor
         if ( *it->first == n1 || *it->first == n2 || *it->first == anchor ||
               !neighborhood().has_valid_neighbor( *it->first ) ||
               !neighborhood().has_valid_nneighbor( *it->first, anchor ) )
            continue;

         // node is connected to n1 and n2
         if ( !neighborhood().has_valid_nneighbor( n1, *it->first ) &&
               !neighborhood().has_valid_nneighbor( n2, *it->first ) )
            continue;

         temp.push_back( it->first );
      }

      return temp;
   }
   // ----------------------------------------------------------------------
   const NodeList
   LocalizationEuclideanModule::
   find_common_neighbor_neighbors( const Node& anchor, const Node& n1, const Node& n2 )
      const throw()
   {
      NodeList temp;

      for ( ConstNeighborhoodIterator it = neighborhood().begin_neighborhood(); it != neighborhood().end_neighborhood(); ++it )
      {
         // node is not n1 or n2, and connected to self and anchor
         if ( *it->first == n1 || *it->first == n2 || *it->first == anchor ||
               !neighborhood().has_valid_neighbor( *it->first ) ||
               !neighborhood().has_valid_nneighbor( *it->first, anchor ) )
            continue;

         // node is connected to n1 and n2
         if ( !neighborhood().has_valid_nneighbor( n1, *it->first ) ||
               !neighborhood().has_valid_nneighbor( n2, *it->first ) )
            continue;

         temp.push_back( it->first );
      }

      return temp;
   }
   // ----------------------------------------------------------------------
   const NodeList
   LocalizationEuclideanModule::
   find_common_neighbor_neighbors_opt( const Node& anchor, const Node& n1,
         const Node& n2, double& col_measure )
      const throw()
   {
      NodeList temp;
      col_measure = std::numeric_limits<double>::min();

      for ( ConstNeighborhoodIterator it = neighborhood().begin_neighborhood(); it != neighborhood().end_neighborhood(); ++it )
      {
         // node is not n1 or n2, and connected to self and anchor
         if ( *it->first == n1 || *it->first == n2 || *it->first == anchor ||
               !neighborhood().has_valid_neighbor( *it->first ) ||
               !neighborhood().has_valid_nneighbor( *it->first, anchor ) )
            continue;

         // node is connected to n1 and n2
         if ( !neighborhood().has_valid_nneighbor( n1, *it->first ) ||
               !neighborhood().has_valid_nneighbor( n2, *it->first ) )
            continue;

         double n1_n2 = neighborhood().nneighbor_distance( n1, n2 );
         double n1_n3 = neighborhood().nneighbor_distance( n1, *it->first );
         double n2_n3 = neighborhood().nneighbor_distance( n2, *it->first );

         double tmp_measure = collinear_measure( n1_n2, n1_n3, n2_n3 );

         if ( tmp_measure > col_measure )
         {
            col_measure = tmp_measure;

            temp.clear();
            temp.push_back( it->first );
         }
      }

      return temp;
   }
   // ----------------------------------------------------------------------
   double
	LocalizationEuclideanModule::
	neighbor_vote( const Node& anchor, const Node& n1, const Node& n2, const DistancePair& dp1, const NodeList& nl )
    const throw()
   {
      // Following idea is from K. Langendoen and N. Reijers in their
      // implementation of ad-hoc positioning using OMNeT++.
      // See also:
      //    http://www.consensus.tudelft.nl/software.html
      //
      // The following lines are inspired by above mentioned people and their
      // code.

	   if ( nl.empty() )
	   {
		   EUCDEBUG("List empty (neighbor_vote)");
		   return -1;
	   }

      // temporary workaround. update to 2D/3D should follow soon, so euclidean
      // will exclusively work with NodeLists.
      NodeList nl_refs;
      nl_refs.push_back( &n1 );
      nl_refs.push_back( &n2 );

      LocalizationStatistic stat1, stat2;
      stat1 += dp1.first;
      stat2 += dp1.second;

      for ( NodeList::const_iterator it = nl.begin(); it != nl.end(); ++it )
      {
         const Node& n3 = **it;

         for ( NodeList::const_iterator it_refs = nl_refs.begin(); it_refs != nl_refs.end(); ++it_refs )
         {

            const Node& n1_2 = **it_refs;
            if ( !neighborhood().has_nneighbor( n1_2, n3 ) )
               continue;

            double self_n12 = neighborhood().neighbor_distance( n1_2 );
            double self_n3 = neighborhood().neighbor_distance( n3 );
            double n12_n3 = neighborhood().nneighbor_distance( n1_2, n3 );
            double n12_anchor = neighborhood().nneighbor_distance( n1_2, anchor );
            double n3_anchor = neighborhood().nneighbor_distance( n3, anchor );

            // check collinearity
            switch ( cc_nv_ )
            {
               case eu_cc_nv_strict:
                  if ( is_collinear( self_n12, self_n3, n12_n3, col_measure_ ) ||
                        is_collinear( n12_n3, n12_anchor, n3_anchor, col_measure_ ) )
                     continue;
                  break;

               case eu_cc_nv_lax:
                  if ( is_collinear( self_n12, self_n3, n12_n3, col_measure_ ) &&
                        is_collinear( n12_n3, n12_anchor, n3_anchor, col_measure_ ) )
                     continue;
                  break;

               case eu_cc_nv_none:
                  ;
            }

            DistancePair dp2 = localization::trilateration_distance( self_n12, self_n3, n12_n3, n12_anchor, n3_anchor );
            if ( dp2.first > -1.00000000001 && dp2.first < -0.999999999999)
               continue;

            enum ChooseDist { d11, d12, d21, d22 } ch_dist = d11;
            double dev_sel = fabs( stat1.mean() - dp2.first );

            if ( fabs( stat1.mean() - dp2.second ) < dev_sel )
            {
               dev_sel = fabs( stat1.mean() - dp2.second );
               ch_dist = d12;
            }
            if ( fabs( stat2.mean() - dp2.first ) < dev_sel )
            {
               dev_sel = fabs( stat2.mean() - dp2.first );
               ch_dist = d21;
            }
            if ( fabs( stat2.mean() - dp2.second ) < dev_sel )
            {
               dev_sel = fabs( stat2.mean() - dp2.second );
               ch_dist = d22;
            }

            switch ( ch_dist )
            {
               case d11:
               case d22:
               {
                  stat1 += dp2.first;
                  stat2 += dp2.second;
                  continue;
               }
               case d12:
               case d21:
               {
                  stat1 += dp2.second;
                  stat2 += dp2.first;
                  continue;
               }
            }// switch
         }// for NodeList refs
      }// for NodeList nl

      //double real = euclidean_distance( anchor.real_position(), node().real_position() );
      //DEBUG( owner().logger(),
      //   "Stats: "
      //      << std::endl
      //      << stat1 << ";;; " << stat2
      //      << std::endl
      //      << "Real: " << real << ": "
      //      << std::endl
      //      << stat1.mean() << "; " << stat1.std_dev() << " <= " << stat1.mean()*0.05
      //      << " | "
      //      << stat2.mean() << "; " << stat2.std_dev() << " <= " << stat2.mean()*0.05
		//     << std::endl );

      if ( stat1.std_dev() > stat1.mean()*0.05 && stat2.std_dev() > stat2.mean()*0.05 )
         return -1;

      if ( stat1.std_dev()*3 < stat2.std_dev() )
         return stat1.mean();
      else if ( stat2.std_dev()*3 < stat1.std_dev() )
         return stat2.mean();

      return -1;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationEuclideanModule::
   common_neighbor( const Node& anchor, const Node& n1, const Node& n2,
         const DistancePair& dp, const NodeList& nl )
      const throw()
   {
      // Following idea is from K. Langendoen and N. Reijers in their
      // implementation of ad-hoc positioning using OMNeT++.
      // See also:
      //    http://www.consensus.tudelft.nl/software.html
      //
      // The following lines are inspired by above mentioned people and their
      // code.

      if ( nl.empty() ) return -1;

      for ( NodeList::const_iterator
               it = nl.begin();
               it != nl.end();
               ++it )
      {
         const Node& n3 = **it;

         double self_n1 = neighborhood().neighbor_distance( n1 );
         double self_n2 = neighborhood().neighbor_distance( n2 );
         double self_n3 = neighborhood().neighbor_distance( n3 );
         double n1_n2, n1_anchor, n2_anchor, n3_n1, n3_n2;
         if ( neighborhood().has_valid_nneighbor( n1, n2 ) &&
               neighborhood().has_valid_nneighbor( anchor, n1 ) &&
               neighborhood().has_valid_nneighbor( anchor, n2 ) &&
               neighborhood().has_valid_nneighbor( n3, n1 ) &&
               neighborhood().has_valid_nneighbor( n3, n2 ) )
         {
            n1_n2 = neighborhood().nneighbor_distance( n1, n2 );
            n1_anchor = neighborhood().nneighbor_distance( anchor, n1 );
            n2_anchor = neighborhood().nneighbor_distance( anchor, n2 );
            n3_n1 = neighborhood().nneighbor_distance( n3, n1 );
            n3_n2 = neighborhood().nneighbor_distance( n3, n2 );
         }
         else
            continue;

         // check collinearity
         switch ( cc_cn_ )
         {
            case eu_cc_cn_strict:
               if ( is_collinear( self_n1, self_n2, n1_n2, col_measure_ ) ||
                     is_collinear( n1_n2, n3_n1, n3_n2, col_measure_ ) ||
                     is_collinear( n1_n2, n1_anchor, n2_anchor, col_measure_ ) )
                  continue;
               break;

            case eu_cc_cn_lax:
               if ( is_collinear( self_n1, self_n2, n1_n2, col_measure_ ) &&
                     is_collinear( n1_n2, n3_n1, n3_n2, col_measure_ ) &&
                     is_collinear( n1_n2, n1_anchor, n2_anchor, col_measure_ ) )
                  continue;
               break;

            case eu_cc_cn_none:
               ;
         }

         // distancepair self-n3
         DistancePair dp_sn3 = trilateration_distance(
                                 self_n1, self_n2, n1_n2, n3_n1, n3_n2 );
         if ( dp_sn3.first == -1 )
            continue;

         int side_n3 = 0;
         if ( fabs( self_n3 - dp_sn3.first ) < fabs( self_n3 - dp_sn3.second ) )
            side_n3 = 1;
         else
            side_n3 = -1;

         // distancepair n3-anchor
         DistancePair dp_n3a = localization::trilateration_distance(
                                 n3_n1, n3_n2, n1_n2, n1_anchor, n2_anchor );
         if ( dp_n3a.first == -1 )
            continue;

         int side_a = 0;
         double n3_a = neighborhood().find( n3 )->second->neighbor_distance( anchor );
         if ( fabs( n3_a - dp_n3a.first ) < fabs( n3_a - dp_n3a.second ) )
            side_a = 1;
         else
            side_a = -1;

         double real_dist = euclidean_distance( node().real_position(), anchor.real_position() );
         DEBUG( owner().logger(),
            real_dist << ": " << dp.first << "; " << dp.second
               << " :: " << side_n3 * side_a
               << std::endl
               << self_n3 << ": " << dp_sn3.first << "; " << dp_sn3.second
               << std::endl
               << n3_a    << ": " << dp_n3a.first << "; " << dp_n3a.second
               << std::endl );

         if ( side_n3 == 1 && side_a == 1 )
            return dp.first;
         else if ( side_n3 * side_a == -1 || ( side_n3 == -1 && side_a == -1 ) )
            return dp.second;
      }// for

      return -1;
   }
   // ----------------------------------------------------------------------
   void
   LocalizationEuclideanModule::
   init_parameters( void )
      throw()
   {
      const SimulationEnvironment& se = node().world().simulation_controller().environment();

      cc_std_ = EuclideanCollinearCheckStd( get_index(
         se.optional_string_param( "loc_eucl_col_check_std", "" ),
         EUCL_COL_CHECK_STD,
         sizeof( EUCL_COL_CHECK_STD ) / sizeof( EUCL_COL_CHECK_STD[0] ),
         int( cc_std_ ) ) );
      cc_nv_ = EuclideanCollinearCheckNV( get_index(
         se.optional_string_param( "loc_eucl_col_check_nv", "" ),
         EUCL_COL_CHECK_NV,
         sizeof( EUCL_COL_CHECK_NV ) / sizeof( EUCL_COL_CHECK_NV[0] ),
         int( cc_nv_ ) ) );
      cc_cn_ = EuclideanCollinearCheckCN( get_index(
         se.optional_string_param( "loc_eucl_col_check_cn", "" ),
         EUCL_COL_CHECK_CN,
         sizeof( EUCL_COL_CHECK_CN ) / sizeof( EUCL_COL_CHECK_CN[0] ),
         int( cc_cn_ ) ) );
      algo_ = EuclideanAlgo( get_index(
         se.optional_string_param( "loc_eucl_algo", "" ),
         EUCL_ALGO,
         sizeof( EUCL_ALGO ) / sizeof( EUCL_ALGO[0] ),
         int( algo_ ) ) );
      vote_ = EuclideanVote( get_index(
         se.optional_string_param( "loc_eucl_vote", "" ),
         EUCL_VOTE,
         sizeof( EUCL_VOTE ) / sizeof( EUCL_VOTE[0] ),
         int( vote_ ) ) );

      col_measure_ = se.optional_double_param( "loc_eucl_col_measure", 0.1 );
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/distance/localization_euclidean_module.cpp,v $
 * Version $Revision: 622 $
 * Date    $Date: 2011-07-19 17:46:58 +0200 (Tue, 19 Jul 2011) $
 *-----------------------------------------------------------------------
 * $Log: localization_euclidean_module.cpp,v $
 *-----------------------------------------------------------------------*/
