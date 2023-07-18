#define SDOUT_20K  21 
#define CLK_20K  22
#define SDOUT_1K  19 
#define CLK_1K   18
#include "HX711.h"
HX711 scale_20k;
HX711 scale_1k;
#define SCALE_20K 109
#define SCALE_1K 76
  double uA=0;
  double uB=0; 
void scale_setup() {
  scale_20k.begin(SDOUT_20K, CLK_20K);
  scale_20k.tare(); //Reset the scale to 0
  scale_20k.set_scale(SCALE_20K);
  
  scale_1k.begin(SDOUT_1K, CLK_1K);
  scale_1k.set_scale(SCALE_1K);
  scale_1k.tare(); //Reset the scale to 0
}

void scale_tare(){
  Serial.println("****** TARE CMD ******");
  scale_20k.tare(); //Reset the scale to 0
  scale_1k.tare(); //Reset the scale to 0
}
double scale_getValue() {
  uA=scale_20k.get_units();
  uB=scale_1k.get_units();
  return uA+uB;
}

void scale_loop() {

<<<<<<< HEAD
 

  Serial.print("A:");
  Serial.print(uA,1);  
  Serial.print("AOffset:");
  Serial.print(scale_20k.get_offset());  
  Serial.print(" B:");
  Serial.print(uB,1);
  Serial.print("BOffset:");
  Serial.print(scale_1k.get_offset());  
  Serial.println();
=======
 Serial.print("SCALE : Reading: ");
 double u;
 u=scale.get_units(10);
 Serial.print(u, 5);
 Serial.print(" g"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
//  Serial.print(" calibration_factor: ");
//  Serial.print(calibration_factor);
 Serial.println();
//
//  if(Serial.available())
//  {
//    char temp = Serial.read();
//    if(temp == '+' || temp == 'a')
//      calibration_factor += 50;
//    else if(temp == '-' || temp == 'z')
//      calibration_factor -= 50;
//      else if (temp == 't')
//        scale.tare();
//        else if(temp == 'e')
//        {
//          double r=u/72.0f;
//          scale.set_scale(r); 
//        }
//  }
>>>>>>> 3db95b27524fe6a3a0676757a379f5980b390934
}
