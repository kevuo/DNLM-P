/*
 * main.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: davidp
 */
#include "parallelDeWAFF.hpp"
#include <ctime>
using namespace std;
  

int main(int argc, char* argv[]){
    ParallelDeWAFF deWAFF;
    double elapsedTime;

    //Check input arguments
    if (argc != 4){
        cout << "ERROR: Not enough parameters" << endl;
        deWAFF.help();
        return -1;
    }else{

    //Open input video file
    const string inputFile = argv[1];
    // Find extension point
    string::size_type pAt = inputFile.find_last_of('.');
    const int num_threads =  atoi (argv[2]);
    // Form the new name with container
    const string outputFile = inputFile.substr(0, pAt) + "_DeNLM.jpg";
    const char* results = argv[3];
    //Create the Laplacian of Gaussian mask once
    NoAdaptiveLaplacian* nAL = deWAFF.getNAL();
    Mat h =  Tools::fspecialLoG(17, 0.005);
    nAL->setMask(-h);
    Mat U,F1;
    U = imread(inputFile, CV_LOAD_IMAGE_COLOR);
    //Read one frame from input video
        if(!U.data){
            cout << "Could not read image from file." << endl;
            return -1;
        }
        //time start
        //clock_t begin = clock();
        timerStart();
        F1 = deWAFF.processImage(U, num_threads);
        //time end
        elapsedTime = timerStop();
//    clock_t end = clock();
  //  double elapsed_secs =  ((double) (end - begin)) / CLOCKS_PER_SEC;
    Tools::writeToFile(results, elapsedTime);
    cout << "Time to process an image: "  << elapsedTime<< " seconds." <<endl;
    
    //Write image to output file.
    imwrite(outputFile, F1);
    return 0;
    }
}

NoAdaptiveLaplacian* ParallelDeWAFF::getNAL(){
    return &(this->nal);
}

void ParallelDeWAFF::help(){
    cout
        << "------------------------------------------------------------------------------" << endl
        << "Usage:"                                                                         << endl
        << "./ParallelDeNLM inputImageName num_threads outputFile"                                                << endl
        << "------------------------------------------------------------------------------" << endl
        << endl;
}

//used parameters for the paper CONCAPAN 2016
Mat ParallelDeWAFF::processImage(const Mat& U, int num_threads){
    //Set parameters for processing
    int wRSize = 21;
    int wSize_n=1;
    double sigma_s = wRSize/1.5;
    int sigma_r = 3; //13
    int lambda = 1.7;

    Mat fDeceivedNLM = filterDeceivedNLM(U, wRSize, wSize_n, sigma_s, sigma_r, lambda, num_threads);
    //Mat fBilD = filterDeceivedBilateral(U, wRSize, sigma_s, sigma_r, lambda);
    //Mat fBilD = filterDGF(lambda, wRSize, 1, sigma_r, U);

    return fDeceivedNLM;
}

//Input image must be from 0 to 255
Mat ParallelDeWAFF::filterDeceivedNLM(const Mat& U, int wSize, int wSize_n, double sigma_s, int sigma_r, int lambda, int num_threads){
    Mat Unorm;
    //convert to grayscale
    //cvtColor(U, grey, CV_BGR2GRAY);
    //The image has to have values from 0 to 1
    U.convertTo(Unorm,CV_32FC3,1.0/255.0);
    //[L, alfaMat, Vnorm] = adaptiveLaplacian(Unorm, amps, trap1, trap2);

    Mat L = this->nal.noAdaptiveLaplacian(Unorm, lambda);
    Mat F = this->nlmfd.nlmfilterDeceived(Unorm, L, wSize, wSize_n, sigma_s, sigma_r, num_threads);

    //putting back everything
    F.convertTo(F,CV_8UC1,255);
    //Tools::showImg(F);
    return F;
}
