#pragma once
#include <pebble.h>
#define SETTINGS_KEY 1
// A structure containing our settings
typedef struct ClaySettings {

//  GColor Back1Color;
  GColor FrameColor1;
  GColor Text1Color;
  GColor Text2Color;
  GColor Text3Color;
  GColor Text4Color;
  GColor Text5Color;
  GColor Text6Color;
  GColor Text7Color;
  GColor HourColor;
  GColor MinColor;
  bool ALIEN;
  char* WeatherTemp;
  char* TempFore;
  char iconnowstring[4];
  char iconforestring[4];
  char tempstring[8];
  char temphistring[10];
  int UpSlider;
  int WeatherUnit;
  char* WindUnit;

 // bool Rotate;
 // bool RightLeft;
} __attribute__((__packed__)) ClaySettings;
