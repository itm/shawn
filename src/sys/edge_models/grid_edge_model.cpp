/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/edge_models/grid_edge_model.h"
#include "sys/world.h"
#include "sys/communication_model.h"
#include <iomanip>

using namespace std;

namespace shawn
{

#ifndef NDEBUG
   const int         GridEdgeModel::MAGIC_UNITIALIZED_NODE_CELL_X_ = std::numeric_limits<int>::max();
#endif

   const std::string GridEdgeModel::INITIAL_GRID_SIZE_PARAMETER("grid_size");
   const std::string GridEdgeModel::INITIAL_CELL_SIZE_PARAMETER("grid_cell_size");
   const std::string GridEdgeModel::CLOSENESS_FRACTION_PARAMETER("grid_closeness_fraction");
   // ----------------------------------------------------------------------
   template<typename NodeType, typename NodeSetIterator>
   shawn::GridEdgeModel::GridIteratorHelper<NodeType, NodeSetIterator>::
   GridIteratorHelper( const GridEdgeModel& em,
							 EdgeModel::CommunicationDirection dir,
                             NodeType& n,
                             int x_begin, int x_end,
                             int y_begin, int y_end )
      : AbstractIteratorHelper<NodeType>(dir),
	    edge_model_   ( em ),
        node_         ( n ),
        x_begin_      ( x_begin ),
        x_end_        ( x_end ),
        y_end_        ( y_end ),
        x_            ( x_begin ),
        y_            ( y_begin ),
        it_           ( em.cell_data(x_,y_).begin() ),
        end_it_       ( em.cell_data(x_,y_).end() )
   {}
   // ----------------------------------------------------------------------
   template<typename NodeType, typename NodeSetIterator>
   shawn::GridEdgeModel::GridIteratorHelper<NodeType, NodeSetIterator>::
   GridIteratorHelper( const GridIteratorHelper& o )
      : AbstractIteratorHelper<NodeType>(o.direction_),
	    edge_model_   ( o.edge_model_   ),
        node_         ( o.node_         ),
        x_begin_      ( o.x_begin_      ),
        x_end_        ( o.x_end_        ),
        y_end_        ( o.y_end_        ),
        x_            ( o.x_            ),
        y_            ( o.y_            ),
        it_           ( o.it_           ),
        end_it_       ( o.end_it_       )
   {}
   // ----------------------------------------------------------------------
   template<typename NodeType,
            typename NodeSetIterator>
		shawn::GridEdgeModel::GridIteratorHelper<NodeType,NodeSetIterator>::
   ~GridIteratorHelper()
   {}
   // ----------------------------------------------------------------------
   template<typename NodeType,
            typename NodeSetIterator>
   void
   shawn::GridEdgeModel::GridIteratorHelper<NodeType,NodeSetIterator>::
   advance_if_infeasible( void )
      throw()
   {
      while( it_ == end_it_ )
         {
            if( x_ < x_end_ )
               ++x_;

            if( x_ == x_end_ )
               {
                  if( y_>=y_end_-1 )
                     return;
                  
                  x_=x_begin_;
                  ++y_;
               }
            
            it_=edge_model_.cell_data(x_,y_).begin();
            end_it_=edge_model_.cell_data(x_,y_).end();
         }
   }
   // ----------------------------------------------------------------------
   template<typename NodeType,
            typename NodeSetIterator>
   void
   shawn::GridEdgeModel::GridIteratorHelper<NodeType,NodeSetIterator>::
   init( void )
      throw()
   {
		advance_if_infeasible();
		while (it_ != end_it_) 
		{
			
			if (edge_model_.are_adjacent(node_, **it_, base_type::direction_)) 
				break;
			++it_;
			advance_if_infeasible();
		}
   }

