/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_VALUE_COLLECTION_H
#define __SHAWN_APPS_VALUE_COLLECTION_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_DISTEST
/*
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/vec.h"
#include "apps/distest/histogram.h"
#include "sys/node_distance_estimate.h"
#include "apps/distest/distest_postscript_writer.h"
#include "sys/distance_estimates/perfect_distance_estimate.h"
//#include "apps/distest/distest_task.h"*/
#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <fstream>
#include <deque>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>

using namespace std;

//namespace shawn {class Node;}

//typedef unsigned int uint;
//typedef vector< vector<double> > FloatMatrix;
//typedef vector< vector<uint> > UINTMatrix;


namespace distest
{

	class FileHandleCache {
	private:
		struct Data {
			Data(int key, const string& id) : key_(key), id_(id) {}
			std::ofstream* fh_;
			int key_;
			string id_;
			bool operator==(const Data& other) {
				return key_ == other.key_ && id_ == other.id_;
			}
		};

		string prefix_;
		int max_;
		deque<Data> cache_;

	public:
		FileHandleCache(string prefix, int max = 500) : prefix_(prefix), max_(max) {}

		~FileHandleCache() 
		{
			//cout << "Destructor called" << endl << flush;
			emptyCache();
		}

		void emptyCache(){
			for(deque<Data>::iterator it = cache_.begin(), end = cache_.end(); it!=end; ++it)
			{
				std::ofstream* tmpfh = (*it).fh_;

				if(tmpfh!=NULL) {
				//cout << "Closing file key["<<(*it).key_<<"], ident["<<(*it).id_<<"]" << endl << flush;
					tmpfh->flush();
					tmpfh->close();
					delete tmpfh;
					(*it).fh_ = NULL;
				}

			}
		}

		std::ofstream* get(int key, const std::string& id) 
		{
			deque<Data>::iterator it = find(cache_.begin(), cache_.end(), Data(key, id));
			std::ofstream* fh = NULL;
			
			//Gefunden im Cache -> nach vorne in die queue
			if( it != cache_.end() ) 
			{
				fh = (*it).fh_;
				cache_.push_back( (*it) );
				cache_.erase(it);
			}
			//Nicht gefunden im Cache -> neu anlegen, evtl. alten loeschen und nach vorne in die queue
			else
			{
				if( int(cache_.size()) >= max_ )
				{
					cache_.back().fh_->flush();
					cache_.back().fh_->close();
					//cout << "Closing file key["<<cache_.back().key_<<"], ident["<<cache_.back().id_<<"] --> Remove from cache." << endl << flush;
					cache_.pop_back();
				}

				ostringstream os;
				os << prefix_ << key << "_"<<id<< ".ms";
				//cout << "FHC: Creating file " << os.str() << "..." << flush;

				cache_.push_front( Data(key, id) );
				fh = new std::ofstream(os.str().c_str(), ofstream::out | ofstream::app );
				cache_.front().fh_ = fh;
				assert( cache_.front().fh_->is_open() );

				//cout << " success" << endl << flush;
			}

			assert(fh != NULL);
			return fh;
		}

	};



	class valueCollection {

	public:
		valueCollection();
		void insert( int key, std::string ident, float val );
		void set_file_handle_cache(FileHandleCache*);

		//void close();
		void clear();
		multiset<float>* getVals(int key, std::string ident);
		~valueCollection();
//	protected:
		//friend class MultihopDistanceEstimateTask;
		//friend class DistanceEstimateTask;

	private:
		FileHandleCache* fhc_;
		//std::ofstream* newFile(int key, std::string ident);
		bool file;
		multiset<float>* err;
		//std::ofstream* fh;

	};


}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/distest/value_collection.h,v $
* Version $Revision: 197 $
* Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: value_collection.h,v $
 *-----------------------------------------------------------------------*/
