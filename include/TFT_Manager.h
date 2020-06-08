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
#include "./../UTFT/UTFT.h"                                                    // Link to Rinky-Dink Electronics version
#include "./../URTouch/URTouch.h"
//#include "TouchScreen_kbv.h"


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

#define   MAX_STRING_LEN    10                                                 // Button's label max length

#define   SCREEN_ORIENTATION LANDSCAPE
#if SCREEN_ORIENTATION == LANDSCAPE || SCREEN_ORIENTATION == REV_LANDSCAPE
  #define SCREEN_W         320                                                 // Set the number of pixels in width
  #define SCREEN_H         240                                                 // Set the number of pixels in height
#else
  #define SCREEN_W         240                                                 // Set the number of pixels in width
  #define SCREEN_H         320                                                 // Set the number of pixels in height
#endif

#define   MAP_LEFT         902                                                 // TFT correction data
#define   MAP_RIGHT        149                                                 // TFT correction data
#define   MAP_TOP          956                                                 // TFT correction data
#define   MAP_BOTTOM       157                                                 // TFT correction data

#define TFT_BLACK            0x0000
#define TFT_NAVY             0x000F
#define TFT_DARKGREEN        0x03E0
#define TFT_DARKCYAN         0x03EF
#define TFT_MAROON           0x7800
#define TFT_PURPLE           0x780F
#define TFT_OLIVE            0x7BE0
#define TFT_LIGHTGREY        0xC618
#define TFT_DARKGREY         0x7BEF
#define TFT_BLUE             0x001F
#define TFT_GREEN            0x07E0
#define TFT_CYAN             0x07FF
#define TFT_RED              0xF800
#define TFT_MAGENTA          0xF81F
#define TFT_YELLOW           0xFFE0
#define TFT_WHITE            0xFFFF
#define TFT_ORANGE           0xFDA0
#define TFT_GREENYELLOW      0xB7E0
#define TFT_PINK             0xFC9F

class TS_Point
 {
  public:
    TS_Point();
    TS_Point(uint16_t uiX, uint16_t uiY, uint16_t uiPressure);
    bool operator==(TS_Point);
    bool operator!=(TS_Point);
    uint16_t muiX;
    uint16_t muiY;
    uint16_t muiPressure;
 };

class TFT_Button
 {
  public:
    TFT_Button();
    void initButton(uint16_t uiX,          uint16_t uiY,       uint16_t uiWidth,   uint16_t uiHeight, 
                    uint16_t uiOutlineCol, uint16_t uiFillCol, uint16_t uiTextCol, 
                    char *cLabel,          uint8_t uiTextSize);
    void drawButton(bool bInverted = false);
    bool contains(int16_t x, int16_t y);
    void press(bool p);
    bool isPressed(void);
    bool justPressed(void);
    bool justReleased(void);
    bool contains(uint16_t uiX, uint16_t uiY);

  private:
    uint16_t  muiX, muiY;                                                      // Coordinates of top-left corner
    uint16_t  muiWidth, muiHeight;
    uint8_t   muiTextSize;
    uint16_t  muiOutlineCol, muiFillCol, muiTextCol;
    char      mcLabel[MAX_STRING_LEN];
    bool      bCurrState, bLastState;  
 };

bool      setupTFT(uint8_t uiOrientation);
void      writeText(int16_t iX, int16_t iY, uint16_t iFrColor, uint16_t iBkColor, /*uint8_t iSize, const GFXfont *fFont,*/ char *pcText);
void      cleanScreen(uint16_t uiBk_Color, uint16_t uiFr_Color);
void      splachScreen(char *pcVersion);
void      drawFrame(char *pcFileName);
uint8_t   getScreenAction(void);


#endif