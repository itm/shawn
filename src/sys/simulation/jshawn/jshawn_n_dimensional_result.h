/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_SIMULATION_JSHAWN_JSHAWN_N_DIMENSIONAL_RESULT
#define __SHAWN_SYS_SIMULATION_JSHAWN_JSHAWN_N_DIMENSIONAL_RESULT

#include "sys/simulation/simulation_task.h"
#include "sys/misc/number_formatter.h"


#include <stdexcept>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>


namespace shawn {

	//----------------------------------------------------------------------------
	/**
	*
	*
	*/
	template<typename Type, typename Index>
	class JShawnNDimensionalResult : public SimulationTaskResult {
	private:
		///
		typedef std::map<Index, JShawnNDimensionalResult<Type, Index> > MapType;
		///
		typedef typename MapType::iterator MapIt;

		///
		std::string axis_label_;
		///
		std::string description_;

		///
		bool is_value_;
		///
		Type value_;
		///
		MapType values_;



		//------------------------------------------------------------------------
		/**
		* 
		*/
		virtual std::vector<std::string> to_table_string_helper(std::string seperator) {
			std::vector<std::string> result_string_vector;//=new std::vector<std::string>();
			std::stringstream ss;	

			if (is_value_ == true) {	
				ss << DoubleFormatter(value_);
				std::string result=ss.str();//value_.toString();
				ss.str("");
				result_string_vector.push_back(result);
			} 
		
			else { //getting into "<index....> level" see xml
			
				int mapsize=values_.size();

				MapIt entry;
				for (entry=values_.begin();entry!=values_.end();++entry) {
					Type key=entry->first;
					ss << DoubleFormatter(key);
					std::string key_string=ss.str();//value_.toString();
					//std::cerr<<key_string<<std::endl;
					ss.str("");
					JShawnNDimensionalResult value=entry->second;
					//recursion
					//receive a stringvector where the first String is always the columnnames
					std::vector<std::string> temp_string_vector=value.to_table_string_helper(seperator);

					int vectorsize=temp_string_vector.size();
					for (int j=0;j<vectorsize;j++) { //The first argument was already used
						result_string_vector.push_back(key_string +seperator+temp_string_vector[j]);
					}
				}
			}
			return result_string_vector;
		}
			
		

	public:

		//----------------------------------------------------------------------------
		JShawnNDimensionalResult()
			: is_value_(false), axis_label_("default_axis")
		{}

		virtual ~JShawnNDimensionalResult()
		{}

		//----------------------------------------------------------------------------
		JShawnNDimensionalResult<Type,Index>& operator[](Index i) 
			throw(std::logic_error)
		{
			if(is_value_ == true) 
				throw std::logic_error("A value can not be indexed.");

			return values_[i];
		}

		//----------------------------------------------------------------------------
		void operator=(Type i)
			throw(std::logic_error)
		{
			if(values_.size() > 0) 
				throw std::logic_error("A index can not be assigned a value.");

			is_value_ = true;
			value_ = i;
		}    

		//----------------------------------------------------------------------------
		Type value() 
		{
			if(!is_value_) 
				throw std::logic_error("Not a value type");
			return value_;
		}

		//----------------------------------------------------------------------------
		void set_axis_label(std::string label) {
			axis_label_=label;
		}

		//----------------------------------------------------------------------------
		virtual std::string name()
		{
			return "n_dimensional_result";
		}

		//----------------------------------------------------------------------------
		virtual std::string description()
		{
			return description_;
		}

		//----------------------------------------------------------------------------
		virtual void set_description(std::string d)
		{
			description_ = d;
		}

		//----------------------------------------------------------------------------
		virtual void send_to(std::ostream& o) 
		{
			o<<"<axis name=\""<<axis_label_<<"\" ";
			if( is_value_ )
			{
				o<<"is_value=\"true\" value=\""<<value_<<"\">"<<std::endl;
			}
			else
			{			
				o<<"is_value=\"false\">"<<std::endl;

				for(MapIt it = values_.begin(); it!= values_.end(); ++it) {
					o<<"<index value=\""<<(it->first)<<"\">"<<std::endl;
					it->second.send_to(o);
					o<<"</index>"<<std::endl;
				}
			}
			o<<"</axis>"<<std::endl;
		}

		//----------------------------------------------------------------------------
		virtual void send_table_string_to(std::ostream& o, std::string seperator)  {
			
			std::string table_string="";
			std::string lineSep = "\n";
		
			std::string column_names=get_all_axis_labels(seperator);
		
			column_names=column_names+lineSep;
			//std::cerr<<column_names<<std::endl<<std::flush;
		
			std::vector<std::string> table_string_vector=to_table_string_helper(seperator);
		//std::cerr<<"nach Methode"<<std::endl<<std::flush;
			int vectorsize=table_string_vector.size();
			for (int j=0;j<vectorsize;j++) { 
				table_string=table_string+table_string_vector[j]+lineSep;
			}
		
			o<<(column_names+table_string);


		}


		//-----------------------------------------------------------------------
		/**
		*  The getAllAxisLabels-method retrieves the axis labels from all first elements in every dimension
		*  and puts them into a String sperated by the given seperator
		*/
		virtual std::string get_all_axis_labels(std::string seperator) {
			if (is_value_==true) {
				return axis_label_;
			}
			
			MapIt entry = values_.begin(); //getting the first element
			JShawnNDimensionalResult value = entry->second;
			return axis_label_+seperator+value.get_all_axis_labels(seperator);
		}

		//----------------------------------------------------------------------------
		virtual void clear()
		{
			//clears the result and resets the standard settings
			//ATTENTION: The rekursive structure is not yet cleared
			is_value_=false;
			axis_label_="default_axis";
			values_.clear();
			
		}

	};

}

#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/simulation/jshawn/jshawn_n_dimensional_result.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: jshawn_n_dimensional_result.h,v $
 *-----------------------------------------------------------------------*/

 
 

