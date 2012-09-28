/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_SIMPLE_READING_H
#define __SHAWN_APPS_SIMPLE_READING_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/readings/reading.h"
#include "sys/misc/box.h"

namespace reading
{
    ///  Integer Reading
    /**  This class is derived form the superclass Reading.
    * It contains Readings with integer values.
    */
    class IntegerReading
        : public Reading
    {
    public:
    ///@name construction / destruction
	 ///@{
       IntegerReading();
       virtual ~IntegerReading(){}
    ///@}
       virtual int value( const shawn::Vec& v ) const throw() = 0;
    };
   DECLARE_HANDLES(IntegerReading);

    //---------------------------------------------------------------------
    ///  Double Reading
    /**  This class is derived form the superclass Reading.
    * It contains Readings with double values.
    */
    class DoubleReading
        : public Reading
    {
    public:
        ///@name construction / destruction
	///@{
      DoubleReading();
      ///
      virtual ~DoubleReading(){}

      virtual double value( const shawn::Vec& v ) const throw() = 0;
	///@}
    };
   DECLARE_HANDLES(DoubleReading);

    //---------------------------------------------------------------------
    ///  String Reading
    /**  This class is derived form the superclass Reading.
    * It contains Readings with string values.
    */
    class StringReading
        : public Reading
    {
    public:
       	///@name construction / destruction
	///@{
       StringReading();
	///
       virtual ~StringReading(){}
       ///@}
       virtual std::string value( const shawn::Vec& v ) const throw() = 0;
    };
   DECLARE_HANDLES(StringReading);

    //---------------------------------------------------------------------
    ///  Bool Reading
    /**  This class is derived form the superclass Reading.
    * It contains Readings with bool values.
    */
    class BoolReading
        : public Reading
    {
    public:
       	///@name construction / destruction
	///@{
       BoolReading();
       ///
       virtual ~BoolReading(){}
       ///@}
       virtual bool value( const shawn::Vec& v ) const throw() = 0;
    };
   DECLARE_HANDLES(BoolReading);

}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/readings/simple_reading.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Di, 29. Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: simple_reading.h,v $
 *-----------------------------------------------------------------------*/

