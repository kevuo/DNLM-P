/*
 * ParallelDeWAFF.hpp
 *
 *  Created on: Nov 5, 2015
 *      Author: davidp
 */

#ifndef PARALLELDEWAFF_HPP_
#define PARALLELDEWAFF_HPP_

#include <string>
#include "bfilterDeceived.hpp"
#include "nlmfilterDeceived.hpp"
#include "noAdaptiveLaplacian.hpp"
#include "timer.h" 
using namespace std;

class ParallelDeWAFF{
public:
	void help();
	Mat processImage(const Mat& U, int num_threads);
	NoAdaptiveLaplacian* getNAL();

private:
	NoAdaptiveLaplacian nal;
	BFilterDeceived bfd;
	NLMFilterDeceived nlmfd;

	Mat filterDeceivedNLM(const Mat& U, int wSize, int wSize_n, double sigma_s, int sigma_r, int lambda, int num_threads);
};
#endif /* PARALLELDEWAFF_HPP_ */
