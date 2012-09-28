/**
 * @file trust_bamberger_2010.h
 * Defines the trust model class vanet::TrustBamberger2010.
 *
 * @author Walter Bamberger
 *
 * $Revision: 589 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/trust/trust_bamberger_2010.h $
 * $Id: trust_bamberger_2010.h 589 2011-05-11 11:52:37Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_TRUST_TRUST_BAMBERGER_2010_H
#define __SHAWN_APPS_VANET_TRUST_TRUST_BAMBERGER_2010_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "trust_computer.h"
#include "../opinion.h"
#include "sys/misc/tokenize.h"
#include <iosfwd>
#include <stdexcept>
#include <string>
#include <vector>

namespace shawn
{
class SimulationController;
}

namespace vanet
{

class EvidenceIntegrator;
class IntReport;
class KnowledgeBase;
class Report;

/**
 * Trust model following the paper of Bamberger et al. (2010) but already goes
 * beyond it.
 */
class TrustBamberger2010 : public TrustComputer
{
public:
   TrustBamberger2010( const EvidenceIntegrator& integrator,
            const KnowledgeBase& kb, const std::string& vehicleLabel,
            const shawn::SimulationController& sc );

   virtual
   ~TrustBamberger2010();

   /**
    * @throw None
    */
   virtual std::ostream&
   computeCsvRecord( std::ostream& os, const Report* report );

   /**
    * @throw None
    */
   virtual std::pair<float, float>
   computeTrustValue( const Report* report );

   /**
    * @throw None
    */
   virtual std::string
   getCsvHeader() const;

private:
   struct ComputationParameters;

   const EvidenceIntegrator& integrator_;
   const KnowledgeBase& knowledgeBase_;
   std::ostream* logStream_;
   TrustComputer* reputationComputer_;
   const shawn::SimulationController& simulationController_;
   const bool withPredictability_;

   BinaryOpinion
   combineTrustInTypes( const Report* reportToTrust,
            const std::vector<ComputationParameters>& parameterSet ) const;

   /**
    * Computes the trust value. This method combines the functionality of
    * #computeTrustValue and #computeCsvRecord. Both methods must compute the
    * trust value and @c computeCsvRecord must in addition output some data
    * for statistics. So the majority of both methods is the same. That is why
    * their implementation has been moved in this method. The CSV output needs
    * just a couple of additional lines. They are only executed if @c csvStream
    * is not 0.
    *
    * This method walks through the pieces of evidence, the reports. It
    * delegates the processing of each report to #updateTrustInType, but
    * performs the logging output on its own.
    */
   std::pair<float, float>
   computeTrustValueAndCsv( const Report* reportPtr, std::ostream* csvStream );

   std::pair<bool, IntOpinion>
            makeDecision(
                     std::vector<std::pair<float, IntOpinion> > opinionList ) throw ();

   inline void
   updateCompetence( const IntReport* r, ComputationParameters& ps ) throw ();

   void
            updatePredictability( const IntReport* r, ComputationParameters& ps ) throw ();

   /**
    * Processes one report in the trust computation. To do so, it calls
    * #updateCompetence and #updatePredictability appropriately.
    */
   void
   updateTrustInType( const IntReport* r, ComputationParameters& ps ) throw ();

   inline float
   trustToTrustValue( const BinaryOpinion& trust ) throw ();
};

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_TRUST_TRUST_BAMBERGER_2010_H */
