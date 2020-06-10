#ifndef _GEARMOTOR_H_
#define _GEARMOTOR_H_

#include "Arduino.h"

#include <SCMD_config.h>
#include <SCMD.h>

extern SCMD gearmotor;

void gearmotor_setup( Stream * pstream );

void gearmotor_adjust_speed( int8_t delta );

#endif // _GEARMOTOR_H_