   // ----------------------------------------------------------------------
	template<typename NodeType, typename NodeSetIterator>
	void
	   shawn::GridEdgeModel::GridIteratorHelper<NodeType,NodeSetIterator>::
	   next( void )
	   throw()
   {
		if( it_ != end_it_ )
		{ 
			++it_; 
			init(); 
		}
   }
   // ----------------------------------------------------------------------
	template<typename NodeType, typename NodeSetIterator>
		NodeType*
		shawn::GridEdgeModel::GridIteratorHelper<NodeType,NodeSetIterator>::
		current( void )
		const throw()
   {
      if( it_ == end_it_ )
         return NULL;
      else
         return *it_;
   }
   // ----------------------------------------------------------------------
   template<typename NodeType,
            typename NodeSetIterator>
		shawn::EdgeModel::AbstractIteratorHelper<NodeType>*
   shawn::GridEdgeModel::GridIteratorHelper<NodeType,NodeSetIterator>::
   clone( void )
      const throw()
   {
      return new
         GridEdgeModel::GridIteratorHelper
         <NodeType,NodeSetIterator>( *this );
   }
   // ----------------------------------------------------------------------
	template class GridEdgeModel::GridIteratorHelper<Node,GridEdgeModel::const_node_set_iterator>;
   template class GridEdgeModel::GridIteratorHelper<const Node,GridEdgeModel::const_node_set_iterator>;









   GridEdgeModel::
   GridEdgeModel()
      : log_x_size_   ( 3 ),
        log_y_size_   ( 3 ),
        initialized_  ( false ),
        transm_scale_ ( 1.5 )
   {
      //cout << __FILE__<<__LINE__<<" init all vars!" <<endl;
   }
   // ----------------------------------------------------------------------
   GridEdgeModel::
   ~GridEdgeModel()
   {
      delete[] node_cells_;	
	  
	  for (int j = 0; j < y_size(); j++){
			for (int i = 0; i < x_size(); i++){
			  if (is_initialized_cell(i,j)) delete cell_data_[j][i];
		  }
		  delete[] cell_data_[j];
	  }
	  delete[] cell_data_;
   }

















