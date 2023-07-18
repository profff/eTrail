
#ifndef SCALE_H
#define SCALE_H
#include "HX711.h"
void scale_setup();
void scale_loop();
double scale_getValue();
void scale_tare();
extern HX711 scale;

#endif
