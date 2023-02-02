#include "CalibrationCalculator.h"
#include "H5Reader.h"
#include "TCanvas.h"


int main(){
  CalibrationCalculator calculator;
  H5Reader reader("/Users/jenniferwalker/straw_calibration_calculation/overnight_uncalibrated4-2022-02-28_2215.nxs", 1000000, 100000000000, 512);
  reader.readData();
  for (int i = 0; i < reader.events.size(); i++){
    auto event = reader.events[i];
    TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600);
    double *x = new double[100];
    double *y = new double[100];
    calculator.histogram = new TH1D("histogram_name", "histogram_title", 512, 0, 512);
    calculator.fillHistogram1D(event);
    calculator.findPeaks(x, y);
    calculator.histogram->Draw();
    canvas->Update();
    std::string filename = "canvas_" + std::to_string(i) + ".png";
    canvas->SaveAs(filename.c_str());
  }
  // calculator.findPeaks(x, y);
  return 0;
}