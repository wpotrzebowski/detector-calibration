#include <iostream>
#include <vector>
#include <map>
#include <hdf5.h>
#include <H5Cpp.h>

// Define a class to read the HDF5 data
class H5Reader {
public:
  H5Reader(std::string measuredFilename, std::string simulatedFilename, double tMin, double tMax, int strawResolution)
    : measuredFilename(measuredFilename), simulatedFilename(simulatedFilename), tMin(tMin), tMax(tMax), strawResolution(strawResolution) {}


void readSimulatedData() {
  
  try {
      std::cout << "reading simulated data" << std::endl;;
      // Open the HDF5 file
      H5::H5File file(simulatedFilename, H5F_ACC_RDONLY);

      // Open the "event data" dataset
      H5::Group group = file.openGroup("mantid_workspace_1/mask_workspace");
      H5::DataSet maskPosition = group.openDataSet("values");

      // Get the dataspace of the dataset
      H5::DataSpace dataspace = maskPosition.getSpace();

      std::cout << "read data, manipulating into vectors" << std::endl;;

      // Get the number of events
      int numPoints = dataspace.getSimpleExtentNpoints();

      // Read the data from the dataset
      std::vector<int> vectorMaskPosition(numPoints);
      maskPosition.read(vectorMaskPosition.data(), H5::PredType::NATIVE_INT);

      std::cout << "splitting into straws" << std::endl;;
      // Separate the events into straws based on pixel ID
      for (int i = 0; i < numPoints; i++) {
        int straw = i / strawResolution;
        if (vectorMaskPosition[i] == 0) {
          simulatedEvents[straw].push_back(i%strawResolution);
        }
      }

    } catch (H5::Exception &e) {
      std::cerr << "HDF5 exception: " << e.getDetailMsg() << std::endl;
    }
  
}

  // Read the data from the HDF5 file
  void readMeasuredData() {
    try {
      // Open the HDF5 file
      H5::H5File file(measuredFilename, H5F_ACC_RDONLY);

      // Open the "event data" dataset
      H5::Group group = file.openGroup("entry/instrument/larmor_detector/larmor_detector_events");
      H5::DataSet pixel_ids = group.openDataSet("event_id");
      H5::DataSet time_of_flight = group.openDataSet("event_time_offset");

      // Get the dataspace of the dataset
      H5::DataSpace dataspace = pixel_ids.getSpace();

      // Get the number of events
      int numEvents = dataspace.getSimpleExtentNpoints();

      // Read the data from the dataset
      std::vector<double> timeOfFlight(numEvents);
      std::vector<int> pixelId(numEvents);
      time_of_flight.read(timeOfFlight.data(), H5::PredType::NATIVE_DOUBLE);
      pixel_ids.read(pixelId.data(), H5::PredType::NATIVE_INT);

      // Filter the events based on time of flight
      for (int i = 0; i < numEvents; i++) {
        if (timeOfFlight[i] >= tMin && timeOfFlight[i] <= tMax) {
          filteredTimeOfFlight.push_back(timeOfFlight[i]);
          filteredPixelId.push_back(pixelId[i]);
        }
      }

      // Separate the events into straws based on pixel ID
      int numFilteredEvents = filteredTimeOfFlight.size();
      for (int i = 0; i < numFilteredEvents; i++) {
        int straw = filteredPixelId[i] / strawResolution;
        measuredEvents[straw].push_back(filteredPixelId[i]%strawResolution);
      }

    } catch (H5::Exception &e) {
      std::cerr << "HDF5 exception: " << e.getDetailMsg() << std::endl;
    }
  }

  std::vector<std::vector<int>> simulatedEvents = std::vector<std::vector<int>>(458752);
  std::vector<std::vector<int>> measuredEvents = std::vector<std::vector<int>>(458752);

private:
  std::string measuredFilename;
  std::string simulatedFilename;
  double tMin, tMax;
  int strawResolution;
  std::vector<double> filteredTimeOfFlight;
  std::vector<int> filteredPixelId;

};
