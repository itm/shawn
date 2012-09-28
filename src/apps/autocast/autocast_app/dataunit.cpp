/************************************************************************
 ** This file is part of the network simulator ns-2.                   **
 ** Copyright (C) 2006 by AutoNomos (www.auto-nomos.de)                **
 ** This part of ns-2 is free software; you can redistribute it and/or **
 ** modify it under the terms of the GNU General Public License,       **
 ** version 2.                                                         **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>                  **
 **                                                                    **
 ************************************************************************/
#include "dataunit.h"
#include <iostream>
#ifdef ENABLE_AUTOCAST

namespace autocast{

	DataUnit::DataUnit() : area_(NULL),
						   max_life_time_(0),
						   priority_(0),
						   from_addr_(NULL),
						   time_(0),
						   x_(0),
						   y_(0),
						   responsible_app_(""),
						   id_(0)
	{
		hop_count_ = 0;
		size_ = 1;
	}

	DataUnit::DataUnit(int id,
					   const shawn::Node* from_addr,
					  /* double x,
					   double y,
					   double time,*/
					   double max_life_time,
					   int priority,
					   std::string responsible_app,
					   const autocast::DistributionArea* area) : area_(area),
																 max_life_time_(max_life_time),
																 priority_(priority),
																 from_addr_(from_addr),
																 time_(from_addr->world().current_time()),
																 x_(from_addr->real_position().x()),
																 y_(from_addr->real_position().y()),
																 responsible_app_(responsible_app),
																 id_(id)
	{
		hop_count_ = 0;
		size_ = 1;
	}


	// ----------------------------------------------------------------------
	DataUnit::
		DataUnit(const DataUnit& o) : max_life_time_(o.max_life_time()),
									  priority_(o.priority()),
									  from_addr_(o.from_addr()),
									  time_(o.time()),
									  x_(o.x()),
									  y_(o.y()),
									  responsible_app_(o.responsible_app()),
									  id_(o.id())
	{
		area_ = o.distribution_area()->copy();
		hop_count_ = 0;
		size_ = o.size();
	}
	// ----------------------------------------------------------------------
	DataUnit::
		~DataUnit()
	{
		if(area_) delete area_;
		area_ = NULL;
	}
	// ----------------------------------------------------------------------
	int
		DataUnit::
		id()
		const
		throw()
	{
		return id_;
	}
	// ----------------------------------------------------------------------
	double
		DataUnit::
		time()
		const
		throw()
	{
		return time_;
	}
	// ----------------------------------------------------------------------
	double
		DataUnit::
		x()
		const
		throw()
	{
		return x_;
	}
	// ----------------------------------------------------------------------
	double
		DataUnit::
		y()
		const
		throw()
	{
		return y_;
	}
	// ----------------------------------------------------------------------
	int
		DataUnit::
		size()
		const
		throw()
	{
		return size_;
	}
	// ----------------------------------------------------------------------
	void
		DataUnit::
		size(const int s)
		throw()
	{
		size_ = s;
	}
	// ----------------------------------------------------------------------
	std::string
		DataUnit::
		responsible_app()
		const
		throw()
	{
		return responsible_app_;
	}
	// ----------------------------------------------------------------------
	const autocast::DistributionArea*
		DataUnit::
		distribution_area()
		const
		throw()
	{
		return area_;
	}
	// ----------------------------------------------------------------------
	double
		DataUnit::
		max_life_time()
		const
		throw()
	{
		return max_life_time_;
	}
	// ----------------------------------------------------------------------
	int
		DataUnit::
		hop_count()
		const
		throw()
	{
		return hop_count_;
	}
	// ----------------------------------------------------------------------
	void
		DataUnit::
		hop_count(const int h)
		throw()
	{
		hop_count_ = h;
	}
	// ----------------------------------------------------------------------
	int
		DataUnit::
		priority()
		const
		throw()
	{
		return priority_;
	}
	// ----------------------------------------------------------------------
	const shawn::Node*
		DataUnit::
		from_addr()
		const
		throw()
	{
		return from_addr_;
	}
	// ----------------------------------------------------------------------
	int
		operator<(const autocast::DataUnit& lv, const autocast::DataUnit& rv)
	{
		return lv.id() < rv.id();
	}

}
#endif


/*-----------------------------------------------------------------------
 * Source  $Source$
 * Version $Revision: 269 $
 * Date    $Date: 2007-01-09 19:25:34 +0100 (Di, 09 Jan 2007) $
 *-----------------------------------------------------------------------
 * $Log$
 * Revision 1.9  2006/08/24 13:38:57  wegener
 * - spyglass support
 * - HDCtransport erweitert
 *
 * Revision 1.8  2006/08/03 12:43:56  wegener
 * *** empty log message ***
 *
 * Revision 1.7  2006/07/10 12:49:42  wegener
 * First version of HDCtransport
 *
 * Revision 1.6  2006/06/22 13:16:37  wegener
 * *** empty log message ***
 *
 * Revision 1.4  2006/06/16 11:44:55  wegener
 * *** empty log message ***
 *
 * Revision 1.3  2006/06/15 13:18:58  wegener
 * added copyright and CVS-log
 *
 *-----------------------------------------------------------------------*/
