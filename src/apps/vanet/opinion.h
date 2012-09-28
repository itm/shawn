/**
 * @file opinion.h
 * Implements the opinion of subjective logic.
 *
 * @author Walter Bamberger
 *
 * $Revision: 592 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/opinion.h $
 * $Id: opinion.h 592 2011-05-11 12:11:17Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_OPINION_H
#define __SHAWN_APPS_VANET_OPINION_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include <algorithm>
#include <ostream>
#include <numeric>
#include <cfloat>
#include <cstring>
#include <stdexcept>

namespace vanet
{

/**
 * @class Opinion
 *
 * Implements the opinion of subjective logic and the operations on it
 * (see Jøsang, 2002).
 *
 * In subjective logic, an opinion describes the subjective view of an
 * individual about a certain property of the world. In court, for example,
 * some evidence might support the accusation and some evidence might contradict
 * the accusation. And still, there might some lack of knowledge about the case.
 * Subjective logic describes a mathematical theory to deal with the available
 * evidence and the subjectivity behind it. It is related to the theory of
 * evidence of Dempster and Shafer.
 *
 * Below you find a reference section. It contains a complete specification of
 * all scientific references used in the documentation of this class. A good
 * introduction to subjective logic is Jøsang, 2001. You should start with this
 * article to understand the concepts of this class. While this paper only deals
 * with binomial opinions (<tt>size_ = 2</tt>), this class can handle
 * multinomial opinions as well. The mathematics of multinomial opinions is
 * described in the other papers of the reference section.
 *
 * This class represents a general multinomial opinion. It implements it as a
 * template; its only template parameter @c size_ sets the number of possible
 * states. This template implementation is very memory and time efficient, while
 * still elegant and easy to use. So for example, Opinion<2> is a binomial
 * opinion; such an opinion just describes the agreement and disagreement with
 * a given proposition. It is widely used, so it got the dedicated type
 * definition BinomialOpinion. (A binomial opinion is a cluster Dirichlet bba
 * with two clusters.)
 *
 * @par References
 * A. Jøsang. A logic for uncertain probabilities. In <em>International Journal
 * of Uncertainty, Fuzziness and Knowledge-Based Systems</em>, 9(3),
 * pp. 279–311, June 2001. <a href="http://dx.doi.org/10.1142/S0218488501000831"
 * title="A logic for uncertain probabilities">doi:10.1142/S0218488501000831</a>.
 * @par
 * A. Jøsang. Probabilistic logic under uncertainty. In J. Gudmundsson and
 * B. Jay (eds.), <em>Thirteenth Computing: The Australasian Theory Symposium
 * (CATS2007)</em>, volume 65 of CRPIT, pp. 101–110. Australian Computer
 * Society, Inc., Darlinghurst, Australia, 2007. ISBN 1-920-68246-5.
 * <a href="http://crpit.com/confpapers/CRPITV65Josang.pdf" title="Probabilistic
 * logic under uncertainty">http://crpit.com/confpapers/CRPITV65Josang.pdf</a>.
 * @par
 * A. Jøsang, S. Marsh, and S. Pope. Exploring different types of trust
 * propagation. In <em>Trust Management. 4th International Conference, iTrust
 * 2006, Pisa, Italy, May 16-19, 2006. Proceedings</em>, pp. 179–192. Springer,
 * Berlin, 2006. <a href="http://dx.doi.org/10.1007/11755593_14" title=
 * "Exploring different types of trust propagation">doi:10.1007/11755593_14</a>.
 */
