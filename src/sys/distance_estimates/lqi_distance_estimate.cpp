/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/random/uniform_random_variable.h"
#include "sys/distance_estimates/lqi_distance_estimate.h"
#include "sys/node.h"
#include <iostream>
#include <fstream>
#include <cstdlib>



namespace shawn {

// ----------------------------------------------------------------------
LQIDistanceEstimate::LQIDistanceEstimate(std::string name, std::string filename) :
	name_(name),filename_(filename),max_measure_size_(0)

{



	}


void LQIDistanceEstimate::init(void){

	nr_.init();

		std::ifstream inStream;

		inStream.open(filename_.c_str());
		int i=0;

		if (inStream.fail()) {

			std::cout << "Can't open file "<<filename_<<" !\n";

			exit(1);

		}

		else {



			while ((!inStream.eof()&&(i<max_measure_size_))||(!inStream.eof()&&(max_measure_size_==0))) {


				double fdist,fmean,fstdv;

				inStream >> fdist >> fmean >> fstdv;



				dists.push_back(fdist);
				means.push_back(fmean);
				stdvs.push_back(fstdv);

				i++;



			}

			#ifdef debug_iqdis
				std::cout << "barrasy \n";
				for(int j=0; j<max_measure_size_;j++){

					std::cout << j << " "<<dists[j] << "  "<<means[j]<<" "<<stdvs[j]<<"\n";
				}
			#endif

				// Close File

				inStream.close();

			}

		if(max_measure_size_==0)
			max_measure_size_=i-1;

	//extending the measure
		else{
		if(i<max_measure_size_-1){

			for(int k =i ; k<max_measure_size_-1; k++){

				dists.push_back(dists[k-1]+1);
				means.push_back(dists[k-1]+1);
				//keep the last stdv
				stdvs.push_back(stdvs[k-1]);

			}

		}
		}
#ifdef debug_iqdis
		std::cout << "arrasy \n";
		for(int j=0; j<max_measure_size_;j++){

			std::cout << j << " "<<dists[j] << "  "<<means[j]<<" "<<stdvs[j]<<"\n";
		}
#endif
}


	// ----------------------------------------------------------------------
	LQIDistanceEstimate::
	~LQIDistanceEstimate()
	{}

	// ----------------------------------------------------------------------
	bool LQIDistanceEstimate::estimate_distance(const Node& source,
			const Node& target, double& result) const throw () {


		double
				dist =
						(source.real_position() - target.real_position()).euclidean_norm();



		// find distance
		int index=0;
		for(int i = 0 ; i< max_measure_size_ ; i++){


			if(dists[i]>dist){
				index = i-1;
				break;
			}
		}




		if ((index >= max_measure_size_)|| (index<1)){
			//use the the same distance as mean and the last stdv
			result = (double(nr_) * stdvs[index]) + dist;

		}

		else{

			//take the average of the current distance and the next one
			double first_result = (double(nr_) * stdvs[index]) + means[index];

			double second_result = (double(nr_) * stdvs[index+1]) + means[index+1];

			if((second_result<=first_result)||(means[index+1]<=means[index])){

				result = (first_result + second_result)/2;
			}else{

				result = dists[index]+(((second_result-first_result)*(dist-dists[index]))/(dists[index+1]-dists[index]));
				#ifdef debug_iqdis
						std::cout << "dist test \n";


							std::cout << dist << " "<<result <<" \n";

				#endif
							return true;
			}



		}

		return true;
	}

	// ----------------------------------------------------------------------
	std::string LQIDistanceEstimate::name(void) const throw () {
		return name_;
	}

	// ----------------------------------------------------------------------
	std::string LQIDistanceEstimate::description(void) const throw () {
		return std::string("distance estimate to other nodes using LQI");
	}


	 void   LQIDistanceEstimate::set_max_measure_size( int max_measure_size )
	   {
		 max_measure_size_ = max_measure_size;
	   }



}

