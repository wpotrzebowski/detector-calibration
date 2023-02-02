#include "CalibrationCalculator.h"
#include <iostream>


int CalibrationCalculator::findPeaks(double *x, double *y){
	histogram_background = (TH1D*) histogram->ShowBackground(20, "same");   //Measure Background
	histogram->Add(histogram_background, -1.);                      //subtract Background
	int npeaks;
	int index = 1;
	npeaks=histogram->ShowPeaks(2.0, "", 0.5);                           // find peaks
	TList *histogram_functions = histogram->GetListOfFunctions();
  TPolyMarker *amy_peaks = (TPolyMarker *)histogram_functions->FindObject("TPolyMarker");
  double *ax = amy_peaks->GetX();
  double *ay = amy_peaks->GetY();

  for (int a = 0; a < npeaks; a++) {
    x[a] = ax[a];
    y[a] = ay[a];
  }
	selectionSort(x, y, npeaks);
	return npeaks;
}


// sorts the elements in array a from smallest to largest, and re-organises
// array b the same way, regardless of the numerical order of elements in b
// n is the length of both arrays a and b
void CalibrationCalculator::selectionSort(double* a, double* b, int n) {
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

void CalibrationCalculator::fillHistogram1D(std::vector<int> hits) {
	std::cout << "Histogram contains " + std::to_string(hits.size()) + " hits \n";
  for (int hit : hits) {
		histogram->Fill(hit);
	}
}
