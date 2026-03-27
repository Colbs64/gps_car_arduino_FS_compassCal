// ************************   COMPASS FUNCTIONS   ************************//

// This file contains various functions for the compass



// ************************   GET_COMPASS_DATA   ************************//

void get_compass_data(float target_lat, float target_lon) {
  if (hmc_flag) {
    sensors_event_t event;
    compass_HMC.getEvent(&event);
    compass_heading = atan2((event.magnetic.y - offsetY), (event.magnetic.x - offsetX)) * 180.0 / M_PI;  // - compass_offset;
  } else {
    compass_QMC.read();
    compass_heading = compass_QMC.getAzimuth();
  }
  compass_heading = compass_heading - compass_offset;
  gps_heading = atan2(target_lon - gps.location.lng(), target_lat - gps.location.lat()) * 180.0 / M_PI;

  // Once you have your compass_heading, you must then add your 'Declination Angle', which
  // is the 'Error' of the magnetic field in your location in radians.
  // Find yours here: http://www.magnetic-declination.com/
  // Salt Lake is 10°0', or 10.0°

  float declinationAngle = 10.0;
  compass_heading = compass_heading + declinationAngle;

  // Check for wrap due to addition of declination or subtraction of offset.
  if (abs(compass_heading) > 180) {
    if (compass_heading > 180) {
      compass_heading -= 360;
    }
    if (compass_heading < -180) {
      compass_heading += 360;
    }
  }

  heading_error = compass_heading - gps_heading;
  // wrap heading_error so it is between -180 and 180
  if (heading_error < -180) {
    heading_error += 360;
  }
  if (heading_error > 180) {
    heading_error += -360;
  }
}  //End of get_compass_data



// ************************   STOP_NO_COMPASS   ************************//

// This code is meant to be called if the compass is not connected
// It gives the "Compass NOT detected" error on LCD
void stop_no_compass() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(F("Compass NOT detected"));
  lcd.setCursor(0, 2);
  lcd.print(F("Program will not"));
  lcd.setCursor(0, 3);
  lcd.print(F("continue ..."));
  while (1) {
    // Serial.println(F("Ooops, no Compass detected ... Check your wiring!"));
  }
}  //End of stop_no_compass

// ************************   GET_COMPASS_CALIBRATION   ************************//


/*
This function is used to calibrate the compass by measuring values during a 20 second period.
You're supposed to spin the car while this is happening so that the compass can measure maximum and
minimum values for the axis' we're using, x and y. It then uses these to get an average between them ((xMin+xMax)/2)
The logic behind this is to adjust our measurements for the distorted range of measurement. When the compass measures data
it is always going to be distorted due to nearby metals, magnets, or even our motor. What might be good in the future is to
do this calibration while the motor is running so that we can offset any distortions caused by it.
*/
void get_compass_calibration() {
  unsigned long startTime = millis();


  if(LCD_screen == 9 && digitalRead(Sw) == LOW) {
    lcd.setCursor(0, 0);
    lcd.print(F("   compass info   "));
    lcd.setCursor(0, 1);
    lcd.print(F("Calibrating..."));
    lcd.setCursor(0, 2);
    lcd.print(F("spin the car"));

    float xMin = 9999;
    float zMin = 9999;
    float yMin = 9999;
    float yMax = -9999;
    float xMax = -9999;
    float zMax = -9999;

    unsigned long start_time = millis();


    while (millis() - start_time < 20000) { // Spin car for 20 seconds so we can get the highest and lowest values
      sensors_event_t event;
      compass_HMC.getEvent(&event);

      if (event.magnetic.x < xMin) xMin = event.magnetic.x;
      if (event.magnetic.x > xMax) xMax = event.magnetic.x;
      if (event.magnetic.y < yMin) yMin = event.magnetic.y;
      if (event.magnetic.y > yMax) yMax = event.magnetic.y;
      if (event.magnetic.z < zMin) zMin = event.magnetic.z;
      if (event.magnetic.z > zMax) zMax = event.magnetic.z;


      delay(10);
    }

    // calculating Offsets

    offsetX = (xMax + xMin) / 2;
    offsetY = (yMax + yMin) / 2;

    char finalBuffer[15];

    sprintf(finalBuffer, "%.2f:%.2f");

    // writing the data that we just got
    FS_writeData(compass_calibration, finalBuffer, sizeof(finalBuffer));
  }

}


// ************************   RETRIEVE_COMPASS_DATA   ************************//
// This function is used to retrieve the compass data from LittleFS on startup. The values are stored as
// "offsetX:offsetY", this parses both parts and assigns the offsets to the global variables.

void retrieve_Compass_Data() {
  char temp[15];

  int correct = FS_readData(compass_calibration, temp, sizeof(temp));
  if (correct) {

    char* xValue = strtok(temp, ":");

    char* yValue = strtok(NULL, ":");


    offsetX = atof(xValue);
    // Serial.println(values[0]);
    offsetY = atof(yValue);
    // Serial.println(values[1]);
  }




}
