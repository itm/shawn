/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_READING_H
#define __SHAWN_APPS_READING_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#ifndef READING_TRACE
#define READING_TRACE(x) //std::cout<<x<<std::endl;	//show comments
#endif

#include "sys/util/keeper_managed.h"
#include "sys/world.h"
#include "sys/misc/box.h"

#include <vector>
#include <set>

namespace reading
{
  class ReadingKeeper;
  class ReadingChangedHandler;


  DECLARE_HANDLES(Reading);

  ///  Superclass for every type of reading.
  /**  Readings in general contain information about a world.
   *
   */
  class Reading
  : public shawn::KeeperManaged
  {
  public:

    typedef std::pair<ReadingChangedHandler*, shawn::Box> RB;
    typedef std::vector<RB> ReadingChangedHandlerList;

    ///@name construction / destruction
    ///@{
    ///
    Reading();
    ///
    virtual ~Reading();
    ///@}

    ///@name world membership
    ///@{
    /** Sets the world that contains this reading,
     *  used by World
     */
    virtual void set_world( shawn::World& )throw();
    /// \return the domain of the reading, i.e., the region in which
    /// value() returns meaningful values. It is feasible to query
    /// values outside domain().
    virtual shawn::Box domain( void ) const throw()
    {
      return shawn::Box::INFINITE_3D_SPACE;
    }
    ///@}
    ///
    /** called after world is setup and before first node is created.
     *  used by World
     */
    virtual void init( void ) throw();

    virtual std::string name( void ) const throw();
    virtual std::string description( void ) const throw();


  protected:
    ///
    ///@name world membership
    ///@{
    /** returns true or false if this Reading already has a World or not */
    bool has_world( void ) const throw();
    /** \return const World that contains this Reading */
    virtual const shawn::World& world( void ) const throw();
    /** \return writable World that contains this Reading */
    virtual shawn::World& world_w( void ) throw();
    ///@}
    //std::string name_;

  private:
    shawn::World* world_;
  };


}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/reading.h,v $
 * Version $Revision: 287 $
 * Date    $Date: 2008-11-16 11:35:44 +0100 (So, 16. Nov 2008) $
 *-----------------------------------------------------------------------
 * $Log: reading.h,v $
 *-----------------------------------------------------------------------*/

