/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_BOX_H
#define __SHAWN_SYS_MISC_BOX_H

#include "sys/vec.h"
#include <iostream>

namespace shawn
{
    /// Axis-parallel box (cuboid)
    class Box
    {
    public:
        static const Box INFINITE_3D_SPACE;
        static const Box EMPTY_BOX;

        ///@name construction / destruction
        ///@{
        ///
        Box();
        ///
        Box( const Vec&, const Vec& );
        ///
        Box( const Box& );
        ///
        explicit Box( const Vec& );
        ///
        virtual ~Box();
        ///@}


        ///@name modification
        ///@{
        ///
        Box& operator = ( const Box& ) throw();
        ///
        inline Vec& lower_w( void ) throw()
        { return p1; }
        ///
        inline Vec& upper_w( void ) throw()
        { return p2; }
        ///@}

        ///@name access
        ///@{
        /** \return whether the given vector is contained in the box */
        bool contains( const Vec& ) const throw();
        ///
	/** \return whether the given coordinates, as there are x, y and z 
	 *  from the vectors p1 & p2 overlap with passed vectors. 
	 *  Furthermore just one coordinate is checked at a time. That is 
	 *  why the method intersects() calls the method overlaps() three
	 *  times, if necessary, in order to determine, if the the given 
	 *  boxes intersect. 
	 */
	bool overlaps( const double& this_p1, const double& this_p2, const double& b1, const double& b2 ) const throw();
	///
	/** \return whether the given box intersecs with this box */
	bool intersects( const Box& b ) const throw();
	///
        inline const Vec& lower( void ) const throw()
        { return p1; }
        ///
        inline const Vec& upper( void ) const throw()
        { return p2; }
        ///@}

        ///
        inline bool operator == ( const Box& p ) const throw()
        { return p1 == p.p1 && p2 == p.p2; }

        friend std::ostream& operator << ( std::ostream&, const Box& )
            throw();
    private:
        Vec p1;
        Vec p2;
    };

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/misc/box.h,v $
* Version $Revision: 359 $
* Date    $Date: 2009-08-05 14:06:18 +0200 (Wed, 05 Aug 2009) $
*-----------------------------------------------------------------------
* $Log: box.h,v $
 *-----------------------------------------------------------------------*/
