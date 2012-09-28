/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_READING_RANDOM_PROCESSOR_FACTORY_H
#define __SHAWN_APPS_READING_RANDOM_PROCESSOR_FACTORY_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include "sys/processors/processor_factory.h"


namespace shawn
{

   class SimulationController;

}// namespace shawn

namespace reading
{

   /// Factory that produces \ref LocalizationProcessor
   /// "localization processors"
   /** This class implements the superclass ProcessorFactory.
    */
   class RandomDoubleTestProcessorFactory
      : public shawn::ProcessorFactory
   {

   public:
      ///@name construction / destruction
      ///@{
      ///
      RandomDoubleTestProcessorFactory( void );
      ///
      virtual ~RandomDoubleTestProcessorFactory();
      ///@}


      ///@name basic methods inherited from ProcessorFactory
      ///@{
      /** \return used name to be indentified by ProcessorKeeper
       *  \sa ProcessorFactory::name()
       */
      virtual std::string name( void ) const throw();
      /** \return short description about functionality
       *  \sa ProcessorFactory::description()
       */
      virtual std::string description( void ) const throw();
      /** \return new LocalizationProcessor
       *  \sa ProcessorFactory::create()
       */
      virtual shawn::Processor* create( void ) throw();
      ///@}


      /** This method registers this factory to the ProcessorKeeper.
       */
      static void register_factory( shawn::SimulationController& ) throw();

   };


}
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/localization_processor_factory.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Di, 29. Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_processor_factory.h,v $
 *-----------------------------------------------------------------------*/
