#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "Arduino.h"

typedef struct _encoder_state_t {
  volatile uint32_t count = 0;
  uint32_t total = 1024*4;
  uint32_t A = 0;
  uint32_t B = 0;
} encoder_state_t;

extern encoder_state_t enc_state;
extern volatile bool enc_isr;

// these are the actual Apollo3 pads (specific to Artemis Nano)
#define ENC_A 11
#define ENC_B 29

#define ENC_CW (+1)
#define ENC_CCW (-1)

void encoder_setup( Stream * pstream );

#endif // _ENCODER_H_
