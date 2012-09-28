/************************************************************************
** This file is part of the network simulator Shawn.                   **
** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                 **
** This part of Shawn is free software; you can redistribute it and/or **
** modify it under the terms of the BSD License. Refer to the		   **
** shawn-licence.txt file in the root of the Shawn source tree for	   **
** further details.													   **
**                                                                     **
** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
** \author Torsten Teubler <teubler@itm.uni-luebeck.de>			       **
**                                                                     **
************************************************************************/
#ifndef ns_autocastdataunit_h
#define ns_autocastdataunit_h

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"
#include "sys/util/defutils.h"
#include "distribution_area.h"
#include "autocast_app_processor.h"
#include "sys/message.h"
#include "sys/processor.h"
#include <string>
#include <map>
#include <vector>


namespace autocast {

	class AutoCastAppProcessor;
	class AutoCastProcessor;

	//using namespace std;

	/** \brief This class describes the content of an Hovering Data Cloud. 
	 **        It should be derived by different applications that use the 
	 **        HDCtransport layer to add the applications data.            
	 **/
	DECLARE_HANDLES(DataUnit);
	class DataUnit 
		: public shawn::RefcntPointable
	{
		friend class AutoCastAppProcessor;
		//friend class AutoCastProcessor;

	public:
		DataUnit();
		/// Copy constructor
		DataUnit(const DataUnit& o);
		
		/// Constructor
		DataUnit(int id,
				 const shawn::Node* from_addr,
				 /*double x,
				 double y,
				 double time,*/
				 double max_life_time,
				 int priority,
				 std::string responsible_app,
				 const autocast::DistributionArea* area);

		/// Destructor
		virtual ~DataUnit();

		/// Returns an id composed of creation time and fromAddr_
		int id() const throw();

		/// Returns the creation time
		double time() const throw();

		/// Returns x-value of creation 
		double x() const throw();

		/// Returns y-value of creation
		double y() const throw();

		/// Returns the size of this object for use from the message size()
		virtual int size() const throw();

		/// Every application processor has an id to which a DataUnit is associated
		std::string responsible_app() const throw();

		/// Returns the distribution area of the DataUnit
		const autocast::DistributionArea* distribution_area() const throw();

		/// Returns the maximum livetime interval of the DataUnit. If this time expires
		/// the DataUnit runs out of validity.
		double max_life_time() const throw();

		/// The hop count of the DataUnit
		int hop_count() const throw();

		// For incrementing the hop count while sending
		void hop_count(const int) throw();

		/// A priority among the DataUnits - range[0,255]
		int priority() const throw();

		/// The address of the node the DataUnit was created
		const shawn::Node* from_addr() const throw();

	protected:
	
		virtual void size(const int s) throw();
	
	private:
		/// The size of the DataUnit in bytes
		int size_;

		/// Describes the area in that the HDC should be distributed
		const autocast::DistributionArea* area_;

		/// Maximum lifetime (relative to time_).
		/// Set to -1 (default), if the data unit should not be dispatched by the AutoCast layer.
		double max_life_time_;

		/// Maximum HopCount
		/// Set to -1 (default), if the data unit should not be dispatched by the AutoCast layer
		/// Otherwise hopCount is reduced by one every hop
		int hop_count_;

		/// Priority between 0 (rather unimportant) up to 255 (high important)
		int priority_;
	    
		/// the sender's address
		const shawn::Node* from_addr_;

		/// Time and place of creation
		double time_;
		double x_;
		double y_;

		/// The responsible application id
		std::string responsible_app_;
		
		/// The unique id of the DataUnit
		int id_;

		friend int operator<(const DataUnit& lv, const DataUnit& rv);
	};
	/// Less operation by id
	int operator<(const autocast::DataUnit& lv, const autocast::DataUnit& rv);
}
#endif
#endif
