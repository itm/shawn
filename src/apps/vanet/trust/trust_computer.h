/**
 * @file trust_computer.h
 * Defines the interface vanet::TrustComputer for trust models.
 *
 * @author Walter Bamberger
 *
 * $Revision: 590 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/trust/trust_computer.h $
 * $Id: trust_computer.h 590 2011-05-11 11:56:36Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_TRUST_TRUST_COMPUTER_H
#define __SHAWN_APPS_VANET_TRUST_TRUST_COMPUTER_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "../knowledge_base/report.h"
#include <iosfwd>
#include <stdexcept>

namespace vanet
{

/**
 * Interface for trust models and reputation models.
 * 
 * Reputation represents the opinion of the other, while trust represents the
 * own opinion. So a trust model computes the overall trust in a report. It is 
 * invoked, for example, by an information integrator. In contrast, a reputation 
 * model only computes the reputation of the sender with regard to the given report.
 * It is usually invoked only by a trust model. (A simple trust model might be,
 * though, to just return the reputation.)
 */
class TrustComputer
{
public:
   virtual
   ~TrustComputer()
   {
   }

   /**
    * Computes the trust in the report.
    *
    * The first return value is the trust value, i.e. something
    * like a probability that the report is correct. It should be in the range
    * [0, 1]: 0 means completely untrustworthy and 1 means completely
    * trustworthy.
    *
    * The second return value describes the reliability of the trust value
    * (first return value). It should also be in the range [0, 1]: 0 means
    * completely unreliable and 1 means completely reliable. If the trust model
    * does not define such a reliablilty value, this second return value should
    * be derived from the number N of reports included in the trust computation.
    * For example, you could say the reliability
    *
    *     r = 1 - 2 / (N + 2)
    *
    * @param report
    *         the piece of information the trust in which should be computed
    * @return the trust in the given report expressed by a trust value and
    *         a reliability of the trust value
    *
    * @throw None
    * @todo Should be const
    */
   virtual std::pair<float, float>
   computeTrustValue( const Report* report ) = 0;

   /**
    * @throw None
    */
   virtual std::ostream&
   computeCsvRecord( std::ostream& os, const Report* report );

   /**
    * @throw None
    */
   virtual std::string
   getCsvHeader() const;
};

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_TRUST_TRUST_COMPUTER_H */
