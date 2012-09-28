/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MATH_SIMPLE_MATRIX_H
#define __SHAWN_APPS_LOCALIZATION_MATH_SIMPLE_MATRIX_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION
#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"

#include <vector>
#include <stdexcept>


namespace localization
{

   class SimpleMatrixError
      : public std::logic_error
   {
   public:
      ///
      SimpleMatrixError( const std::string& msg )
         : std::logic_error( msg )
      {};
      ///
      virtual ~SimpleMatrixError() throw()
      {};
   };


   template<class T>
   class SimpleMatrix
	   : public shawn::RefcntPointable
   {

   public:
      ///@name construction / destruction
      ///@{
      ///
      SimpleMatrix( size_t = 0, size_t = 0 );
      SimpleMatrix( const SimpleMatrix<T>& );
      virtual ~SimpleMatrix();
      ///@}

      ///
      inline const T& at( size_t, size_t ) const throw( SimpleMatrixError );
      ///
      inline T& at( size_t, size_t ) throw( SimpleMatrixError );
      ///
      inline const T& operator() ( size_t, size_t ) const throw( SimpleMatrixError );
      ///
      inline T& operator() ( size_t, size_t ) throw( SimpleMatrixError );

      ///
      inline SimpleMatrix<T>& operator= (
         const SimpleMatrix<T>& )
         throw();
      ///
      inline SimpleMatrix<T>& operator*= (
         const SimpleMatrix<T>& )
         throw( SimpleMatrixError );
      ///
      inline friend SimpleMatrix<T> operator*(
         const SimpleMatrix<T>& lsm1, const SimpleMatrix<T>& lsm2 )
         throw( SimpleMatrixError )
      {
         SimpleMatrix<T> tmp( lsm1 );
         tmp *= lsm2;

         return tmp;
      }
      ///
      SimpleMatrix<T>& operator*= ( T )
         throw();
      ///
      friend SimpleMatrix<T> operator*(
         const SimpleMatrix<T>& lsm, T value )
         throw()
      {
         SimpleMatrix<T> tmp( lsm );
         tmp *= value;

         return tmp;
      }
      ///
      friend SimpleMatrix<T> operator*(
         T value, const SimpleMatrix<T>& lsm )
         throw()
      { return lsm * value; }

	   inline SimpleMatrix<T>& operator-= (
         const SimpleMatrix<T>& )
         throw( SimpleMatrixError );
      ///
      inline friend SimpleMatrix<T> operator-(
         const SimpleMatrix<T>& lsm1, const SimpleMatrix<T>& lsm2 )
         throw( SimpleMatrixError )
      {
         SimpleMatrix<T> tmp( lsm1 );
         tmp -= lsm2;

         return tmp;
      }
	  inline SimpleMatrix<T>& operator+= (
         const SimpleMatrix<T>& )
         throw( SimpleMatrixError );
      ///
      inline friend SimpleMatrix<T> operator+(
         const SimpleMatrix<T>& lsm1, const SimpleMatrix<T>& lsm2 )
         throw( SimpleMatrixError )
      {
         SimpleMatrix<T> tmp( lsm1 );
         tmp += lsm2;

         return tmp;
      }
      ///
      ///
      SimpleMatrix<T> transposed( void ) const throw();
      ///
      double det( void ) const throw();
      ///
      SimpleMatrix<T> inverse( void ) const throw();

	  SimpleMatrix<T> covariance ( void ) const throw();

      ///
      inline size_t row_cnt( void ) const throw();
      ///
      inline size_t col_cnt( void ) const throw();
      ///
      inline const std::vector<T>& as_vector( void ) const
         throw();

      ///
      friend std::ostream& operator<<(
            std::ostream& os, const SimpleMatrix<T>& lsm )
         throw()
      {
         for ( size_t i=0; i < lsm.row_cnt(); i++ )
         {
            for ( size_t j=0; j < lsm.col_cnt(); j++ )
               os << lsm( i, j ) << '\t';

            os << std::endl;
         }

         return os;
      }

   private:
      size_t rows_, cols_;
      std::vector<T> matrix_;

   };


   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------


