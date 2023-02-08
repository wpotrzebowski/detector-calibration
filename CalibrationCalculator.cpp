#include "CalibrationCalculator.h"
#include <iostream>
#include <fstream> 
#include "TGraph.h"

/// \brief takes measured and simulated events as a vector of vectors of ints, where each vector of ints represents a straw, and each int represents a neutron event's position along the straw
/// and calculates a 4th order polynomial relationship between the measured peaks of events and the simulated peaks of events, then saving it to a json file
void CalibrationCalculator::calculateCalibration(std::vector<std::vector<int>> measuredEvents, std::vector<std::vector<int>> simulatedEvents){
  std::map<int, std::vector<double>> calibrationPerStraw;
  for (int strawNumber = 0; strawNumber < measuredEvents.size(); strawNumber++){
    std::vector<double> measuredPeaks = getStrawPeaksGaussian(measuredEvents[strawNumber], strawNumber, "measured");
    std::vector<double> simulatedPeaks = getStrawPeaksSimple(simulatedEvents[strawNumber], strawNumber, "simulated");
    if (measuredPeaks.size() != simulatedPeaks.size()){
      std::cout << "npeaks doesn't match, measured = " + std::to_string(measuredPeaks.size()) + " and simulated = " + std::to_string(simulatedPeaks.size()) << std::endl;
      //TODO, determine way to filter peaks so that they match
    }
    else{
      std::cout << "npeaks match!!! " + std::to_string(measuredPeaks.size())<< std::endl;
      std::vector<double> calibrationParameters = calculateStrawCalibrationParameters(measuredPeaks, simulatedPeaks, strawNumber);
      calibrationPerStraw[strawNumber] = calibrationParameters;
    }
  }

}




std::vector<double> CalibrationCalculator::calculateStrawCalibrationParameters(std::vector<double> measuredPeaks, std::vector<double> simulatedPeaks, int strawId){
  std::vector<double> result;
  const int n = measuredPeaks.size();

  TGraph *graph = new TGraph(n, measuredPeaks.data(), simulatedPeaks.data());
  TF1 *function = new TF1("f", "[0] + [1]*x + [2]*x^2 + [3]*x^3 + [4]*x^4", 0, strawResolution);
  graph->Fit(function, "Q");

  // getting each of the 4 polynomial values
  for (int i = 0; i < 5; ++i) {
    result.push_back(function->GetParameter(i));
  }

  if (plottingGraphs){
    TCanvas *canvas1 = new TCanvas("c1", "c1", 600, 400);
    graph->Draw("AP");
    std::string filename  = std::to_string(strawId) + "_calibrationfit.png";
    canvas1->SaveAs(filename.c_str());
  }

  return result;
}



std::pair<std::vector<double>, std::vector<double>> CalibrationCalculator::findPeaks(TH1D* histogram){
	TH1D* histogram_background = (TH1D*) histogram->ShowBackground(20, "same");   //Measure Background
	histogram->Add(histogram_background, -1.);                      //subtract Background
	int npeaks=histogram->ShowPeaks(2.0, "", 0.5);                           // find peaks
	std::vector<double> x;
  std::vector<double> y;

  //code to display peaks found by ShowPeaks function above
  TList *histogram_functions = histogram->GetListOfFunctions();
  TPolyMarker *amy_peaks = (TPolyMarker *)histogram_functions->FindObject("TPolyMarker");
  if (amy_peaks == nullptr) {
    std::cout << "Error getting TPolyMarker object!" << std::endl;
    return std::make_pair(x, y);
  }
  double *ax = amy_peaks->GetX();
  double *ay = amy_peaks->GetY();

   for (int a = 0; a < npeaks; a++) {
    x.push_back(ax[a]);
    y.push_back(ay[a]);
  }
	selectionSort(x, y, npeaks);
	return std::make_pair(x, y);
}

std::vector<double> CalibrationCalculator::getStrawPeaksSimple(std::vector<int> hits, int strawNum, std::string file_prefix){
  if(hits.size() == 0){
     std::vector<double> emptyVector;
    return emptyVector;
  }
  TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600);
  std::pair<std::vector<double>, std::vector<double>> x_y_peaks;
  TH1D* histogram = new TH1D(("histogram_straw_" + std::to_string(strawNum)).c_str(), ("histogram_title" + std::to_string(strawNum)).c_str(), strawResolution, 0, strawResolution);
  fillHistogram1D(hits, histogram);
  x_y_peaks = findPeaks(histogram);
  histogram->Draw();
  canvas->Update();
  std::string filename = file_prefix + "_canvas_" + std::to_string(strawNum) + ".png";
  canvas->SaveAs(filename.c_str());

  return x_y_peaks.first;
}

