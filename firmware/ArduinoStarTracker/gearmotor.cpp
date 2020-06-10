#include "gearmotor.h"

uint8_t gearmotor_speed = 255;

SCMD gearmotor;

void gearmotor_setup( Stream * pstream ){
  gearmotor.settings.commInterface = I2C_MODE;
  gearmotor.settings.I2CAddress = 0x5D;

  while ( gearmotor.begin() != 0xA9 ){
//    pstream->println( "ID mismatch, trying again" );
    delay(500); 
  }
//  pstream->println( "ID matches 0xA9" );

//  pstream->print("Waiting for enumeration...");
  while ( gearmotor.ready() == false );
//  pstream->println("Done.");
//  pstream->println();

  while ( gearmotor.busy() ){};
  gearmotor.enable();
//  pstream->println();

  gearmotor.setDrive( 0, 1, gearmotor_speed);
}

void gearmotor_adjust_speed( int8_t delta ){

  int32_t result = gearmotor_speed + delta;
  if(result > 255){
    result = 255;
  }
  if(result < 0){
    result = 0;
  }
  gearmotor_speed = (uint8_t)result;
  gearmotor.setDrive( 0, 1, gearmotor_speed);
}
