// ************************   PID FUNCTIONS   ************************//

// perhaps rename variables so they are all the same or rename them
// to specify which PID they are a part off


// ************************   ESC_PID   ************************//

int esc_pid(int speed) {
  static unsigned long last_PID;

  float dt = 0.02; // set for a frequency of 20hz
  pid_time += pid_delay;

  // static long prev_P;
  static float prev_P_filtered = 0.0;
  int throttle_command, pid_throttle;  // make variables "static" if you uncomment Integral Check

  // The following gains were found from testing on the dyno
  // const float Kp = 0.27;
  // const float Ki = 0.07;
  // const float Kd = 0.25;

  // const float MPH_TO_RPM_ENCODER = 175.36766;  // 5280 ft\mile * 12 in\ft * 2.6 rev_enc\rev_wheel / 15.65625 in\rev_wheel / 60 minutes\hr
  const float MPH_TO_RPM_ENCODER = 67.2;  // 5280 ft\mile * 12 in\ft / (pi*5)in\rev_wheel / 60 minutes\hr

  const long max_I = (esc_full_forward - esc_default) / Ki;  // Determine the max I value for desired max influence

  // rpm = calc_mag_rpm();
  rpm_encoder = calc_mag_rpm();
  // if (rpm_encoder == 0 && armed) {
  //   if (!timer_start) {
  //     no_encoder_countdown = millis();
  //     timer_start = true;
  //     // if it's happening after 1 second we will shut down the car.
  //   } else if(millis() - no_encoder_countdown > 1000) {
  //     currentState = STATE_NO_RPM_READING;
  //   }
  // } else {
  //     timer_start = false;

  // }
  set_rpm_encoder = speed * MPH_TO_RPM_ENCODER;  // this is the Encoder RPM we are trying to achieve


  P = set_rpm_encoder - rpm_encoder;    // current error of rpm
  I = constrain(I + (P * Ki * dt), -max_I, max_I);  // summation of the error of rpm

  // Filtering noise from D term
  float alpha = 0.1;
  float P_filtered = alpha * P + (1-alpha) * prev_P_filtered;
  D = Kd * (P_filtered - prev_P_filtered) / dt;                     // difference between current and previous error

  pid_throttle = P*Kp + I + (long)D;

  throttle_command = constrain(esc_default + pid_throttle, esc_full_reverse, esc_full_forward);  // Keep it in range of the ESC

  // prev_P = P;  // grab error for next time through
  prev_P_filtered = P_filtered;

  return throttle_command;
}

void stop_no_encoder() {
  lcd.clear();
   while (1) {
      lcd.setCursor(0, 0);
      lcd.print(F("No encoder"));
      lcd.setCursor(0, 1);
      lcd.print(F("Check Wiring"));
      delay(500);
  }
}
