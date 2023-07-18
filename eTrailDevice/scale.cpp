

#define DOUT  21 
#define CLK   22
#include <HX711.h>
HX711 scale;


float calibration_factor = 10000; //-7050 worked for my 440lb max scale setup

void scale_setup() {

  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0

//  long zero_factor = scale.read_average(); //Get a baseline reading
//  Serial.print("SCALE: Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
//  Serial.println(zero_factor);
}

double scale_getValue() {

  double u;
  u=scale.get_units();
  return u;
}

void scale_loop() {

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
}