   // ----------------------------------------------------------------------
   Box
   GridEdgeModel::
   observer_initial_zone(Node& v, const Vec& pos, const Vec& velo ) 
      throw()
   {
      if( !initialized_ )
         delayed_init(v,pos);

      int id = v.id();
      int cx,cy;
      CellData& cd = expand_find_point(pos,cx,cy);
      NodeCell& nc = node_cell_new_w(id);

#ifndef NDEBUG
//       cout << "ADDING TO CELL " << cx << " " << cy << endl
//            << "  (FEAS " << valid_x_begin_ << ".." << valid_x_end_
//            << " / " << valid_y_begin_ << ".." << valid_y_end_ << endl
//            << "  (CD AT " << (void*)(&cd) << endl;
#endif
      set_node_cell_absolute(nc,cx,cy);
      cd.add(v);

#if 0
      assert_integrity();
      debug_dump();
#endif

      Box b= pad_if_close( pos, cd.zone() );
      //cout << "RETURNING BOX " << b.lower() << " " << b.upper() << endl;
      return b;
      //return pad_if_close( pos, cd.zone() );
   }
   // ----------------------------------------------------------------------
   Box
   GridEdgeModel::
   pad_if_close( const Vec& pos, const Box& box )
      const throw()
   {
      bool pad = false;
      double xhook = pos.x();
      double yhook = pos.y();

      if( fabs(pos.x()-box.lower().x())<closeness_distance_/2.0 )
         {
            pad=true;
            xhook = box.lower().x();
         }
      else if( fabs(pos.x()-box.upper().x())<closeness_distance_/2.0 )
         {
            pad=true;
            xhook = box.upper().x();
         }

      if( fabs(pos.y()-box.lower().y())<closeness_distance_/2.0 )
         {
            pad=true;
            yhook = box.lower().y();
         }
      else if( fabs(pos.y()-box.upper().y())<closeness_distance_/2.0 )
         {
            pad=true;
            yhook = box.upper().y();
         }
//       cout << "PAD " << (pad?'Y':'N') << " xhook=" << xhook << " yhook=" << yhook
//            << " CDIST=" << closeness_distance_ << endl;

      if( pad )
         return Box( Vec( xhook-closeness_distance_,
                          yhook-closeness_distance_,
                          -std::numeric_limits<double>::max() ),
                     Vec( xhook+closeness_distance_,
                          yhook+closeness_distance_,
                          std::numeric_limits<double>::max() ) );
      else
         return box;
   }
   // ----------------------------------------------------------------------
   int
   GridEdgeModel::
   nof_adjacent_nodes( const Node& v, CommunicationDirection d )
      const throw()
   {
      int res=0;
      for( const_adjacency_iterator
              it    = begin_adjacent_nodes(v, d),
              endit = end_adjacent_nodes(v);
           it != endit;
           ++it )
         ++res;
      return res;
   }
   // ----------------------------------------------------------------------
   EdgeModel::const_adjacency_iterator
   GridEdgeModel::
   begin_adjacent_nodes( const Node& v, CommunicationDirection d )
      const throw()
   {
      const Vec& pos = v.real_position();
      return new GridIteratorHelper<const Node,const_node_set_iterator>
         ( *this, d, v, 
           clipped_cell_x( pos.x()-trans_range_-closeness_distance_-.0001 ),
           clipped_cell_x( pos.x()+trans_range_+closeness_distance_+.0001 )+1,
           clipped_cell_y( pos.y()-trans_range_-closeness_distance_-.0001 ),
           clipped_cell_y( pos.y()+trans_range_+closeness_distance_+.0001 )+1 );
   }
   // ----------------------------------------------------------------------
   EdgeModel::const_adjacency_iterator
   GridEdgeModel::
   end_adjacent_nodes( const Node& )
      const throw()
   {
      return EdgeModel::const_adjacency_iterator( NULL );
   }
   // ----------------------------------------------------------------------
   EdgeModel::adjacency_iterator
   GridEdgeModel::
   begin_adjacent_nodes_w( Node& v, CommunicationDirection d )
      throw()
   {
      const Vec& pos = v.real_position();
      return new
         GridIteratorHelper<Node,const_node_set_iterator>
         ( *this, d, v, 
           clipped_cell_x( pos.x()-trans_range_-closeness_distance_-.0001 ),
           clipped_cell_x( pos.x()+trans_range_+closeness_distance_+.0001 )+1,
           clipped_cell_y( pos.y()-trans_range_-closeness_distance_-.0001 ),
		   clipped_cell_y( pos.y()+trans_range_+closeness_distance_+.0001 )+1
		 );
   }
   // ----------------------------------------------------------------------
   EdgeModel::adjacency_iterator
   GridEdgeModel::
   end_adjacent_nodes_w( Node& v )
      throw()
   {
      return EdgeModel::adjacency_iterator( NULL );
   }
   // ----------------------------------------------------------------------
   Box
   GridEdgeModel::
   observer_update_zone(Node& v,
                const Vec& pos,
                const Vec& velo )
      throw()
   {
      assert( initialized_ );

      int cx,cy;
      CellData& cd = expand_find_point(pos,cx,cy);
      NodeCell& nc = node_cell_old_w(v.id());

      if( (cx != nc.x+x_origin_cell_) ||
          (cy != nc.y+y_origin_cell_) )
         {
            CellData& oldcd = cell_data( nc.x+x_origin_cell_,
                                         nc.y+y_origin_cell_ );
            set_node_cell_absolute(nc,cx,cy);
            oldcd.remove(v);
            cd.add(v);
         }

//       cout << "GUPD BOX POS " << pos << endl
//            << "GUPD PRE-PAD " << cd.zone().lower() << " " << cd.zone().upper() << endl;
      Box b= pad_if_close( pos, cd.zone() );
//       cout << "GUPD RETURNING BOX " << b.lower() << " " << b.upper() << endl;
      return b;
      //return pad_if_close( pos, cd.zone() );
   }
   // ----------------------------------------------------------------------
   bool
   GridEdgeModel::
   supports_mobility( void )
      const throw()
   {
      return true;
   }
   // ----------------------------------------------------------------------  
   void
   GridEdgeModel::
   node_added( Node& )
      throw()
   {
      // do nothing, the important 'add-method' here is add_edge()
   }
   // ----------------------------------------------------------------------
   void
   GridEdgeModel::
   node_removed( Node& node )
      throw()
   {
      NodeCell& nc = node_cell_old_w( node.id() );
      CellData& cd = cell_data( nc.x + x_origin_cell_,
                                nc.y + y_origin_cell_ );
      cd.remove( node );
   }


















































#ifndef NDEBUG
void
GridEdgeModel::debug_dump( void )
{
   std::cout << "SIZE " << x_size() << " x " << y_size() << std::endl
             << "X INITIALIZED " << valid_x_begin_ 
             << " ... " << valid_x_end_-1 << std::endl
             << "Y INITIALIZED " << valid_y_begin_ 
             << " ... " << valid_y_end_-1 << std::endl
             << "ORIGIN CELL " << x_origin_cell_ << " " << y_origin_cell_ << std::endl
             << "CELL SIZE " << cell_width_ << "x" << cell_height_ << std::endl
             << "X0/Y0 " << x0() << " " << y0() << endl;

   std::cout << std::setw(2);
   for( int y=y_size()-1; y>=0; --y )
      {
         for( int x=0; x<x_size(); ++x )
            {
               if( !is_initialized_cell(x,y) )
                  std::cout << " x";
               else
                  std::cout << " I";

               if( (x==x_origin_cell_)&&(y==y_origin_cell_) )
                  std::cout << "o";
               else
                  std::cout << " ";
            }
         std::cout << std::endl;
      }

   std::cout << "size " << x_size() << " x " << y_size()
             << " = " << x_size()*y_size() << " cells" << std::endl
             << "allocated " << (valid_x_end_-valid_x_begin_)
             << " x " << (valid_y_end_-valid_y_begin_)
             << " = " << (valid_x_end_-valid_x_begin_)*(valid_y_end_-valid_y_begin_) << " cells"
             << std::endl;
   int nof_nonempty = 0;
   int max_size = 0;
   int sum_size = 0;
   for( int x=valid_x_begin_; x<valid_x_end_; ++x )
      for( int y=valid_y_begin_; y<valid_y_end_; ++y )
         {
            int sz = cell_data_[y][x]->nodes_.size();
            if( sz>max_size ) max_size=sz;
            sum_size+=sz;
            if( sz>0 ) ++nof_nonempty;
         }
   std::cout << (valid_x_end_-valid_x_begin_)*(valid_y_end_-valid_y_begin_)-nof_nonempty
             << " empty allocated cells" << std::endl
             << nof_nonempty << " nonempty allocated cells" << std::endl
             << max_size << " max entries in a cell" << std::endl
             << (double(sum_size)/double(nof_nonempty)) << " avg entries in nonempty cell" << std::endl;
}
#endif



