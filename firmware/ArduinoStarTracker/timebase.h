#ifndef _TIMEBASE_H_
#define _TIMEBASE_H_

#include "Arduino.h"

#define ET_SYSTICKS_PER_MAJOR (0xFFFFFFFF)

typedef struct _elapsed_time_t {
  uint32_t systicks_major = 0;
  uint32_t systicks_minor = 0;
  
  uint32_t micros = 0;
  uint32_t millis = 0;
  uint32_t secs = 0;
} elapsed_time_t;

extern elapsed_time_t elapsed_time;

void timebase_setup( Stream * pstream );

void timebase_update_elapsed( void );

#endif // _TIMEBASE_H_
