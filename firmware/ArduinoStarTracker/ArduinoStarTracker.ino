#include "encoder.h"
#include "gearmotor.h"
#include "timebase.h"

#define SERIAL_PORT Serial
#define FORMAT_PLOTTER 0

#define CONTROL_PERIOD_US (100000)





void setup() {
  SERIAL_PORT.begin(1000000);
  delay(2000);
  SERIAL_PORT.print(" \n \n \n");

  encoder_setup(&SERIAL_PORT);
  gearmotor_setup(&SERIAL_PORT);
  timebase_setup(&SERIAL_PORT);
  
}

void loop() {
  update_control();
}

int32_t sign( double x ){
  if(x >= 0){ return 1; }
  return -1;
}

void update_control( void ){
//  if(enc_isr){
//    enc_isr = false;
//    timebase_update_elapsed();
//    SERIAL_PORT.printf("s: %d, m: %d, u: %d, count: 0x%04X\n", elapsed_time.secs, elapsed_time.millis, elapsed_time.micros, enc_state.count);
//  }

  // desired rotational rate:
  // 1 revolution per day
  // w/ a 14" radius 1 revolution is 2*pi*R = 2*pi*14" = 28*pi inches
  // so 1 rev / day is also 28*pi inches per day
  // with a thread pitch of 20 threads per inch that becomes 20*28*pi = 560*pi turns per day (turns are revolutions of the main drive gear)
  // there is an 84:16 gear ratio between the main drive gear and the pinion gear that is used to power it. So in terms of the main motor angular rate that's (84/16)*560*pi periods per day. Or: 2940*pi periods per day
  // periods per day is a pretty useless number - we will be updating the control system much more often than per day. There are 3600*24 = 86400 seconds in a day
  // therefore the main motor should complete (2940*pi)/86400 periods per second
  // we can measure the main motor's angular rate with the encoder - it has identical gearing so the encoder's angular rate equals the motor's anglular rate
  // however, the encoder has 4096 positions per period - so in terms of positions per second the encoder's angular rate should be (2940*4096*pi)/(86400), or (12042240*pi)/86400 pulses per second (that's about 437.868 pulses per second for anyone willing to approximate)

  // now consider that the end goal is to track star's *positions* in the sky. so far we have only discussed the desired rotational rate of the system. any error in that will result in error in the 
  // final position of the sytsem wrt to the stars. to avoid this we need to keep track of position error as well.

  static elapsed_time_t prev_time;
  static elapsed_time_t delta_time;
  
  
  timebase_update_elapsed();

  delta_time.micros = elapsed_time.micros - prev_time.micros;
  if((delta_time.micros) >= CONTROL_PERIOD_US){ // it has been about one second
    static uint32_t prev_pulses = 0;
    static int32_t delta_pulses = 0;
    delta_pulses = enc_state.count - prev_pulses;

    // since my motor is currently too slow.... let's set a fake target pulse per second
    double target_pps = 10; // fake
//    double target_pps = ((double)12042240/86400)*3.1415926535;
    double actual_pps = ((double)((1000000)*delta_pulses)/(delta_time.micros));

    static double target_pos = 0;
    target_pos += target_pps * ((double)delta_time.micros/1000000); // accumulate the position
    static double actual_pos = 0;
    actual_pos += delta_pulses;

    // calculate the errors
    
    double error_pps = actual_pps - target_pps;
    static double prev_error_pos = 0;
    double error_pos = actual_pos - target_pos;
    double error_pos_velocity = error_pos - prev_error_pos;
    prev_error_pos = error_pos;


    // make changes to motor speed based on errors
    int32_t delta_speed = (int32_t)(-0.1 * error_pos) + (int32_t)(-3.0*error_pos_velocity);

    gearmotor_adjust_speed(delta_speed);


#if FORMAT_PLOTTER
    SERIAL_PORT.printf("%f %f\n", error_pos, error_pps);
#else
    SERIAL_PORT.printf("tpos: %f, apos: %f, epos %f, tpps %f, apps: %f, epps %f, [dt: %d, dp: %d]\n", target_pos, actual_pos, error_pos, target_pps, actual_pps, error_pps, delta_time.micros, delta_pulses);
#endif
    
    prev_time = elapsed_time;
    prev_pulses = enc_state.count;
  }  
}
