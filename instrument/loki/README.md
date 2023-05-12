# Detector Calibration
This repository is for all detector calibration calculations for calibration applied at the data acquisition stage, ie. irreversible calibration applied before any data is stored. Specific forms of calibration are listed below.

## Straw Calibration
Parallex effect calibration is calculated using a mask with slits in it, and a simulation with the same mask. For each straw in the detector, the measured peaks in neutron events are compared to simulated peaks, and a correction polynomial is fitted. This code is based off code from Davide Raspino from STFC.

Simulated peaks are clear and easily defined. They can be found with the ROOT function ShowPeaks.

<img src="https://user-images.githubusercontent.com/9928665/218416771-7654ac43-a676-4fbd-93ac-5676950308c7.png" width="50%" height="50%">

Measured data is more noisy, and harder to identify the peaks. For these values we approximate with the ROOT function ShowPeaks and then refine it with a multi-gaussian fit.

<img src="https://user-images.githubusercontent.com/9928665/218417468-dd34ae67-2a7b-42fc-8992-93c348f592d3.png" width="50%" height="50%">

The difference between simulated and measured peaks is calculated, and a polynomial relationship is fitted. This is then saved to a json file in the expected format for the Event Formation Unit/EFU.
