/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                **
 ** This part of Shawn is free software; you can redistribute it and/or**
 ** modify it under the terms of the BSD License. Refer to the		   **
 ** shawn-licence.txt file in the root of the Shawn source tree for	   **
 ** further details.												   **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
 ** \author Torsten Teubler <teubler@itm.uni-luebeck.de>			   **
 **                                                                    **
 ************************************************************************/

#ifndef __SHAWN_PROTO_AUTOCASTAPPLICATION_H
#define __SHAWN_PROTO_AUTOCASTAPPLICATION_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

#include "dataunit.h"
#include "sys/node.h"


namespace autocast {

	class DataUnit;
	typedef class shawn::RefcntPointer<const DataUnit> ConstDataUnitHandle;
	typedef class shawn::RefcntPointer<DataUnit> DataUnitHandle;

	class AutoCastApplication{
	public:
		virtual ~AutoCastApplication(){}
		// MUST return true, if the Application would really responsible for the DataUnit !!!
		virtual bool receive_DataUnit( const ConstDataUnitHandle& ) throw() = 0;
		// Refresh the DataUnit
		virtual const autocast::ConstDataUnitHandle& refresh( const ConstDataUnitHandle& ) throw() = 0;

	protected:
		AutoCastApplication()
		{}
	private:
		AutoCastApplication(const AutoCastApplication&);
	};

}
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/