   void GridEdgeModel::delayed_init( const Node& v,
                                     const Vec& pos ) throw()
   {
      cell_data_ = new CellData** [y_size()];
      for( int i=0; i<y_size(); ++i )
         cell_data_[i] = new CellData*[x_size()];

      valid_x_begin_=x_size()/2;
      valid_y_begin_=y_size()/2;
      valid_x_end_ = valid_x_begin_+1;
      valid_y_end_ = valid_y_begin_+1;

      // already checked at consruction
      assert( communication_model().exists_communication_upper_bound() );

      trans_range_ = communication_model().communication_upper_bound();
      cell_width_  = trans_range_ * transm_scale_;
      cell_height_ = trans_range_ * transm_scale_;
      closeness_distance_ = closeness_fraction_*(cell_width_-trans_range_)/2.0;


//       cout << trans_range_ << endl
//            << cell_width_ << endl
//            << closeness_fraction_ << endl
//            << closeness_distance_ << endl;abort();


      virtual_x0_ = pos.x() - (cell_width_/2.0);
      virtual_y0_ = pos.y() - (cell_height_/2.0);
      x_origin_cell_ = valid_x_begin_;
      y_origin_cell_ = valid_y_begin_;


      alloc_cell_block( valid_y_begin_, valid_y_end_,
                        valid_x_begin_, valid_x_end_ );

      init_node_cells(v.id());

      initialized_ = true;
   }

