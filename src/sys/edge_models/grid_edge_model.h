/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_EDGE_MODELS_GRID_EDGE_MODEL_H
#define __SHAWN_SYS_EDGE_MODELS_GRID_EDGE_MODEL_H


#include <cstdlib>
#include <cassert>
#include <cmath>
#include <set>
#include <iostream>
#include <limits>
#include <stdexcept>
#include "sys/misc/box.h"

#include "sys/edge_model.h"
#include "sys/node.h"


namespace shawn
{
   // 16 is 65k cells 
#define HUGE_GRID_LOG_SIZE 16


   // CellData is [y][x]-indexed. Is y_size() times x_size() large
   // x_size() is 2^log_x_size_, y_size() is 2^log_y_size_
   // Rectangular Block  valid_y_begin_..(valid_y_end_-1) X valid_x_begin_..(valid_x_end_-1) contains valid CellData pointers, rest is unitialized memory

   // x_origin_cell_ and y_origin_cell_ give the cell numbers to which virtual_cell=0 (in node lookup) refers

   // cell_width_ and cell_height_ are self-explanatory

   // virtual_x0_ and virtual_y0_ refer to world-coordinates of the virtual_cell 0












   /// EdgeModel based on a geometric grid
   /** The GridEdgeModel uses a geometric grid for fast lookup
    *  of node positions. It is much faster than LazyEdgeModel,
    *  and it supports node mobility.
    *
    *  Configuration parameters are:
    *
    *  grid_size  specifies roughly the initial size of the
    *  grid: it will consist of grid_size x grid_size cells.
    *
    */
   class GridEdgeModel
      : public EdgeModel
   {
   public:
      
      ///@name environment parameter names
      ///@{
      ///
      static const std::string INITIAL_GRID_SIZE_PARAMETER;
      ///
      static const std::string INITIAL_CELL_SIZE_PARAMETER;
      ///
      static const std::string CLOSENESS_FRACTION_PARAMETER;
      ///@}
      

      ///@name types & constants
      ///@{
#ifndef NDEBUG
      /** only exists in debugging versions. magic value to mark unused
       *  node_cells_[id].x so it can be detected if something screws up
       *  the correct order of add_node()/update_zone(). */
      static const int MAGIC_UNITIALIZED_NODE_CELL_X_;
#endif
      ///
      typedef std::set<Node*> NodeSet;
      ///
      typedef NodeSet::iterator node_set_iterator;
      ///
      typedef NodeSet::const_iterator const_node_set_iterator;
      ///
      template<typename NodeType,
               typename NodeSetIterator>
      class GridIteratorHelper
         : public AbstractIteratorHelper<NodeType>
      {
      public:
         typedef AbstractIteratorHelper<NodeType> base_type;
         GridIteratorHelper( const GridEdgeModel&,
							 EdgeModel::CommunicationDirection dir,
                             NodeType&,
                             int x_begin, int x_end,
                             int y_begin, int y_end );
         GridIteratorHelper( const GridIteratorHelper& );
         virtual ~GridIteratorHelper();
         virtual void init( void ) throw();
         virtual void next( void ) throw();
         virtual NodeType* current( void ) const throw();
         virtual AbstractIteratorHelper<NodeType>* clone( void ) const throw();
         void advance_if_infeasible( void ) throw();
      private:
         const GridEdgeModel&      edge_model_;
         NodeType&                 node_;
         int x_begin_;
         int x_end_;
         int y_end_;
         int x_;
         int y_;
         NodeSetIterator it_;
         NodeSetIterator end_it_;
      };
      ///
      struct NodeCell
      {
         int x,y;
      };
      ///
      friend class CellData;
      ///
      class CellData
      {
         friend class GridEdgeModel;
      public:
         typedef GridEdgeModel::NodeSet NodeSet;
         typedef GridEdgeModel::node_set_iterator node_set_iterator;
         typedef GridEdgeModel::const_node_set_iterator const_node_set_iterator;
         CellData( GridEdgeModel& g, int relx, int rely )
            : zone_ ( Vec( g.virtual_x0_ + (double(relx)*g.cell_width_),
                           g.virtual_y0_ + (double(rely)*g.cell_height_),
                           -std::numeric_limits<double>::max() ),
                      Vec( g.virtual_x0_ + (double(relx+1)*g.cell_width_),
                           g.virtual_y0_ + (double(rely+1)*g.cell_height_),
                           std::numeric_limits<double>::max() ) )
         {}
         CellData( const CellData& cd )
         { abort(); }
         ~CellData()
         {}
         inline void add( Node& v ) throw()
         { nodes_.insert(&v); }
         inline void remove( Node& v ) throw()
         { nodes_.erase(&v); }
         inline const Box& zone( void ) const throw()
         { return zone_; }
         inline bool contains( const Node& v ) throw()
         { return nodes_.find(const_cast<Node*>(&v)) != nodes_.end(); }
         inline const_node_set_iterator begin(void) const throw()
         { return nodes_.begin(); }
         inline const_node_set_iterator end(void) const throw()
         { return nodes_.end(); }
      private:
         Box zone_;
         NodeSet nodes_;
      };
      ///@}









      ///@name construction / destruction
      ///@{
      ///
      GridEdgeModel();
      ///
      virtual ~GridEdgeModel();
      ///@}


      ///@name EdgeModel interface
      ///@{
      ///
      virtual int
      nof_adjacent_nodes( const Node&, CommunicationDirection d = CD_BIDI ) 
         const throw();
      ///
      virtual const_adjacency_iterator
      begin_adjacent_nodes( const Node&, CommunicationDirection d = CD_BIDI )
         const throw();
      ///
      virtual const_adjacency_iterator
      end_adjacent_nodes( const Node& )
         const throw();
      ///
      virtual adjacency_iterator
		  begin_adjacent_nodes_w( Node&, CommunicationDirection d = CD_BIDI )
         throw();
      ///
      virtual adjacency_iterator
      end_adjacent_nodes_w( Node& )
         throw();
      ///@}

      ///@name Mobility interfaces
      ///@{

      ///
      virtual Box observer_initial_zone(Node&, const Vec& pos, const Vec& velo ) 
         throw();
      ///
      virtual Box observer_update_zone(Node&, const Vec& newpos, const Vec& velo ) throw();
      ///
      virtual bool supports_mobility( void ) const throw();

      ///@}

      virtual void node_added( Node& ) throw();
      virtual void node_removed( Node& ) throw();



      ///@name User configuration
      ///@{
      /** Sets the initial size of the grid to \a s x \a s . Must be
       *  called before the first node gets added.
       *
       *  \a s is rounded up to the next power of 2.
       *
       *  Controlled via env parameter grid_size
       */
      void set_initial_size( int s ) throw( std::runtime_error );
      /** Sets the edge length of each grid cell to
       *  \a tm x CommunicationModel::communication_upper_bound().
       *  Must be called before the first node gets added.
       * 
       *  Argument must be greater than one.
       *
       *  Controlled via env parameter grid_cell_size
       */
      void set_transmission_scale( double tm ) throw( std::runtime_error );
      ///
      void set_closeness_fraction( double cf ) throw( std::runtime_error );
      ///@}

   private:
      
   ///@name TODO
   ///@{
   ///
      void delayed_init( const Node&, const Vec& pos ) throw();
      ///@}































      ///@name Access to cell array structure
      ///@{
      /** Central array to hold all allocated cells. Array is of type
       *  (CellData*)[y_size()][x_size()].
       *
       *  The array is resized and realloced by expand_find_point().
       *
       *  \warning cell_data_ contains a rectangle of initialized cells.
       *  The remainder is uninitialized pointers (ie, possibly not NULL!)
       *  Expanding this allocated rectangle is done by expand_find_point().
       *
       *  \see expand_find_point(), is_initialized_cell() */
      CellData*** cell_data_;
      /** 2-log of the X dimension of cell_data_
       *  \see cell_data_, x_size()
       */
      int log_x_size_;
      /** 2-log of the Y dimension of cell_data_
       *  \see cell_data_, y_size()
       */
      int log_y_size_;
      /** Size in X direction of the cell array.
       *  \see cell_data_ */
      inline int x_size( void ) const throw()
      { return 1<<log_x_size_; }
      /** Size in Y direction of the cell array.
       *  \see cell_data_ */
      inline int y_size( void ) const throw()
      { return 1<<log_y_size_; }
      /** \return the writable cell contents of cell (x,y).
       *  (x,y) are absolute indices within cell_data_, that is,
       *  0<=\a x<x_size() and 0<=\a y<y_size().
       *
       *  The origin cell is completely independent of this.
       *
       *  \warning the cell must already be allocated and initialized.
       *  \see cell_data_, is_initialized_cell()
       */
      inline CellData& cell_data( int x, int y ) throw()
      {
         assert( (x>=valid_x_begin_) && (x<valid_x_end_) );
         assert( (y>=valid_y_begin_) && (y<valid_y_end_) );
         return *(cell_data_[y][x]);
      }
      /** \return the const cell contents of cell (x,y).
       *  (x,y) are absolute indices within cell_data_, that is,
       *  0<=\a x<x_size() and 0<=\a y<y_size().
       *
       *  The origin cell is completely independent of this.
       *
       *  \warning the cell must already be allocated and initialized.
       *  \see cell_data_, is_initialized_cell()
       */
      inline const CellData& cell_data( int x, int y ) const throw()
      {
         assert( (x>=valid_x_begin_) && (x<valid_x_end_) );
         assert( (y>=valid_y_begin_) && (y<valid_y_end_) );
         return *(cell_data_[y][x]);
      }
      ///@}


      ///@name CONTAINED INITIALIZED BLOCK
      ///@{
      int valid_x_begin_;
      int valid_x_end_;
      int valid_y_begin_;
      int valid_y_end_;

      inline bool is_initialized_cell( int x, int y ) const throw()
      { 
         return
            (x>=valid_x_begin_) &&
            (x< valid_x_end_) &&
            (y>=valid_y_begin_) &&
            (y<valid_y_end_);
      }
      ///@}


      ///@name Cell structure modification
      ///@{
      ///
      CellData& expand_find_point( const Vec& pos, int& cx, int& cy ) throw();
      void expand_realloc( int logx,
                           int logy,
                           int xold,
                           int yold )
         throw();
      inline CellData** alloc_plain_row( int logx ) throw()
      { return new CellData*[ 1<<logx ]; }
      /** destroys row ypos in cell_data_, builds
       *  a replacement one that is 2^newlogx wide and
       *  contains the old row at xold
       */
      CellData** realloced_row( int ypos,
                                int newlogx,
                                int xold ) throw();

      void alloc_cell_block( int y_begin, int y_end,
                             int x_begin, int x_end )
         throw();
      void alloc_cell_row( int y_pos,
                           int x_begin, int x_end )
         throw();
      ///@}






      ///@name Misc. Member variables
      ///@{
      ///
      bool initialized_;
      /** \see set_transmission_scale() */
      double transm_scale_;
      /** Stored copy of CommunicationModel::communication_upper_bound() */
      double trans_range_;
      /** \see set_closeness_fraction() */
      double closeness_distance_;
      /** \see set_closeness_fraction() */
      double closeness_fraction_;
      ///@}

   
















      /** NODE IDS TO CELL POSITIONS **/
      NodeCell* node_cells_;
      int     node_cells_size_;
      int     node_valid_end_;
      void init_node_cells( int id ) throw();
      NodeCell& node_cell_new_w( int id ) throw();
      inline void set_node_cell_absolute( NodeCell& nc,
                                          int x, int y ) throw()
      {
         nc.x = x - x_origin_cell_;
         nc.y = y - y_origin_cell_;
      }
      inline const NodeCell& node_cell_old( int id ) const throw()
      {
         assert( id>=0 );
         assert( id < node_valid_end_ );
#ifndef NDEBUG
         assert( /* if this fails, add_node() resp. obtain_initial_zone() was forgotten */
                node_cells_[id].x != MAGIC_UNITIALIZED_NODE_CELL_X_ );
#endif
         return node_cells_[id];
      }
      inline NodeCell& node_cell_old_w( int id ) throw()
      {
         assert( id>=0 );
         assert( id < node_valid_end_ );
#ifndef NDEBUG
         assert( /* if this fails, add_node() resp. obtain_initial_zone() was forgotten */
                  node_cells_[id].x != MAGIC_UNITIALIZED_NODE_CELL_X_ );
#endif
         return node_cells_[id];
      }







      /** WORLD COORDINATES vs CELL IDS **/
      double virtual_x0_;
      double virtual_y0_;
      double cell_width_;
      double cell_height_;
      int    x_origin_cell_;
      int    y_origin_cell_;

      inline double x0( void ) const throw()
      { return virtual_x0_ - cell_width_*double(x_origin_cell_); }
      inline double y0( void ) const throw()
      { return virtual_y0_ - cell_height_*double(y_origin_cell_); }
      inline int cell_x( double xpos ) const throw()
      { return int( floor( (xpos-x0())/cell_width_) ); } // TODO-X0
      inline int cell_y( double ypos ) const throw()
      { return int( floor((ypos-y0())/cell_height_) ); } // TODO-X0
   
      inline int clipped_cell_x( double xpos ) const throw()
      {
         int c=cell_x(xpos);
         return
            c < valid_x_begin_
            ? valid_x_begin_
            : c >= valid_x_end_ ? valid_x_end_-1
            : c;
      }
      inline int clipped_cell_y( double ypos ) const throw()
      {
         int c=cell_y(ypos);
         return
            c < valid_y_begin_
            ? valid_y_begin_
            : c >= valid_y_end_ ? valid_y_end_-1
            : c;
      }







      /** UTILITY FUNCTIONS **/
      /** \returns smallest integer c such that (1<<c)>=x */
      inline int ceil_log( int x ) throw()
      { return ceil_log(double(x)); }//todo: bit-wise (ceil_log(8)=4 cause of rounding!)
      /** \returns smallest integer c such that (1<<c)>=x */
      inline int ceil_log( double x ) throw()
      { return int(ceil( (log(x)/log(2.0))+.0001 )); }

      Box pad_if_close( const Vec&, const Box& ) const throw();



#ifndef NDEBUG
      void debug_dump( void );
#endif
#ifndef NDEBUG
      void assert_integrity( void ) const throw();
#endif

   };





}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/grid_edge_model.h,v $
 * Version $Revision: 182 $
 * Date    $Date: 2008-04-07 15:14:38 +0200 (Mon, 07 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: grid_edge_model.h,v $
 *-----------------------------------------------------------------------*/
