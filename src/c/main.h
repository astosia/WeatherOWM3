#pragma once
#include <pebble.h>
#define SETTINGS_KEY 1
// A structure containing our settings
typedef struct ClaySettings {

  GColor Back1Color;  //background colour
  GColor FrameColor1; //divider bar color
  GColor Text2Color; //Battery Bar Colour
  GColor Text3Color;  //Date & Temperature Colour
  GColor Text4Color;  //Quiet Time & BT icon colour
  GColor Text7Color; //weathericon colour
  GColor HourColor; //time colour
  char* WeatherTemp;
  char* TempFore;
  char iconnowstring[4];
  char iconforestring[4];
  char tempstring[8];
  char temphistring[10];
  int UpSlider;
  int iconnumbernow;
  int iconnumberfore;
  int WeatherUnit;
  bool CurrentOrFore;
  bool UseForecast;
  char* WindUnit;

 // bool Rotate;
 // bool RightLeft;
} __attribute__((__packed__)) ClaySettings;
