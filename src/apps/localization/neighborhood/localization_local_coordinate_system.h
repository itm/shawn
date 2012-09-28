/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_NEIGHBORHOOD_LOCAL_COORDINATE_SYSTEM_H
#define __SHAWN_APPS_LOCALIZATION_NEIGHBORHOOD_LOCAL_COORDINATE_SYSTEM_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/node.h"
#include "sys/vec.h"
#include "apps/localization/util/localization_defutils.h"
#include "apps/localization/neighborhood/localization_neighborhood.h"


namespace localization
{

   /// Local Coordinate System
   /** Class representing a single Local Coordinate System
    *  This class represents a single LCS such as described in the paper by
    *  Capkun, Hamdi and Hubaux, <em>GPS-free positioning in mobile ad-hoc
    *  networks</em>.
    *
    *
    */
   class LocalizationLocalCoordinateSystem
   {

   public:

      /// Container for data of correction of LCS
      struct CorrectionData
      {
         double correction_angle;
         bool mirror;
         shawn::Vec pos;
      };

      ///@name construction / destruction
      ///@{
      ///
      LocalizationLocalCoordinateSystem();
      ///
      LocalizationLocalCoordinateSystem( const LocalizationLocalCoordinateSystem& );
      ///
      virtual ~LocalizationLocalCoordinateSystem();
      ///@}

      ///@name nodes
      ///@{
      /**
       */
      virtual void update_basic_nodes( const shawn::Node&, const shawn::Node&, const LocalizationNeighborhood& ) throw();
      /** Add a node to the current LCS, if there can be a position computed.
       */
      virtual void update_node( const shawn::Node& ) throw();
      /** Set src position and adapt all positions of neighbors in LCS.
       */
      virtual void set_src_node( const shawn::Node& ) throw();
      /** Set src position and adapt all positions of neighbors in LCS.
       */
      virtual void set_position( const shawn::Vec& ) throw();
      ///@}

      ///@name information
      ///@{
      /** \result Used epsilon
       */
      virtual double epsilon( void ) const throw();
      /** Set epsilon
       */
      virtual void set_epsilon( double ) throw();
      /** \result /c true, if LCS is valid.
       */
      virtual bool is_valid( void ) const throw();
      /** \return number of nodes of current LCS (include the basic nodes p
       *    and q)
       */
      virtual int size( void ) const throw();
      /** \result Src-Node.
       */
      virtual const shawn::Node& src_node( void ) const throw();
      /** \param Node neighbor to search for
       *
       *  \result \true if neighbor exists
       */
      virtual bool has_neighbor( const shawn::Node& ) const throw();
      /** \result Position of given node.
       */
      virtual const shawn::Vec& node_position( const shawn::Node& ) const throw();
      /** \result Position of src node.
       */
      virtual const shawn::Vec& src_position( void ) const throw();
      ///@}

      ///@name special methods on LCS
      ///@{
      /** Perform the correction of LCS based on given CorrectionData.
       */
      virtual void perform_correction( const CorrectionData& ) throw();
      /** Rotate all coordinates by given angle.
       */
      virtual void rotate( double ) throw();
      /** Mirror coordinate system on x-axis.
       */
      virtual void mirror_x( void ) throw();
      /** Mirror coordinate system on y-axis.
       */
      virtual void mirror_y( void ) throw();
      /** Correct LCS in respect to the given one. At first it is turned by a
       *  computed correction angle and then, if necessary, mirrored by x-
       *  and/or y-axis.
       *
       *  \param LocalizationLocalCoordinateSystem LCS
       *  \return \c true, if computation succeeds.
       */
      virtual bool correct_lcs( const LocalizationLocalCoordinateSystem&, CorrectionData& ) const throw();
      /** Correct LCS to the direction of the real global coordinate system.
       *  Therefor the real positions of self and the basic nodes p and q are
       *  taken.
       *
       *  \return \c true, if computation succeeds.
       */
      virtual bool correct_lcs_to_real_ncs( CorrectionData& ) const throw();
      /** Adapt own location and coordinates of
       */
      virtual void adapt_to_real_positions( void ) throw();
      ///@}

      ///@name internal methods
      ///@{
      /** Delete all internal data.
       */
      virtual void clear( void ) throw();
      ///@}

      ///@name write out
      ///@{
      /** Print out members of LCS.
       */
      friend std::ostream& operator<<( std::ostream&, const LocalizationLocalCoordinateSystem& ) throw();
      ///@}


   private:

      const shawn::Node *p_, *q_, *src_node_;
      const LocalizationNeighborhood* neighborhood_;
      shawn::Vec src_pos_;

      localization::LocationMap locations_;

      bool valid_;

      double epsilon_;
   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/neighborhood/localization_local_coordinate_system.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_local_coordinate_system.h,v $
 *-----------------------------------------------------------------------*/
