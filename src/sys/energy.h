/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_ENERGY_H
#define __SHAWN_SYS_ENERGY_H


#include <iostream>

namespace shawn
{

   class Energy
   {
   public:
      static unsigned int INFINITE_ENERGY_VAL;
      static Energy INFINITE_ENERGY;

      Energy() : val_(0) {}
      Energy( const Energy& e ) : val_(e.val_) {}
      Energy( unsigned int v ) : val_(v) {}
      ~Energy(){}

      Energy& operator= ( const Energy& e )
      { val_ = e.val_; return *this; }
   

      inline bool is_infinite( void )
         const throw()
      { return val_ == INFINITE_ENERGY_VAL; }
      inline bool is_finite( void )
         const throw()
      { return val_ != INFINITE_ENERGY_VAL; }

      bool operator == ( const Energy& e )
         const throw()
      { return val_ == e.val_; }
      bool operator < ( const Energy& e )
         const throw()
      { return val_ < e.val_; }
      bool operator > ( const Energy& e )
         const throw()
      { return val_ > e.val_; }
      bool operator <= ( const Energy& e )
         const throw()
      { return val_ <= e.val_; }
      bool operator >= ( const Energy& e )
         const throw()
      { return val_ >= e.val_; }
      bool operator != ( const Energy& e )
         const throw()
      { return val_ != e.val_; }


      Energy operator + ( const Energy& e )
         const throw()
      {
         if( is_infinite() || e.is_infinite() )
            return INFINITE_ENERGY;
         return Energy( val_ + e.val_ );
      }

      Energy operator - ( const Energy& e )
         const throw()
      {
         if( is_infinite() || e.is_infinite() )
            return INFINITE_ENERGY;
         else if( val_ < e.val_ )
            return Energy( 0 );
         else
            return Energy( val_ - e.val_ );
      }

      Energy& operator += ( const Energy& e )
         throw()
      {
         return *this = (*this + e );
      }
      Energy& operator -= ( const Energy& e )
         throw()
      {
         return *this = (*this - e );
      }

      friend std::ostream& operator<< ( std::ostream& os, const Energy& e )
      { 
         if( e.is_infinite() )
            return os << "INF";
         else
            return os << e.val_;
      }
   private:
      unsigned int val_;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/energy.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: energy.h,v $
 *-----------------------------------------------------------------------*/
