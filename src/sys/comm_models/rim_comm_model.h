/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_COMM_MODELS_RIM_COMM_MODEL_H
#define __SHAWN_SYS_COMM_MODELS_RIM_COMM_MODEL_H

#include "sys/communication_model.h"
#include "sys/misc/random/basic_random.h"
#include <vector>
#include "sys/misc/random/weibull_random_variable.h"
#include "sys/misc/random/normal_random_variable.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/vec.h"
namespace shawn {


	/** \brief Simulates Radio Irregulation Model in Shawn
	*
	* For further information, read
	* <A HREF ="http://www.cs.virginia.edu/~th7c/Research/Publications/Papers/RAM_V11.pdf">
	* www.cs.virginia.edu/~th7c/ Research/Publications/Papers/RAM_V11.pdf</a>
	*
	* The Radio Irregulation Model (RIM) is a communication model which models variation in 
	* sending power and transmission range depending on the direction of signal propagation.
	* The Disk Graph Model simulates communication by using radial transmission
	* range. But Radio Irregularity is a common phenomenom in wireless networks and so
	* RIM is a much more realistic way of simulating and testing a wireless network
	* environment.<BR>
	* To explain, how the several parameters affect the behavior of the simulation,
	* it is neccessary to give a short introduction in the internals of this model:
	* At time of initialisation, this model generates a sequence of 360 values, called KiSeq_Temp. Each of 
	* them models the transmission range percentage variation change per unit degree change
	* in the direction of radio propagation.<BR>
	* These value are directly influenced by the parameter <CODE>doi</CODE>,
	* which is the maximum percentage variation change per unit degree change in the direction
	* of radio propagation. By increasing the doi-value, the variance in transmission
	* range becomes more irregluar. All these values are chosen randomly by a WeibullRandomVariable. 
	* This variable can be affected by the parameters <CODE>alpha</CODE> and
	* <CODE>beta</CODE>. Furthermore, all values in the sequence are assured to be in the intervall from
	* <CODE>min_range</CODE> to <CODE>max_range</CODE>, comprehended as minimum / maximum value of percentage variation
	* in transmission range. If doi is set to zero, the RIM Model behaves like Disk Graph Model 
	* with transmission range <CODE>range</CODE>.<BR>
	* After generating the temporary KiSeq, <CODE>granularity</CODE> values are chosen and stored to the final <CODE>KiSeq_</CODE>.
	* The parameters <CODE>variance</CODE> and <CODE>mean</CODE> affect the behavior of a NormalRandomVariable.
	* This Random Variable affects the maximum percentage variance of the signal sending power
	* for this model (Parameter <CODE>vsp</CODE>). The final vsp_ is set during construction
	* of the RIM Model.<BR>
	* Beware, each node in the simulation communicates with the same final vsp value and uses the same
	* final KiSeq_. There is no difference between different nodes.<BR>
	* <BR>
	* To use the the RIM in Shawn, task <TT>prepare_world</TT> gets value <TT>"rimm_comm"</TT> for the id <TT>"comm\_model"</TT>.
	* All of the parameters are optional:<BR>
	* <OL>
	* <LI><TT>double alpha</TT> (optional) (default: 3.0)</LI>
	* <LI><TT>double beta</TT> (optional) (default: 5.0)</LI>
	* <LI><TT>double mean</TT> (optional) (default: 0.0)</LI>
	* <LI><TT>double variance</TT> (optional) (default: 0.0)</LI>
	* <LI><TT>double range</TT> (optional) (default: 1.0)</LI>
	* <LI><TT>double min_range</TT> (optional) (default: 0.5)</LI>
	* <LI><TT>double max_range</TT> (optional) (default: 1.5)</LI>
	* <LI><TT>double doi</TT> (optional) (default: 0.0)<LI>
	* <LI><TT>int granularity</TT> (optional) (default: 360)</LI>
	* <LI><TT>double vsp</TT> (optional) (default: 0)</LI>
	* </OL>
	*/
   class RimCommModel
	 : public CommunicationModel
   {
   public:


      /** \brief Constructor
	  *
	  *  @param alpha Parameter for Weibull Random Variable
	  *  @param beta Parameter for Weibull Random Variable
	  *  @param mean Parameter for Normal Random Variable
	  *  @param variance Parameter for Normal Random Variable
	  *  @param range Transmission range if doi = 0
	  *  @param min_range Minimum percentage variation in transmission range
	  *  @param max_range Maximum percentage variation in transmission range
	  *  @param doi Degree of Radio Irregularity
	  *  @param granularity Amount of values picked out of 360
	  *  @param vsp Maximum percentage variation of signal sending power
	  *  @param epsilon Determines how much the average Ki value may diverge from 1
	  *  @param rot Rotation
	  *  @param sequence_count
	  */
	   RimCommModel(double alpha, double beta, double mean, double variance,
	            double range, double min_range, double max_range, double doi,
	            int granularity, double vsp, double epsilon, int rot,
	            int sequence_count);
      ~RimCommModel();


	  /** \brief Sets doi
	  */
      virtual void set_doi(double) throw();

	  /** \brief Sets Granularity
	  */
	  virtual void set_granu(int) throw();



     virtual bool
      can_communicate_bidi( const Node&, const Node& ) const throw();

     virtual bool
      can_communicate_uni( Vec u, int uid, Vec v ) const throw();

	 		

	 virtual double angle( Vec u, Vec v ) const throw();
	 virtual bool
      can_communicate_uni( const Node&,
                           const Node& )
         const throw();

	  /// returns whether communication_upper_bound() returns a useful value
      virtual bool
      exists_communication_upper_bound( void )
         const throw();

	  /** if exists_communication_upper_bound(), nodes whose euclidean
       *  distance exceeds communication_upper_bound() can never communicate
       *  in any direction
       */
       /// returns whether communication_lower_bound() returns a useful value
      virtual bool
      exists_communication_lower_bound( void )
         const throw();

	  /** if exists_communication_lower_bound(), nodes whose euclidean
       *  distance exceeds communication_lower_bound() can always communicate
       *  in any direction
       */
      virtual double
      communication_upper_bound( void )
         const throw();

	  /** \brief Returns lower bound.
	  */
      virtual double
      communication_lower_bound( void )
         const throw();

      virtual bool
      is_status_available_on_construction( void )
         const throw();

      virtual int get_granularity( void ) const throw();
      virtual double get_range( void );

	  

	/** \brief Adds a tag named "ki_seq" to World which stores the entries of KiSeq
	* 
	* Used in basic_tasks.cpp
	* The RIM Model provides the functionality of storing the values of the final <TT>KiSeq_</TT>
	* as a tag in an XML-File by running the task <TT>save_world</TT>. The tag 
	* is a integer-double-map-tag and stores the entry index i and the value
	* of percentage radio irregulation.
	* If an existing world is loaded and the RIM Model is set, saved values of a <TT>KiSeq_</TT> are loaded
	* and used during simulation.
	*/
	  virtual void save_to_tag(SimulationEnvironment& se) throw();
      virtual bool load_from_tag(const SimulationEnvironment& se) throw();
	  void save_to_file();

	  void generate_sequences();
	  virtual std::vector<double> get_Ki(int sequence_index);

	  /** \brief Returs K(i) value for a non integer angle.
	  */
      virtual double interpolate_Ki(double, int)  const throw();

	  // Generates Gnuplot - Outout of the Ki value
	  //virtual void print()  throw();
   private:

      WeibullRandomVariable wrv_;
      NormalRandomVariable nrv_;

	 /** \brief Transmission range for doi = 0
	 */
     double range_;

	 /** \brief Minimum percentage variation in transmission range
	 */
     double min_range_;

	 /** \brief Maximum percentage variation in transmission range
	 */
     double max_range_;

	 /** \brief Degree of Radio Irregularity (0<= doi_ <= 1)
	 *
	 *  The maximum range variation per unit degree change in the direction of radio
	 *  propagation
	 * 
	 */
     double doi_;

	 /** \brief Amount of values out of 360 (360 mod granularity_ has to be 0)
	 */
     int granularity_;
	 /** \brief VSP- adjustet sending power 
	 */
	 double vsp_;
	 /** \brief determines how much the average Ki value may diverge from 1
	 */
	 double epsilon_;

	 int rotation_;

	 int sequence_count_;

     
	 //std::vector<double> KiSeq_;
	 std::vector<std::vector<double> > sequences_;

	  /** \brief Returns -1 in 50% of the cases, and 1 otherwise	
	  */
      int sign();

	  /** \brief Returs a single vector of 360 transmission ranges.
	  */
      virtual std::vector<double> generate_Ki_seq(double)throw();

	  double frac(double d );

   };

}//namepsace shawn

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/rim_comm_model.h,v $
 * Version $Revision: 504 $
 * Date    $Date: 2011-01-24 20:00:04 +0100 (Mon, 24 Jan 2011) $
 *-----------------------------------------------------------------------
 * $Log: rim_comm_model.h,v $
 *-----------------------------------------------------------------------*/
