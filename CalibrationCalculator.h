#include <TH1D.h>
#include <TPolyMarker.h>
#include "TList.h"
#include "H5Cpp.h"


class CalibrationCalculator {

public:
  TH1D *histogram;
  TH1D *histogram_background;

  void fillHistogram1D(std::vector<int> hits);
  void loadEventsFromH5File();
  int findPeaks(double *x, double *y);
  void fitPeaks();
  void savePeaksToFile();
  void loadPeaksFromFile();
  void calculateCalibrationParameters();
  void saveCalibrationParametersToFile();
  void selectionSort(double* a, double* b, int n);

};