#include "encoder.h"

encoder_state_t enc_state;
volatile bool enc_isr = false;

ap3_gpio_pin_t ap3_rba_nano_pad2pin( ap3_gpio_pad_t pad ){
  switch(pad){
    case ENC_A : return 2; break;
    case ENC_B : return 3; break;
    default: return AP3_GPIO_PAD_UNUSED;
  }
}

// encoder isrs fire on changes for 4x sampling
void enc_isr_A(void* arg){
  if(arg == NULL){ return; }
  encoder_state_t* pstate = (encoder_state_t*)arg;
  uint32_t A = am_hal_gpio_input_read(ENC_A);
  uint32_t B = am_hal_gpio_input_read(ENC_B);
  pstate->count += A ? (B ? ENC_CCW : ENC_CW) : (B ? ENC_CW : ENC_CCW);
  enc_isr = true;
}

void enc_isr_B(void* arg){
  if(arg == NULL){ return; }
  encoder_state_t* pstate = (encoder_state_t*)arg;
  uint32_t A = am_hal_gpio_input_read(ENC_A);
  uint32_t B = am_hal_gpio_input_read(ENC_B);
  pstate->count += B ? (A ? ENC_CW : ENC_CCW) : (A ? ENC_CCW : ENC_CW);
  enc_isr = true;
}

void encoder_setup( Stream * pstream ){
  pinMode(ap3_rba_nano_pad2pin(ENC_A), INPUT);
  pinMode(ap3_rba_nano_pad2pin(ENC_B), INPUT);

  attachInterruptArg(digitalPinToInterrupt(ap3_rba_nano_pad2pin(ENC_A)), enc_isr_A, &enc_state, CHANGE);
  attachInterruptArg(digitalPinToInterrupt(ap3_rba_nano_pad2pin(ENC_B)), enc_isr_B, &enc_state, CHANGE);
}