   template<class T>
   SimpleMatrix<T>::
   SimpleMatrix( size_t rows, size_t cols )
      : rows_( rows ),
         cols_ ( cols )
   {
      matrix_ = std::vector<T>( rows * cols );
   }
   // ----------------------------------------------------------------------
   template<class T>
   SimpleMatrix<T>::
   SimpleMatrix( const SimpleMatrix& lsm )
      : rows_( lsm.row_cnt() ),
         cols_ ( lsm.col_cnt() ),
         matrix_( lsm.as_vector() )
   {}
   // ----------------------------------------------------------------------
   template<class T>
   SimpleMatrix<T>::
   ~SimpleMatrix()
   {}
   // ----------------------------------------------------------------------
   template<class T>
   const T&
   SimpleMatrix<T>::
   at( size_t row, size_t col )
      const throw( SimpleMatrixError )
   {
      if ( row < 0 || row > rows_ || col < 0 || col > cols_ )
         throw SimpleMatrixError( "index out of range" );

      return matrix_[ row*cols_ + col ];
   }
   // ----------------------------------------------------------------------
   template<class T>
   T&
   SimpleMatrix<T>::
   at( size_t row, size_t col )
      throw( SimpleMatrixError )
   {
      if ( row < 0 || row > rows_ || col < 0 || col > cols_ )
         throw SimpleMatrixError( "index out of range" );

      return matrix_[ row*cols_ + col ];
   }
   // ----------------------------------------------------------------------
   template<class T>
   const T&
   SimpleMatrix<T>::
   operator()( size_t row, size_t col )
      const throw( SimpleMatrixError )
   {
      return at( row, col );
   }
   // ----------------------------------------------------------------------
   template<class T>
   T&
   SimpleMatrix<T>::
   operator()( size_t row, size_t col )
      throw( SimpleMatrixError )
   {
      return at( row, col );
   }
   // ----------------------------------------------------------------------
   template<class T>
   SimpleMatrix<T>&
   SimpleMatrix<T>::
   operator=( const SimpleMatrix<T>& lsm )
      throw()
   {
      rows_ = lsm.row_cnt();
      cols_ = lsm.col_cnt();
      matrix_ = lsm.as_vector();

      return *this;
   }
   // ----------------------------------------------------------------------
   template<class T>
   SimpleMatrix<T>&
   SimpleMatrix<T>::
   operator*=( const SimpleMatrix& lsm )
      throw( SimpleMatrixError )
   {
      if ( cols_ != lsm.row_cnt() )
         throw SimpleMatrixError( "multiplication failed" );

      SimpleMatrix<T> tmp( rows_, lsm.col_cnt() );

      for ( size_t i = 0; i < rows_; i++ )
         for ( size_t j = 0; j < lsm.col_cnt(); j++ )
         {
            tmp(i,j) = 0;
            for ( size_t k = 0; k < cols_; k++ )
               tmp(i,j) += at(i,k) * lsm(k,j);
         }

      *this = tmp;
      return *this;
   }
   // ----------------------------------------------------------------------
   template<class T>
   SimpleMatrix<T>&
   SimpleMatrix<T>::
   operator*=( T value )
      throw()
   {
      for ( size_t i = 0; i < rows_; i++ )
         for ( size_t j = 0; j < cols_; j++ )
            at(i,j) *= value;

      return *this;
   }
// ----------------------------------------------------------------------
      template<class T>
   SimpleMatrix<T>&
   SimpleMatrix<T>::
   operator+=( const SimpleMatrix& lsm )
      throw( SimpleMatrixError )
   {
	   if ( cols_ != lsm.col_cnt() || rows_ != lsm.row_cnt() )
         throw SimpleMatrixError( "addition failed" );

      SimpleMatrix<T> tmp( rows_,cols_);

      for ( size_t i = 0; i < rows_; i++ )
         for ( size_t j = 0; j < cols_; j++ )
         {
            tmp(i,j) = at(i,j) + lsm(i,j);
         }

      *this = tmp;
      return *this;
   }
   // ----------------------------------------------------------------------
      template<class T>
   SimpleMatrix<T>&
   SimpleMatrix<T>::
   operator-=( const SimpleMatrix& lsm )
      throw( SimpleMatrixError )
   {
       if ( cols_ != lsm.col_cnt() || rows_ != lsm.row_cnt() )
         throw SimpleMatrixError( "subtraction failed" );

      SimpleMatrix<T> tmp( rows_,cols_);

      for ( size_t i = 0; i < rows_; i++ )
         for ( size_t j = 0; j < cols_; j++ )
         {
            tmp(i,j) = at(i,j) - lsm(i,j);
         }

      *this = tmp;
      return *this;
   }
   // ----------------------------------------------------------------------
   template<class T>
   SimpleMatrix<T>
   SimpleMatrix<T>::
   transposed( void )
      const throw()
   {
      SimpleMatrix<T> tmp( cols_, rows_ );

      for ( size_t i = 0; i < rows_; i++ )
         for ( size_t j = 0; j < cols_; j++ )
            tmp(j,i) = at(i,j);

      return tmp;
   }
   // ----------------------------------------------------------------------
   template<class T>
   double
   SimpleMatrix<T>::
   det( void )
      const throw()
   {
      //assert( rows_ == 2 && cols_ == 2);
	if( rows_ == 2 && cols_ == 2)
      return at(0,0) * at(1,1) - at(0,1) * at(1,0);
	 if( rows_ == 3 && cols_ == 3)
		 return ((at(0,0)*at(1,1)*at(2,2)) + (at(0,1)*at(1,2)*at(2,0)) +
		 (at(0,2)*at(1,0)*at(2,1)) - (at(0,2)*at(1,1)*at(2,0)) -
		 (at(0,0)*at(1,2)*at(2,1)) - (at(0,1)*at(1,0)*at(2,2)));
	 else 
		 return 0;

   }
   // ----------------------------------------------------------------------
   template<class T>
   SimpleMatrix<T>
	   SimpleMatrix<T>::
	   inverse( void )
	   const throw()
   {
	 //  assert( rows_ == 2 && cols_ == 2 && det() != 0 );
		
	   SimpleMatrix<T> tmp( *this );
	   if(rows_ == 2 && cols_ == 2 && det() != 0){ 
	   
		T save = tmp(0,0);
	   tmp(0,0) = tmp(1,1);
	   tmp(1,1) = save;

	   tmp(0,1) *= -1;
	   tmp(1,0) *= -1;

	   tmp *= ( 1/det() );
	   }
	   else if( (rows_ == 3 && cols_ == 3 && det() != 0))
	   {
		tmp(0,0) = at(1,1)*at(2,2) - at(1,2)* at(2,1);
	    tmp(0,1) = at(0,2)*at(2,1) - at(0,1)* at(2,2);
		tmp(0,2) = at(0,1)*at(1,2) - at(0,2)* at(1,1);
		tmp(1,0) = at(1,2)*at(2,0) - at(1,0)* at(2,2);
		tmp(1,1) = at(0,0)*at(2,2) - at(0,2)* at(2,0);
		tmp(1,2) = at(0,2)*at(1,0) - at(0,0)* at(1,2);
		tmp(2,0) = at(1,0)*at(2,1) - at(1,1)* at(2,0);
		tmp(2,1) = at(0,1)*at(2,0) - at(0,0)* at(2,1);
		tmp(2,2) = at(0,0)*at(1,1) - at(0,1)* at(1,0);
		tmp *= 1/det();
	   }
	

	   return tmp;
   }

      template<class T>
   SimpleMatrix<T>
   SimpleMatrix<T>::
   covariance( void )
      const throw()
   {
      SimpleMatrix<T> tmp( *this );

	  tmp=tmp.transposed();
	  tmp*= *this; 
	  tmp=tmp.inverse();
      return tmp;
   }
   // ----------------------------------------------------------------------
   template<class T>
   size_t
   SimpleMatrix<T>::
   row_cnt( void )
      const throw()
   {
      return rows_;
   }
   // ----------------------------------------------------------------------
   template<class T>
   size_t
   SimpleMatrix<T>::
   col_cnt( void )
      const throw()
   {
      return cols_;
   }
   // ----------------------------------------------------------------------
   template<class T>
   const std::vector<T>&
   SimpleMatrix<T>::
   as_vector( void )
      const throw()
   {
      return matrix_;
   }

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/math/localization_simple_matrix.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_simple_matrix.h,v $
 *-----------------------------------------------------------------------*/
