/**
 * Header for the HX8357 manager.
 * Used to call all the dependencies
 * 
 * Version: 1.0.0 Date: 2020-06-05
 * 
 * Author: Jérôme Rioublanc
 * 
 */

#ifndef __TFT_MANANGER_H__
#define __TFT_MANANGER_H__ 1

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "TouchScreen_kbv.h"


// These are 'flexible' lines that can be changed
#define   TFT_RD            A0
#define   TFT_WR            A1
#define   TFT_RS            A2
#define   TFT_CS            A3
#define   TFT_RST           A4                                                 // RST can be set to -1 if you tie it to Arduino's reset

#define   XP                 7                                                 // Pin used by the TouchScreen manager
#define   XM                A1                                                 // Pin used by the TouchScreen manager
#define   YP                A2                                                 // Pin used by the TouchScreen manager
#define   YM                 6                                                 // Pin used by the TouchScreen manager

#define   PORTRAIT           0                                                 // Set the screen orientation
#define   LANDSCAPE          1                                                 // Set the screen orientation
#define   REV_PORTAIT        2                                                 // Set the screen orientation
#define   REV_LANDSCAPE      3                                                 // Set the screen orientation

#define   SCREEN_ORIENTATION REV_LANDSCAPE
#if SCREEN_ORIENTATION == LANDSCAPE || SCREEN_ORIENTATION == REV_LANDSCAPE
  #define SCREEN_W         480                                                 // Set the number of pixels in width
  #define SCREEN_H         320                                                 // Set the number of pixels in height
#else
  #define SCREEN_W         320                                                 // Set the number of pixels in width
  #define SCREEN_H         480                                                 // Set the number of pixels in height
#endif

#define   MAP_LEFT         902                                                 // TFT correction data
#define   MAP_RIGHT        149                                                 // TFT correction data
#define   MAP_TOP          956                                                 // TFT correction data
#define   MAP_BOTTOM       157                                                 // TFT correction data

extern TouchScreen_kbv       TS;                                               // Main variable managing the touchpad
extern TSPoint_kbv           TPoint;                                           // Main variable managing the touched point

bool      setupTFT(uint8_t uiOrientation);
void      writeText(int16_t iX, int16_t iY, uint16_t iFrColor, uint16_t iBkColor, uint8_t iSize,/* const GFXfont *fFont,*/ char *pcText);
void      cleanScreen(uint16_t uiBk_Color, uint16_t uiFr_Color);
void      splachScreen(char *pcVersion);
void      drawFrame(void);
uint8_t   getScreenAction(void);


#endif