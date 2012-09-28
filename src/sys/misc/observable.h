/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_MISC_OBSERVABLE_H__
#define __SHAWN_SYS_MISC_OBSERVABLE_H__

#include <set>
#include <vector>
#include <iostream>
#include "sys/misc/box.h"

namespace shawn
{

    /**
      *
      */
    template<typename T>
    class Observable
    {
    protected:
		///Set of Observers
        typedef std::set<T*> ObserverSet;        
		///Iterator over a set of observers
        typedef typename std::set<T*>::iterator ObserverIterator;

	private:
        ObserverSet observers_;        

    protected:

        virtual ObserverIterator observers_begin() throw ()
        {
            return observers_.begin();
        }

        virtual ObserverIterator observers_end() throw ()
        {
            return observers_.end();
        }

		virtual void observers_added(T& obs) throw() = 0;

    public:

		virtual ~Observable()
		{}

        virtual void observers_clear() throw()
        {
            observers_.clear();
        }

        virtual void observer_add(T& obs) throw () 
        {
            observers_.insert(&obs);
			observers_added(obs);
        }

        virtual void observer_remove(T& obs) throw ()
        {
            observers_.erase(&obs);
        }

        virtual int observer_count() throw ()
        {
            return observers_.size();
        }

    };
    
    /**
      *
      */
    class BoxContainer
    {
    public:
        typedef std::vector<Box> Boxes;
        typedef Boxes::iterator BoxIterator;
    private:
        /// The current boxes
        Boxes boxes_;

    public:    
		///
		virtual ~BoxContainer()
		{}
        
        /// Begin iterator over the currenty active boxes
        virtual BoxIterator boxes_begin() throw() 
        {
            return boxes_.begin();
        }

        /// End iterator over the currenty active boxes
        virtual BoxIterator boxes_end() throw() 
        {
            return boxes_.end();
        }
        
        ///Remove all boxes
        virtual void boxes_clear() throw()
        {
            boxes_.clear();    
        }
        
        ///Add a box
        virtual void boxes_add(Box b) throw()
        {
            boxes_.push_back(b);
        }
        
        ///Returns true if p is not contained in one of the boxes
        virtual bool boxes_violated(const Vec& p)
        {
            for(BoxIterator it = boxes_begin(), end = boxes_end(); it!=end; ++it)
                if(! (*it).contains(p) )
                    return true;
                
            return false;
        }
        
    };
    
}
    
#endif
    
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/misc/observable.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: observable.h,v $
 *-----------------------------------------------------------------------*/

    
