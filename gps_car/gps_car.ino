#include <TinyGPS.h>

//============= Arduino Code for GPS Car ==============//
// Slick Science SSAV (Small Scale Autonomous Vehicle)
//=====================================================//

/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                          BEGIN STUDENT-ADJUSTABLE SECTION                           //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////

// Student-avaliable variables...
const char team_name[] = "   Slick Science    ";
const char sub_name[] = "(put sub-name here) ";

byte min_dist_to_tgt = 2;   // minimum distance (in meters) to target before moving on to next target - More accurate closer, but harder
byte delay_at_target = 2;   // delay for x seconds (and wag servo) once at gps target before going to next target - makes it more obvious when you get there
int servo_wag_speed = 250;  // milliseconds waiting between left/right - make smaller for faster wag, larger for slower

// Neo-pixel colors  (0 -> 255)
byte neo_red = 100;
byte neo_green = 0;
byte neo_blue = 250;

const char course_name[] = "    Day 1 Demo      ";
const byte num_gps_tgts = 7;
const float target_lats[num_gps_tgts]  = { 40.671649,   40.671460,   40.671635, 40.671510};
const float target_longs[num_gps_tgts] = {-111.946054, -111.946196, -111.946217, -111.945980};

/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                           END STUDENT-ADJUSTABLE SECTION                            //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////-
