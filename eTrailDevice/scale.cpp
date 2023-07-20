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
  Serial.print("A:");
  Serial.print(uA,1);  
  Serial.print(" B:");
  Serial.print(uB,1);
  Serial.println();
}