/// \brief takes the hits for a single straw and uses ROOTs ShowPeaks function for first pass, and then multi gaussian fitting to refine
/// and returns the x values of the peaks on the straw
std::vector<double> CalibrationCalculator::getStrawPeaksGaussian(std::vector<int> hits, int strawNum, std::string file_prefix){
  if(hits.size() == 0){
     std::vector<double> emptyVector;
    return emptyVector;
  }
  TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600);
  TH1D* histogram = new TH1D(("histogram_straw_" + std::to_string(strawNum)).c_str(), ("histogram_title" + std::to_string(strawNum)).c_str(), strawResolution, 0, strawResolution);
  fillHistogram1D(hits, histogram);
  std::pair<std::vector<double>, std::vector<double>> x_y_peaks = findPeaks(histogram);
  std::string filename;

  if (plottingGraphs){
    histogram->Draw();
    canvas->Update();
    filename = file_prefix + "_canvas_" + std::to_string(strawNum) + ".png";
    canvas->SaveAs(filename.c_str());
  }
	
  // refines the peaks found by findPeaks using multiple gaussians fitting
  gaussianFit(x_y_peaks.first, x_y_peaks.second, histogram);

  if (plottingGraphs){
    canvas->Update();
    filename = "gaussian_" + filename;
	  canvas->SaveAs(filename.c_str());
  }
  delete canvas;
  delete histogram;

  // returning just the x values of the peaks - y values no longer relevant
  return x_y_peaks.first;
}


/// \brief given the x and y values of pre-calculated peaks, and a histogram, refines those peaks using multi-gaussian fits
/// x will be adjusted according to the newly refined peak locations, return is void
void CalibrationCalculator::gaussianFit(std::vector<double> x, std::vector<double> y, TH1D* histogram){
	
  // setting up function string for length of multi gaussian needed, based on precalculated peaks
  char functionstring[2000];
	sprintf(functionstring, "gaus(0)");
  int npeaks = x.size();
	if (npeaks > 1) {
	  for (int k = 1; k < npeaks; k++) {
		  sprintf(functionstring, "%s + gaus(%d)", functionstring, k * 3);
	  }
	}
	sprintf(functionstring, "%s + pol2(%d)", functionstring, npeaks * 3);
  TF1* function = new TF1("gaus", functionstring, 100, 450);

  // setting up function parameters, a y value, x value, and --- per straw, and then ...
	const int npar = npeaks * 3 + 3;
	double* par = new double[npar];
	for (int ii = 0; ii < npeaks; ii++) {
		par[ii * 3 + 0] = y[ii];
		par[ii * 3 + 1] = x[ii];
		par[ii * 3 + 2] = 1.1;
	}
	par[npeaks * 3 + 0] = 3.1;
	par[npeaks * 3 + 1] = 0.001;
	par[npeaks * 3 + 2] = 0.00001;

  // setting parameters
	function->SetParameters(par);
	function->SetLineColor(kOrange);
	function->SetNpx(100000);

  // fitting function and drawing it on the histogram canvas
	histogram->Fit(function, "E0", "", 100, 450);
  function->Draw("same");
  
  // // getting the gaussian parameters - we don't use these for anything, could be removed
  // double* results = new double[npar];
  // function->GetParameters(results);


}


void CalibrationCalculator::writePeaksToFile(std::vector<double> peaks, std::string filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: unable to open file '" << filename << "'" << std::endl;
    return;
  }
  
  for (double peak : peaks) {
    file << peak << std::endl;
  }
  
  file.close();
}

// sorts the elements in array a from smallest to largest, and re-organises
// array b the same way, regardless of the numerical order of elements in b
// n is the length of both arrays a and b
void CalibrationCalculator::selectionSort(std::vector<double> a, std::vector<double> b, int n) {
	int i, j, min;
	double  tempa, tempb;
	for (i = 0; i < n - 1; i++) {
		min = i;
		for (j = i + 1; j < n; j++)
			if (a[j] < a[min])
				min = j;
		tempa = a[i];
		tempb = b[i];
		a[i] = a[min];
		b[i] = b[min];
		a[min] = tempa;
		b[min] = tempb;
	}
}

void CalibrationCalculator::fillHistogram1D(std::vector<int> hits, TH1D* histogram) {
	std::cout << "Histogram contains " + std::to_string(hits.size()) + " hits \n";
  for (int hit : hits) {
		histogram->Fill(hit);
	}
}