template<std::size_t size_ = 2>
   class Opinion
   {
   private:
      // For floating point computations with correct types
      static const float sizef_ = size_;

   public:
      /// @name Types
      /// @{

      typedef float* EvidenceIterator;
      typedef const float* ConstEvidenceIterator;
      typedef const float* ConstBaseRateIterator;

      /// @}
      /// @name Construction
      /// @{

      Opinion() throw ()
      {
         std::memset(evidence_, 0, sizeof(evidence_));
         std::fill(baseRate_, baseRate_ + size_ - 1, 1.0f / sizef_);
      }

      /**
       * Creates an opinion with the given evidence and an equally distributed
       * base rate.
       */
      static Opinion<size_>
      fromEvidence( const float evidence[size_] ) throw ()
      {
         return Opinion<size_> (evidence);
      }

      /**
       * Creates an opinion with the given evidence and base rate.
       */
      static Opinion<size_>
      fromEvidence( const float evidence[size_],
               const float baseRate[size_ - 1] ) throw ()
      {
         return Opinion<size_> (evidence, baseRate);
      }

      /**
       * Create an opinion with the given probability distribution and the
       * amount of evidence N.
       *
       * An equally distributed base rate is contructed for the result.
       */
      static Opinion<size_>
      fromProbabilitiesN( const float probabilities[size_ - 1], float N )
      {
         // The output value can be negative, because of rounding errors in
         // the difference. To avoid (impossible) negative evidence,
         // I check for that throughout this method.

         float sumEvidencePlusC = N + size_;
         float evidence[size_];

         const float* in = probabilities;
         float* out = evidence;

         // All but the last element in evidence
         for ( ; out != evidence + size_ - 1; ++in, ++out )
         {
            *out = sumEvidencePlusC * *in - 1; // -1, because a = 1/size_ here
            *out = *out < 0 ? 0.0f : *out;
         }

         // The last element in evidence
         *out = N - std::accumulate(evidence, evidence + size_ - 1, 0.0f);
         *out = *out < 0 ? 0.0f : *out;

         return Opinion<size_> (evidence);
      }

      /// @}
      /// @name Access to the different representations of the opinion
      /// @{

      float
      belief( std::size_t i ) const throw ( std::out_of_range )
      {
         if ( i >= 0 && i < size_ )
            return evidence_[i] / (std::accumulate(evidence_,
                     evidence_ + size_, 0) + sizef_);
         else
            throw(std::out_of_range("Index out of range in Opinion::belief"));
      }

      /**
       * Returns the degree of the lack of knowledge.
       *
       * Jøsang often speaks of the uncertainty or uncertainty mass (letter u).
       * This is a bit confusing, since probabilities are usually said to express
       * the uncertainty of an event.  The uncertainty in Subjective Logic
       * expresses the lack of knowledge, i.e. the ignorance.
       *
       * @return the ignorance (u)
       *
       * @throw None
       */
      float
      ignorance() const
      {
         return sizef_ / (std::accumulate(evidence_, evidence_ + size_, 0.0f)
                  + sizef_);
      }

      float
      baseRate( std::size_t i ) const throw ( std::out_of_range )
      {
         if ( i >= 0 && i < size_ - 1 )
            return baseRate_[i];

         else if ( i == size_ - 1 )
         {
            float baseRate = 1.0f - std::accumulate(baseRate_,
                     baseRate_ + size_ - 1, 0.0f);
            // Because of rounding errors, the base rate could be negative here.
            // It must be at least 0.0 though.
            return baseRate < 0.0f ? 0.0f : baseRate;
         }
         else
            throw(std::out_of_range("Index out of range in Opinion::baseRate"));
      }

      ConstBaseRateIterator
      beginBaseRate() const throw ()
      {
         return baseRate_;
      }

      ConstBaseRateIterator
      endBaseRate() const throw ()
      {
         return baseRate_ + size_ - 1;
      }

      float
      probabilityExpectation( std::size_t i ) const throw ( std::out_of_range )
      {
         if ( i >= 0 && i < size_ )
            return (evidence_[i] + sizef_ * baseRate(i)) / (std::accumulate(
                     evidence_, evidence_ + size_, 0.0f) + sizef_);
         else
            throw(std::out_of_range(
                     "Index out of range in Opinion::probabilityExpectation"));
      }

      float
      evidence( std::size_t i ) const throw ( std::out_of_range )
      {
         if ( i >= 0 && i < size_ )
            return evidence_[i];
         else
            throw(std::out_of_range("Index out of range in Opinion::evidence"));
      }

      EvidenceIterator
      beginEvidence() throw ()
      {
         return evidence_;
      }

      ConstEvidenceIterator
      beginEvidence() const throw ()
      {
         return evidence_;
      }

      EvidenceIterator
      endEvidence() throw ()
      {
         return evidence_ + size_;
      }

      ConstEvidenceIterator
      endEvidence() const throw ()
      {
         return evidence_ + size_;
      }

      static std::size_t
      size() throw ()
      {
         return size_;
      }

      /// @}
      /// @name Modification of the evidence
      /// @{

      void
      addEvidence( std::size_t i, const float& additionalEvidence )
               throw ( std::out_of_range )
      {
         if ( i >= 0 && i < size_ )
            evidence_[i] += additionalEvidence;
         else
            throw(std::out_of_range(
                     "Index out of range in Opinion::addEvidence"));
      }

      void
      setEvidence( std::size_t i, const float& newEvidence )
               throw ( std::out_of_range )
      {
         if ( i >= 0 && i < size_ )
            evidence_[i] = newEvidence;
         else
            throw(std::out_of_range(
                     "Index out of range in Opinion::setEvidence"));
      }

      void
      setBaseRate( const float baseRate[size_ - 1] )
      {
         std::memcpy(baseRate_, baseRate, sizeof(baseRate_));
      }

      /// @}
      /// @name Subjective logic operations
      /// @{

      /**
       * Fuses this object with the given opinion using cumulative fusion.
       * Cumulative fusion is used for independent opinions. So this object and
       * the given opinion should be based on independent evidence.
       *
       * This method also fuses both base rate vectors with the algorithm
       * discussed in Jøsang et al., 2006. This is in contrast to
       * <tt>cumulativeFusion(Opinion<size_>&, float[size_-1])</tt>,
       * where you can give your own base rate for the fused opinion. Consider
       * what is right for your needs.
       *
       * See Jøsang, 2007 and Jøsang et al., 2006. I extended the computation
       * of the base rate to the multinomial case.

       * @param o the opinion to fuse with
       * @return the fused opinion
       * @throw None
       */
      Opinion<size_>
      cumulativeFusion( const Opinion<size_>& o ) const
      {
         float newBaseRate[size_ - 1];

         const float in1Sum = std::accumulate(evidence_, evidence_ + size_,
                  0.0f);
         const float in2Sum = std::accumulate(o.evidence_, o.evidence_ + size_,
                  0.0f);
         const float totalSum = in1Sum + in2Sum;

         if ( totalSum > FLT_MIN * 1000.0f )
         {
            const float* in1 = baseRate_;
            const float* in2 = o.baseRate_;
            float* out = newBaseRate;
            for ( ; out != newBaseRate + size_ - 1; ++out, ++in1, ++in2 )
               *out = (*in1 * in1Sum + *in2 * in2Sum) / totalSum;
         }
         else
         {
            std::fill(newBaseRate, newBaseRate + size_ - 1, 1.0f / sizef_);
         }

         return cumulativeFusion(o, newBaseRate);
      }

      /**
       * Fuses this object with the given opinion using cumulative fusion.
       * Cumulative fusion is used for independent opinions. So this object and
       * the given opinion should be based on independent evidence.
       *
       * The base rate may be considered as an individual view of each agent.
       * Then, it must be provided by the agent and cannot be computed as above.
       * So this method only fuses the evidence but not the base rate. The new
       * opinion has the given base rate instead.
       *
       * See Jøsang, 2007
       *
       * @param o the opinion to fuse with
       * @param baseRate the base rate of the returned opinion
       * @return the fused opinion
       * @throw None
       */
      Opinion<size_>
      cumulativeFusion( const Opinion<size_>& o,
               const float baseRate[size_ - 1] ) const
      {
         float evidence[size_];

         const float* in1 = evidence_;
         const float* in2 = o.evidence_;
         float* out = evidence;
         for ( ; out != evidence + size_; ++out, ++in1, ++in2 )
            *out = *in1 + *in2;

         return Opinion<size_> (evidence, baseRate);
      }

      /**
       * Adds an opinion to this object using cumulative fusion.
       * Cumulative fusion is used for independent opinions. So this object and
       * the given opinion should be based on independent evidence. The result
       * in directly stored in this object. No additional opinion is created.
       *
       * This method also fuses both base rate vectors with the method discussed
       * in Jøsang et al., 2006. Consider whether this is right for your needs.
       *
       * See Jøsang, 2007 and Jøsang et al., 2006. I extended the computation
       * of the base rate to the multinomial case.
       *
       * @param o the opinion to fuse with
       * @throw None
       */
      void
      cumulativeFusionToSelf( const Opinion<size_>& o )
      {
         /*
          *  Compute the new base rate vector.
          */
         const float selfSum = std::accumulate(evidence_, evidence_ + size_,
                  0.0f);
         const float inSum = std::accumulate(o.evidence_, o.evidence_ + size_,
                  0.0f);
         const float totalSum = selfSum + inSum;
         // Compute the new base rate vector only if numerically reasonable.
         if ( totalSum > FLT_MIN * 1000.0f )
         {
            float* selfBR = baseRate_;
            const float* inBR = o.baseRate_;
            for ( ; selfBR != baseRate_ + size_ - 1; ++selfBR, ++inBR )
               *selfBR = (*selfBR * selfSum + *inBR * inSum) / totalSum;
         }

         /*
          * Compute the new evidence vector.
          */
         float* selfE = evidence_;
         const float* inE = o.evidence_;
         for ( ; selfE != evidence_ + size_; ++selfE, ++inE )
            *selfE += *inE;
      }

      /**
       * Fusion of dependent opinions with fused base rate.
       *
       * See Jøsang, 2007 and Jøsang et al., 2006. I extended
       * the computation of the base rate for the multinomial case.
       */
      Opinion<size_>
      averagingFusion( const Opinion<size_>& o ) const throw ()
      {
         float newBaseRate[size_ - 1];

         const float* in1 = baseRate_;
         const float* in2 = o.baseRate_;
         float* out = newBaseRate;
         for ( ; out != newBaseRate + size_ - 1; ++out, ++in1, ++in2 )
            *out = 0.5f * (*in1 + *in2);

         return averagingFusion(o, newBaseRate);
      }

      /**
       * Fusion of dependent opinions with given base rate.
       *
       * The base rate may be considered as an individual value of each agent.
       * Then, it must be provided by the agent and cannot be computed as above.
       *
       * See Jøsang, 2007
       */
      Opinion<size_>
      averagingFusion( const Opinion<size_>& o, const float a[size_ - 1] ) const throw ()
      {
         float evidence[size_];

         const float* in1 = evidence_;
         const float* in2 = o.evidence_;
         float* out = evidence;
         for ( ; out != evidence + size_; ++out, ++in1, ++in2 )
            *out = 0.5 * (*in1 + *in2);

         return Opinion<size_> (evidence, a);
      }

      /**
       * Fuses a prioritised opinion (this) with a discriminated opinion
       * (argument). This method implements the evidence-based priority fusion.
       * It has been introduced in Bamberger et al., 2011 and is different
       * from the priority fusion in Bamberger et al., 2010. See the Note 1
       * in Bamberger et al., 2011 (on the last page).
       *
       * The base rate of the fused opinion is the base rate of the prioritised
       * opinion.
       *
       * @param discriminatedOpinion
       *              The opinion that should have the lower priority in the
       *              fusion
       * @return The fused opinion
       *
       * @throw None
       */
      Opinion<size_>
      priorityFusion( const Opinion<size_>& discriminatedOpinion ) const
      {
         float evidence[size_];
         float in1Ignorance = ignorance();

         const float* in1 = evidence_;
         const float* in2 = discriminatedOpinion.evidence_;
         float* out = evidence;
         for ( ; out != evidence + size_; ++out, ++in1, ++in2 )
            *out = *in1 + in1Ignorance * *in2;

         return Opinion<size_> (evidence, baseRate_);
      }

      /**
       * Discounts the opinion with the the argument using uncertainty favouring
       * discounting. This is one of the operations usually used to implement
       * the transitivity property of trust. It is described in
       * Jøsang et al., 2006. I extended the algorithm to the multinomial case.
       *
       * Here, this object is the opinion that will be discounted
       * (the recommendation x). The argument is the discounting
       * opinion (the trust in the recommender B).
       *
       * @param discountingOpinion the binary opinion that discounts
       * @return the discounted opinion
       * @throw None
       */
      Opinion<size_>
      uncertaintyFavouringDiscountingBy( const Opinion<2u>& discountingOpinion ) const
      {
         return beliefDiscountingBy(discountingOpinion.belief(0));
      }

      /**
       * Discounts the opinion with the argument using base rate sensitive
       * discounting. This is one of the operations usually used to implement
       * the transitivity property of trust. It is described in
       * Jøsang et al., 2006. I extended the algorithm to the multinomial case.
       *
       * Here, this object is the opinion that will be discounted
       * (the recommendation x). The argument is the discounting
       * opinion (the trust in the recommender B).
       *
       * @param discountingOpinion the binary opinion that discounts
       * @return the discounted opinion
       * @throw None
       */
      Opinion<size_>
      baseRateSensitiveDiscountingBy( const Opinion<2u>& discountingOpinion ) const
      {
         return beliefDiscountingBy(
                  discountingOpinion.probabilityExpectation(0));
      }

      /**
       * Discounts the belief of this object with the given factor. This method
       * is related to the uncertainty favouring discounting and the base rate
       * sensitive discounting in Jøsang et al., 2006.  Both methods differ only
       * in what factor is chosen for discounting. I extended the method to the
       * multinomial case.
       *
       * @param discountingFactor the factor to discount this opinion with
       *                          (must be >= 0 and <= 1)
       * @return the discounted opinion
       * @throw None
       */
      Opinion<size_>
      beliefDiscountingBy( float discountingFactor ) const
      {
         /*
          * = Multinomial discounting in belief representation (LaTeX code) =
          *
          *   b^{A:B}(x_i) = b^A_B * b^B(x_i)
          *        u^{A:B} = 1 - b^A_B * (1 - u^B_x)
          *        a^{A:B} = a^B
          *
          * This is how Jøsang describes it. Note that only one factor of the
          * trust opinion is necessary. I call it the discounting factor.
          *
          * This implementation of an opinion works in evidence representation.
          * So an algorithm is necessary that does the above computations with
          * the evidence vectors. This is described next and then implemented
          * below.
          *
          * = Multinomial discounting in evidence representation (LaTeX code) =
          *
          * From the uncertainty of the discounted opinion, the sum S^{A:B} of
          * the discounted evidence can be derived:
          *
          *   S^{A:B} + C = \frac{C}{S^B * (1 - b^A_B) + C} * (S^B + C) .
          *
          * This gives us the relation
          *
          *   R = \frac{S^{A:B} + C}{S^B + C}
          *
          * between input evidence and the discounted evidence.
          *
          * From the first belief equation, the discounting of the evidence
          * values can be derived now:
          *
          *    \frac{r^{A:B}(x_i)}{S^{A:B} + C} =
          *        b^A_B * \frac{r^B(x_i)}{S^B + C} .
          *
          * When resolving for the discounted evidence value, the computation
          * of the belief discounting in evidence representation results in
          *
          *    r^{A:B}(x_i) = b^A_B * r^B(x_i) * R .
          *
          * This is implemented below.
          */

         float sumOfEvidence = std::accumulate(evidence_, evidence_ + size_, 0);
         float evidenceRelation = sizef_ / (sumOfEvidence * (1.0f
                  - discountingFactor) + sizef_);

         float scalingFactor = discountingFactor * evidenceRelation;
         float outEvidence[size_];
         std::transform(evidence_, evidence_ + size_, outEvidence,
                  std::bind2nd(std::multiplies<float>(), scalingFactor));

         return Opinion<size_>::fromEvidence(outEvidence, baseRate_);
      }

      /**
       * Discounts the evidence of this object with the argument.
       *
       * This is another operation for the transitivity property of trust
       * based on evidence considerations.
       *
       * @param discountingOpinion the binary opinion that discounts
       * @param baseRate the base rate of the result
       * @throw None
       */
      Opinion<size_>
      evidenceDiscountingBy( const Opinion<2u>& discountingOpinion,
               const float baseRate[size_ - 1] ) const
      {
         float factor = discountingOpinion.probabilityExpectation(0);
         float newEvidence[size_];
         std::transform(evidence_, evidence_ + size_, newEvidence,
                  std::bind2nd(std::multiplies<float>(), factor));
         return Opinion<size_> (newEvidence, baseRate);
      }

      /**
       * Scales the uncertainty by scaling the evidence vector.
       *
       * @param factor the scaling factor
       * @return the scaled opinion
       * @throw None
       */
      Opinion<size_>
      evidenceScaling( float factor ) const
      {
         float newEvidence[size_];
         std::transform(evidence_, evidence_ + size_, newEvidence,
                  std::bind2nd(std::multiplies<float>(), factor));
         return Opinion<size_> (newEvidence, baseRate_);
      }

      /// @}
      /// @name C++ operators
      /// @{

      bool
      operator==( const Opinion<size_>& o ) const throw ()
      {
         return std::equal(evidence_, evidence_ + size_, o.evidence_)
                  && std::equal(baseRate_, baseRate_ + size_ - 1, o.baseRate_);
      }

      bool
      operator!=( const Opinion<size_>& o ) const throw ()
      {
         return !operator==(o);
      }

      Opinion<size_>&
      operator=( const Opinion<size_>& o ) throw ()
      {
         // Check for self-assignment
         if ( this != &o )
         {
            std::memcpy(evidence_, o.evidence_, sizeof(evidence_));
            std::memcpy(baseRate_, o.baseRate_, sizeof(baseRate_));
         }
         return *this;
      }

      friend std::ostream&
      operator<<( std::ostream& os, const Opinion<size_>& o ) throw ()
      {
         os << "((";
         const float* p = o.evidence_;
         for ( ; p != o.evidence_ + size_ - 1; ++p )
            os << *p << ",";
         os << *p << "),(";
         for ( p = o.baseRate_; p != o.baseRate_ + size_ - 2; ++p )
            os << *p << ",";
         os << *p << "))";
         return os;
      }

      /// @}

   private:
      float evidence_[size_];
      float baseRate_[size_ - 1];

      Opinion( const float evidence[size_] ) throw ()
      {
         std::memcpy(evidence_, evidence, sizeof(evidence_));
         std::fill(baseRate_, baseRate_ + size_ - 1, 1.0f / sizef_);
      }

      Opinion( const float evidence[size_], const float baseRate[size_] ) throw ()
      {
         std::memcpy(evidence_, evidence, sizeof(evidence_));
         std::memcpy(baseRate_, baseRate, sizeof(baseRate_));
      }
   };

typedef Opinion<2u> BinaryOpinion;
typedef Opinion<5u> IntOpinion;

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_OPINION_H */
