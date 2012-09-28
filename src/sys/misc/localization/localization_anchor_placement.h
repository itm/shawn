/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_LOCALIZATION_ANCHOR_PLACEMENT_H
#define __SHAWN_SYS_MISC_LOCALIZATION_ANCHOR_PLACEMENT_H


#include "sys/simulation/simulation_task.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node.h"


namespace shawn
{


   /// Anchor placement on the basis of geometric figures.
   /** This task works on an already created topology. One can select between
    *  different figures and variable count of anchors. If there are some
    *  some anchors left, which don't fit in the figure anymore, they are
    *  placed randomly.
    *
    *  Every given point in the figure is passed and the nearest node, which
    *  is not already one, is set to an anchor. For this reason a processor,
    *  which wants to become an anchor, has to implement
    *  LocalizationAbstractProcessor.
    */
   class SimulationTaskLocalizationAnchorPlacement
      : public SimulationTask
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      SimulationTaskLocalizationAnchorPlacement();
      ///
      virtual ~SimulationTaskLocalizationAnchorPlacement();
      ///@}

      ///@name basic methods inherited from SimulationTask
      ///@{
      /**
       * Basic method which is called when task is started
       * \param sc the SimulationController to access the whole simulation
       * \sa SimulationTask::run()
       */
      virtual void run( SimulationController& sc) throw( std::runtime_error );
      /** \return used name to be indentified by SimulationTaskKeeper
       *  \sa SimulationTask::name()
       */
      virtual std::string name( void ) const throw();
      /** \return short description about functionality
       *  \sa SimulationTask::description()
       */
      virtual std::string description( void ) const throw();
      ///@}

      ///@name information about done placement
      ///@{
      /** \return number of placed anchors
       */
      virtual int anchor_cnt( void ) const throw();
      /** \return used placement method
       */
      virtual const std::string& pl_type( void ) const throw();
      ///@}


   protected:

      ///@name placement methods
      ///@{
      /** Anchors placed on a \c n*n sized grid with maximal number \c n that
       *  satifies \f$ n*n \le \#anchors \f$.
       *
       *  Borders of the grid are the borders of topology, so that nodes are
       *  surrounded by anchors.
       *
       *  \param sc SimulationController to access the nodes in the topology
       *  \param anchor_cnt wanted number of placed anchors
       *  \return number of unplaced anchors
       */
      virtual int outer_grid( SimulationController& sc, int anchor_cnt ) throw();
      /** Anchors placed on a grid just like in outer_grid(), but borders are
       *  \em not the same as ones of topology. Grid is set inside the
       *  topology, so that nodes near the borders are \em not surrounded by
       *  anchors.
       *
       *  \param sc SimulationController to access the nodes in the topology
       *  \param anchor_cnt wanted number of placed anchors
       *  \return number of unplaced anchors
       *  \sa outer_grid()
       */
      virtual int inner_grid( SimulationController& sc, int anchor_cnt ) throw();
      /** Nodes are set randomly to anchors. Random in this case relates to a
       *  randomly generated topology, because this method sets the first \c n
       *  nodes, that are not already one, to an anchor.
       *
       *  \param sc SimulationController to access the nodes in the topology
       *  \param anchor_cnt number of placed anchors
       */
      virtual void random_set( SimulationController& sc, int anchor_cnt ) throw();
      /** This method passes all nodes in the topology and sets the one, which
       *  is nearest to given position, to an anchor.
       *
       *  \param sc SimulationController to access the nodes in the topology
       *  \param pos position to which nearest node is set to an anchor
       */
      virtual void set_nearest_anchor( SimulationController& sc, const Vec& pos )
         throw();
      ///@}

		virtual int tag(SimulationController& sc) throw();
		/** This method passes all nodes in the topology and sets those with an 
		*	 "Anchor-Tag" to an anchor. 
		* 
		* \param SimulationController allows access to nodes in topology
		* \return Number of placed anchors
		*/

   private:

      int anchor_cnt_;
      std::string pl_type_;

   };


}

#endif