   void GridEdgeModel:: set_initial_size( int s ) throw( std::runtime_error )

      {
         assert( !initialized_ );
         if( s<1 )
            throw std::runtime_error("Initial size of GridEdgeModel must be at least 1x1");
         log_x_size_=log_y_size_=ceil_log(s);
      }

      void GridEdgeModel::set_transmission_scale( double tm ) throw( std::runtime_error )
      {
         assert( !initialized_ );
         if( tm < 1.01 )
            throw std::runtime_error("Cell size of GridEdgeModel must be at least 1.01");
         transm_scale_=tm;
      }
   GridEdgeModel::CellData& GridEdgeModel::expand_find_point( const Vec& pos, int& cx, int& cy ) throw()
      {
         cx = cell_x(pos.x());
         cy = cell_y(pos.y());

#ifndef NDEBUG
         //std::cout << "expand_find_point("<<pos.x()<<","<<pos.y()<<") needs cell (" << cx << "," << cy << ")" << std::endl;
#endif

         if( is_initialized_cell(cx,cy) )
            return *(cell_data_[cy][cx]);


         int new_log_x_size = log_x_size_;
         int new_log_y_size = log_y_size_;
         int old_in_new_x   = 0;
         int old_in_new_y   = 0;
         bool need_realloc  = false;

         if( cx < 0 )
            {
               // needed width: (x0-x) + cellwidth*xsize
               new_log_x_size =
                  ceil_log( ((x0()-pos.x())/cell_width_) + x_size() ); // TODO-X0
               // align right:
               old_in_new_x = 
                  (1<<new_log_x_size) - x_size();
               need_realloc = true;
            }
         else if ( cx>=x_size() )
            {
               // needed width: x-x0
               new_log_x_size =
                  ceil_log( (pos.x()-x0())/cell_width_ );// TODO-X0
               // align left
               old_in_new_x = 0;
               need_realloc = true;
            }

         if( cy < 0 )
            {
               // needed height: (y0-y) + cellheight*ysize
               new_log_y_size =
                  ceil_log( ((y0()-pos.y())/cell_height_) + y_size() ); // TODO-X0
               // align top:
               old_in_new_y = 
                  (1<<new_log_y_size) - y_size();
               need_realloc = true;
            }
         else if ( cy>=y_size() )
            {
               // needed height: y-y0
               new_log_y_size =
                  ceil_log( (pos.y()-y0())/cell_height_ ); // TODO-X0
               // align bottom
               old_in_new_y = 0;
               need_realloc = true;
            }

         if( need_realloc )
            {
               expand_realloc( new_log_x_size,
                               new_log_y_size,
                               old_in_new_x,
                               old_in_new_y );
               cx = cell_x(pos.x());
               cy = cell_y(pos.y());
            }

#ifndef NDEBUG
//          std::cout << "new_log_x_size " << new_log_x_size << " -> " << (1<<new_log_x_size) << std::endl
//                    << "old_in_new_x   " << old_in_new_x << std::endl;
//          std::cout << "new_log_y_size " << new_log_y_size << " -> " << (1<<new_log_y_size) << std::endl
//                    << "old_in_new_y   " << old_in_new_y << std::endl;
#endif

      
         assert( cx>=0 ); assert( cx<x_size() );
         assert( cy>=0 ); assert( cy<y_size() );

         if( cx<valid_x_begin_ )
            {
               alloc_cell_block( valid_y_begin_, valid_y_end_,
                                 cx, valid_x_begin_ );
               valid_x_begin_=cx;
            }
         else if( cx>=valid_x_end_ )
            {
               alloc_cell_block( valid_y_begin_, valid_y_end_,
                                 valid_x_end_, cx+1 );
               valid_x_end_=cx+1;
            }

         if( cy<valid_y_begin_ )
            {
               alloc_cell_block( cy, valid_y_begin_,
                                 valid_x_begin_, valid_x_end_ );
               valid_y_begin_=cy;
            }
         else if( cy>=valid_y_end_ )
            {
               alloc_cell_block( valid_y_end_, cy+1,
                                 valid_x_begin_, valid_x_end_ );
               valid_y_end_=cy+1;
            }

         assert( is_initialized_cell(cx,cy) );
         return *(cell_data_[cy][cx]);
      }




