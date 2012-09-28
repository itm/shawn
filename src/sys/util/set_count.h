/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_SET_COUNT_H
#define __SHAWN_SYS_SET_COUNT_H

#include <fstream>

using namespace std;

namespace shawn
{

   /**
    * Counts set differences very fast without an explicit
    * construction of temporaries. The results are written into the output
    * parameters. Any of the output parameters may be NULL, if the result
    * is not needed.
    *
    * @param[in]  s1        the first set
    * @param[in]  s2        the second set
    * @param[out] *cs1wos2  contains @f$|s1\setminus s2|@f$
    * @param[out] *cs2wos1  contains @f$|s2\setminus s1|@f$
    * @param[out] *cs1      contains @f$|s1|@f$
    * @param[out] *cs2      contains @f$|s2|@f$
    * @param[out] *csect    contains @f$|s1\cap s2|@f$
    * @param[out] *cunion   contains @f$|s1\cup s2|@f$
    */
   template<typename T>
   void count_set_differences( const std::set<T>& s1,
                               const std::set<T>& s2,
                               int* cs1wos2 = NULL,
                               int* cs2wos1 = NULL,
                               int* cs1     = NULL,
                               int* cs2     = NULL,
                               int* csect   = NULL,
                               int* cunion  = NULL )
   {
		

	  typename std::set<T>::const_iterator it1  = s1.begin();
      typename std::set<T>::const_iterator it2  = s2.begin();
      typename std::set<T>::const_iterator end1 = s1.end();
      typename std::set<T>::const_iterator end2 = s2.end();

      int cs1wos2_dummy;
      if( cs1wos2 == NULL ) cs1wos2=&cs1wos2_dummy;

      int cs2wos1_dummy;
      if( cs2wos1 == NULL ) cs2wos1=&cs2wos1_dummy;

      int csect_dummy;
      if( csect == NULL ) csect=&csect_dummy;

      *cs1wos2 = 0;
      *cs2wos1 = 0;
      *csect   = 0;

      while( (it1 != end1) && (it2 != end2) )
         if( (*it1) < (*it2) )
            {
               ++*cs1wos2;
               ++it1;
            }
         else if( (*it1) > (*it2) )
            {
               ++*cs2wos1;
               ++it2;
            }
         else
            {
				//cout << "1(" << &s1 << ").s" << s1.size() << "2(" << &s2 << ").s" << s2.size() << "c" << *csect << endl;
				assert( (*it1) == (*it2) );
               ++*csect;
               ++it1;
               ++it2;
            }

      for( ; it1 != end1; ++it1 ) ++*cs1wos2;
      for( ; it2 != end2; ++it2 ) ++*cs2wos1;


      if( cs1 != NULL ) *cs1 = *cs1wos2 + *csect;
      if( cs2 != NULL ) *cs2 = *cs2wos1 + *csect;
      if( cunion != NULL ) *cunion = *cs1wos2 + *cs2wos1 + *csect;
   }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/util/set_count.h,v $
 * Version $Revision: 504 $
 * Date    $Date: 2011-01-24 20:00:04 +0100 (Mon, 24 Jan 2011) $
 *-----------------------------------------------------------------------
 * $Log: set_count.h,v $
 *-----------------------------------------------------------------------*/
