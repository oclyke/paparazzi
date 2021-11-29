#include "timebase.h"

elapsed_time_t elapsed_time;



void timebase_setup( Stream * pstream ){
  // do nothing for now
}

void timebase_update_elapsed( void ){
//  elapsed_time.systicks_major = sysoverflows();
//  elapsed_time.systicks_minor = systicks();

//  uint64_t ticks = (elapsed_time.systicks_major * ET_SYSTICKS_PER_MAJOR) + elapsed_time.systicks_minor;
//
//  
//  elapsed_time.micros = (uint32_t)(ticks / AP3_STIMER_FREQ_MHZ);
//  elapsed_time.millis = (uint32_t)(ticks / AP3_STIMER_FREQ_KHZ);
//  elapsed_time.secs = (uint32_t)(ticks / AP3_STIMER_FREQ_HZ);

  elapsed_time.micros = micros();
  elapsed_time.millis = millis();
  elapsed_time.secs = millis()/1000;
}
