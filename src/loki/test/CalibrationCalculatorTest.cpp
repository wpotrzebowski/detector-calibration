#include "../CalibrationCalculator.h"
#include <gtest/gtest.h>


TEST (CalibrationCalculatorTest, testNullCalibration){
  CalibrationCalculator calculator;
  std::vector<double> measured = {1.0, 2.0, 3.0, 4.0};
  std::vector<double> simulated = {0.0, 1.0, 2.0, 3.0, 4.0};
  std::vector<double> params = calculator.calculateStrawCalibrationParameters(measured, simulated, 0);

  std::vector<double> expected_params = {0.0, 0.0, 0.0, 0.0};
  EXPECT_EQ(params, expected_params);
}


TEST (CalibrationCalculatorTest, testSimpleCalibration){
  CalibrationCalculator calculator;
  std::vector<double> measured = {15, 22, 31, 40, 49, 58, 65};
  std::vector<double> simulated = {0, 10, 20, 30, 40, 50, 60, 70};
  std::vector<double> params = calculator.calculateStrawCalibrationParameters(measured, simulated, 0);
  simulated = {10, 20, 30, 40, 50, 60, 70};
  std::vector<double> result = calculator.applyCalibrationParams(measured, params);
  for(int i = 0; i < measured.size(); i++){
    EXPECT_NEAR(result[i], simulated[i], 1);
  }
}


TEST (CalibrationCalculatorTest, testRange){
  CalibrationCalculator calculator;
  std::vector<double> measured = {15, 22, 31, 40, 49, 58, 65};
  std::vector<double> simulated = {0, 10, 20, 30, 40, 50, 60, 70};
  std::vector<double> params = calculator.calculateStrawCalibrationParameters(measured, simulated, 0);
  
  
  std::vector<double> range(70);
  for(int i = 10; i < 80; i++){
    range[i-10] = i; 
  }
  std::vector<double> result = calculator.applyCalibrationParams(range, params);
  for(int i = 0; i < range.size(); i++){
    EXPECT_TRUE((result[i] >= 0) && (result[i] <= 511));
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  auto retval = RUN_ALL_TESTS();
  return retval;
}