   GridEdgeModel::CellData** GridEdgeModel::realloced_row( int ypos,
                                int newlogx,
                                int xold ) throw()
      {
         CellData** pd = alloc_plain_row(newlogx);
         std::copy( cell_data_[ypos]+valid_x_begin_,
                    cell_data_[ypos]+valid_x_end_,
                    pd + xold + valid_x_begin_ );
         delete[] cell_data_[ypos];
         return pd;
      }

   void GridEdgeModel::alloc_cell_row( int y_pos,
                           int x_begin, int x_end )
         throw()
      {
         assert( y_pos >= 0 );
         assert( y_pos < y_size() );
         assert( x_begin >=0 );
         assert( x_begin < x_end );
         assert( x_end <= x_size() );
         for(int x=x_begin; x!=x_end;++x )
            cell_data_[y_pos][x] = new CellData(*this,x-x_origin_cell_,y_pos-y_origin_cell_);
      }

   void GridEdgeModel::alloc_cell_block( int y_begin, int y_end,
                             int x_begin, int x_end )
         throw()
      {
         for(;y_begin!=y_end;++y_begin)
            alloc_cell_row(y_begin,x_begin,x_end);
      }

   void GridEdgeModel::expand_realloc( int logx,
                           int logy,
                           int xold,
                           int yold )
         throw()
      {
         if( (logx+logy) >= HUGE_GRID_LOG_SIZE )
            {
               std::cout << "WARNING: Expanding grid to ridiculous size of "
                         << (1<<logx) << "x"
                         << (1<<logy) << " cells!" << std::endl
                         << "Increase " << INITIAL_CELL_SIZE_PARAMETER
                         << " to keep the grid size small" << std::endl;
               abort();
            }

         if( logy > log_y_size_ )
            {
               // y-vec needs realloc, too
               CellData*** newcd =
                  new CellData** [1<<logy];
            
               // copy or realloc old rows 0..y_size-1
               // to new array positions from yold
               if( logx > log_x_size_ )
                  for( int y=y_size()-1; y>=0; --y )
                     newcd[yold+y] = realloced_row(y,logx,xold);
               else
                  for( int y=y_size()-1; y>=0; --y )
                     newcd[yold+y] = cell_data_[y];

               // rows 0..yold-1 and yold+ysize..newysize-1
               // must be initialized
               for( int y=0; y<yold; ++y )
                  newcd[y] = alloc_plain_row(logx);
               for( int y=yold+y_size(); y<(1<<logy); ++y )
                  newcd[y] = alloc_plain_row(logx);

               delete[] cell_data_;
               cell_data_ = newcd;
            }
         else
            {
               // no y-rescale, we can simply overwrite cell_data_
               for( int y=y_size()-1; y>=0; --y )
                  cell_data_[y] = realloced_row(y,logx,xold);
            }

         x_origin_cell_ += xold;
         valid_x_begin_ += xold;
         valid_x_end_   += xold;

         y_origin_cell_ += yold;
         valid_y_begin_ += yold;
         valid_y_end_   += yold;

         log_x_size_ = logx;
         log_y_size_ = logy;
      }
   
