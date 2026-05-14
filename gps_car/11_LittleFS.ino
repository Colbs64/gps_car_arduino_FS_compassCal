// ************************   LITTLEFS FUNCTIONS   ************************//

// This file contains the functions that are used for the LittleFS for storing misc data that we need for each car.
// The data will be saved in text files. For now, just the variable compass_calibration

// Returns data as a chararray to out_str
// int is used to check if the file is opened properly or if the command operated successfully
int FS_readData(const char * path, char* out_str, size_t size) {
  if (!out_str || size == 0) return 0;

  FILE *file = fopen(path, "r");

  // Testing if file exists
  if (file) {
    Serial.println("Open ok!");
  } else {
    Serial.println("Open failed");
    // troubleshooting beep for fail open
    // beep();
    return 0;
  }

  int c;
  size_t i = 0;

  // looping through each characters and adding it to our output out_str
  while((c = fgetc(file)) != EOF) {
    // checking if the file is longer than the size of the buffer. If it is return 0
    // so that we can just ask for the user to recalibrate the compass.
    if (i + 1 >= size) {
        out_str[0] = '\0'; // marking output as unusable
        fclose(file);
        return 0;
    }

    out_str[i] = (char)c;
    i++;
  }
  // adding null terminator
  out_str[i] = '\0';

  fclose(file);
  Serial.println();
  Serial.println(out_str);

  return 1;

}

// Writes data into a path. You need to give the path along with the data and sizeof(data), (MBED_LITTLEFS_FILE_PREFIX path, data, sizeof(data))
//
void FS_writeData(const char * path, const char * n, size_t nSize) {
  Serial.print("Writing file: ");
  Serial.println(path);

  // Code was getting stuck here, turns out I forgot to mount the FS using myFS = new LittleFS_MBED();

  FILE *file = fopen(path, "w");

  if (file) {
    Serial.println("File opened");
  } else {
    Serial.println("File failed to open");
    return;
  }

  if (fwrite( (uint8_t *) n, 1, nSize, file )) {
    Serial.println("Writing OK");
  } else {
    // Serial.println("Writing failed");
    // troubleshooting beep for fail
    // beep();
  }

  fclose(file);
}
// ********* Retrieving all calibration data ******* //

void retrieve_calibration_data() {
  char compass_offsets[32];
  char compass_scales[32];
  char accel_biases[32];
  char gyro_biases[32];


  // Recieving offsets
  int compass_offsets_correct = FS_readData(compass_calibration, compass_offsets, sizeof(offset_temp));
  if (compass_offsets_correct) {
    char* xValue = strtok(compass_offsets, ":");
    char* yValue = strtok(NULL, ":");
    char* zValue = strtok(NULL, ":");

    if (xValue != NULL && yValue != NULL && zValue != NULL) {
      offsetX = atof(xValue);
      offsetY = atof(yValue);
      offsetZ = atof(zValue);
    } else {
      Serial.println("compass_offsets: data incorrect format");
    }

  }

  // Recieving scales
  int scales_correct = FS_readData(compass_scales, compass_scales, sizeof(compass_scales));
  if (scales_correct) {
    char* xScale = strtok(compass_scales, ":");
    char* yScale = strtok(NULL, ":");
    char* zScale = strtok(NULL, ":");

    if(xScale != NULL && yScale != NULL && zScale != NULL) {
      scaleX = atof(xScale);
      scaleY = atof(yScale);
      scaleZ = atof(zScale);
    } else {
      Serial.println("compass_scales: data incorrect format");
    }
  }

  int accel_offsets_correct = FS_readData(accelBias, accel_biases, sizeof(accel_biases));
  if (accel_offsets_correct) {
    char* xBias = strtok(accel_biases, ":");
    char* yBias = strtok(NULL, ":");
    char* zBias = strtok(NULL, ":");

    if(xScale != NULL && yScale != NULL && zScale != NULL) {
      accelBiasX = atof(xBias);
      accelBiasY = atof(yBias);
      accelBiasZ = atof(zBias);
    } else {
      Serial.println("accel_biases: data incorrect format");
    }
  }

  int gyro_offsets_correct = FS_readData(gyroBias, gyro_biases, sizeof(gyro_biases));
  if (gyro_offsets_correct) {
    char* xBias = strtok(gyro_biases, ":");
    char* yBias = strtok(NULL, ":");
    char* zBias = strtok(NULL, ":");

    if(xScale != NULL && yScale != NULL && zScale != NULL) {
      gyroBiasX = atof(xBias);
      gyroBiasY = atof(yBias);
      gyroBiasZ = atof(zBias);
    } else {
      Serial.println("gyro_biases: data incorrect format");
    }
  }

}
