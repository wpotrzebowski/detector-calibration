#include <TH1D.h>
#include <TPolyMarker.h>
#include "TList.h"
#include "TF1.h"
#include "H5Cpp.h"
#include "TCanvas.h"


class CalibrationCalculator {

public:

  bool plottingGraphs = true;
  int strawResolution = 512;

  void calculateCalibration(std::vector<std::vector<int>> measuredEvents, std::vector<std::vector<int>> simulatedEvents);
  
  
  std::vector<double> getStrawPeaksSimple(std::vector<int> hits, int strawNum, std::string file_prefix);
  
  
  std::vector<double> getStrawPeaksGaussian(std::vector<int> hits, int strawNum, std::string file_prefix);
  
  
  void fillHistogram1D(std::vector<int> hits, TH1D* histogram);
  
  
  void gaussianFit(std::vector<double> x, std::vector<double> y, TH1D* histogram);
  
  
  std::pair<std::vector<double>, std::vector<double>> findPeaks(TH1D* histogram);
    
  
  std::vector<double> calculateStrawCalibrationParameters(std::vector<double> measuredPeaks, std::vector<double> simulatedPeaks, int strawId);


  void writePeaksToFile(std::vector<double> peaks, std::string filename);
  
  
  void loadPeaksFromFile();
  void saveCalibrationParametersToFile();
  void selectionSort(std::vector<double> a, std::vector<double> b, int n);

};