   void GridEdgeModel::set_closeness_fraction( double cf ) throw( std::runtime_error )
      {
         assert( !initialized_ );
         if( (cf<.0001) || (cf>.9999) )
            throw std::runtime_error("grid_closeness_fraction must be 0<gcf<1");
         closeness_fraction_=cf;
      }


   GridEdgeModel::NodeCell& GridEdgeModel::node_cell_new_w( int id ) throw()
      {
         assert( id >= 0 );
         if( id >= node_cells_size_ )
            {
               int newsize = id+1;
               if( newsize < 2*node_cells_size_ ) 
                  newsize=2*node_cells_size_;

               NodeCell* icnew = new NodeCell[newsize];
#ifdef NDEBUG
               std::copy( node_cells_,
                          node_cells_+node_valid_end_,
                          icnew );
#else
               std::copy( node_cells_,
                          node_cells_+node_cells_size_,
                          icnew );
               for( int i=node_cells_size_; i<newsize; ++i )
                  icnew[i].x = MAGIC_UNITIALIZED_NODE_CELL_X_;
#endif
               node_cells_size_=newsize;
               delete[] node_cells_;
               node_cells_ = icnew;
            }

         if( id>=node_valid_end_ )
            node_valid_end_ = id+1;
      
#ifndef NDEBUG
		 //std::cout<<"GridEdgeModel::node_cell_new_w(): called now"<<std::endl;
//          assert(/* if this fails, add_node() resp. obtain_initial_zone() 
//                    was called at least twice for the same node */
//                   node_cells_[id].x == MAGIC_UNITIALIZED_NODE_CELL_X_ );
#endif
         return node_cells_[id];
      }
   void GridEdgeModel::init_node_cells( int id ) throw()
      {
         assert( id>=0 );
         node_cells_size_ = id+1;
         node_cells_ = new NodeCell[node_cells_size_];
         node_valid_end_ = id+1;
#ifndef NDEBUG
         for( int i=0; i<node_cells_size_; ++i )
            node_cells_[i].x = MAGIC_UNITIALIZED_NODE_CELL_X_;
#endif
      }



#ifndef NDEBUG
   void GridEdgeModel::assert_integrity( void ) const throw()
      {
         assert( node_valid_end_ <= node_cells_size_ );
         for( World::const_node_iterator
                 it    = world().begin_nodes(),
                 endit = world().end_nodes();
              it != endit; ++it )
            {
               assert( it->id() < node_valid_end_ );
               assert( node_cells_[it->id()].x != MAGIC_UNITIALIZED_NODE_CELL_X_ );
               int cx = node_cells_[it->id()].x + x_origin_cell_;
               int cy = node_cells_[it->id()].y + y_origin_cell_;
            
               assert( cx >= valid_x_begin_ );
               assert( cx < valid_x_end_ );
               assert( cy >= valid_y_begin_ );
               assert( cy < valid_y_end_ );

               assert( cell_data_[cy][cx]->contains(*it) );

               Box box = cell_data_[cy][cx]->zone();
               box.lower_w() -= Vec(.001,.001,0);
               box.upper_w() += Vec(.001,.001,0);

               assert( box.contains(it->real_position()) );
            }
      }
#endif

}


/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/grid_edge_model.cpp,v $
 * Version $Revision: 108 $
 * Date    $Date: 2007-11-26 15:45:37 +0100 (Mon, 26 Nov 2007) $
 *-----------------------------------------------------------------------
 * $Log: grid_edge_model.cpp,v $
 *-----------------------------------------------------------------------*/
