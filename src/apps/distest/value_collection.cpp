/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "value_collection.h"
#ifdef ENABLE_DISTEST

#include "apps/distest/value_collection.h"
#include <iostream>
#include <assert.h>
/*#include "sys/node_distance_estimate.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/distance_estimates/perfect_distance_estimate.h"
#include "sys/misc/number_formatter.h"
#include "apps/distest/distest_postscript_writer.h"
#include "apps/distest/neighbor_intersection_distance_estimate.h"
*/
/*
#include "sys/simulation/simulation_environment.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"

#include "apps/distest/histogram.h"
*/

//#include "sys/node.h"
#include <vector>
#include <math.h>
#include <set>


//using namespace shawn;
//using namespace std;


namespace distest
{ 


		valueCollection::valueCollection() 
		{
			file = true;
			err = NULL;
		}

		void valueCollection::set_file_handle_cache(FileHandleCache* fhc) {
			if(file)
				fhc_ = fhc;
		}

		/*
		std::ofstream* valueCollection::newFile(int key, std::string ident) {
			ostringstream os;
			os << "multiset" << key << "_"<<ident<< ".ms";
			cout << "Creating file " << os.str() << "..." << flush;
			std::ofstream* fh = new std::ofstream(os.str().c_str() );
			assert( fh->is_open() );
			cout << " success" << endl << flush;
			return fh;
		}
		*/

		void valueCollection::insert( int key, std::string ident, float val ) {
			if (file) {
				assert(fhc_ != NULL);
				//cout << "insert(key=" << key << ", ident=" << ident  << "): value= " << val << flush;
				std::ofstream* fh = fhc_->get(key, ident);
				assert(fh != NULL);
				(*fh) << val;
				(*fh) << " ";
				//cout << "success."<< endl << flush;

			} else {
				if (err==NULL) err = new multiset<float>();
				err->insert(val);
			}

		}

		/*
		void valueCollection::close() {
			if (fh != NULL) {
				fh->flush();
				fh->close();
			}
		}
		*/

		void valueCollection::clear() {
//			if (file) {
//				delete(fh); fh=NULL;
				delete(err); err = NULL;
//			} else 
//				if (err!=NULL) delete(err); err = NULL;
		}

		multiset<float>* valueCollection::getVals(int key, std::string ident) {
			if (file) {
				ostringstream os;
				os << "multiset" << key << "_"<<ident<< ".ms";
				std::ifstream f(os.str().c_str() );
				err = new multiset<float>();
				assert( f.is_open() );
				float val;
				while ( f >> val) { err->insert(val); f.ignore();}
				//cout << "nach reading aus dem file " << os.str() << endl << flush;
				f.close();
				return err;
			} else {
				return err;
			}
		}


		valueCollection::~valueCollection() {
			if (err!=NULL) 
				delete(err);
		}




}
#endif

