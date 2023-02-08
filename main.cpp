#include "CalibrationCalculator.h"
#include "H5Reader.h"



int main(){
  CalibrationCalculator calculator;
  H5Reader reader("/Users/jenniferwalker/straw_calibration/overnight_uncalibrated4-2022-02-28_2215.nxs", "/Users/jenniferwalker/straw_calibration/Larmor_rear_bank_experiment_xoffset5mm_sdd4099_px512_reduced_new_numbering_masked_new.nxs", 1000000, 100000000000, 512);
  reader.readMeasuredData();
  reader.readSimulatedData();
  calculator.calculateCalibration(reader.measuredEvents, reader.simulatedEvents);
  return 0;
}