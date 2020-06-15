/**
 * Header for the HX8357 manager.
 * Used to call all the dependencies
 * 
 * Version: 1.0.0 Date: 2020-06-05
 * 
 * Author: Jérôme Rioublanc
 * 
 */

#ifndef __TFT_CLASSES_H__
#define __TFT_CLASSES_H__ 1

#include <Arduino.h>
#include "./UTFT.h"                                                            // Link to Rinky-Dink Electronics version
#include "./../URTouch/URTouch.h"

#define   MAX_LABEL_LEN        10                                              // Button's label max length
#define   BOUNCING_DELAY    300UL                                              // buttons' anti-bouncing delay

#define   PAUSE_LABEL           "\"\""
#define   START_LABEL           "! "
#define   STOP_LABEL            "$#"
#define   NEW_LABEL             "&%"

#define   ACTION_PAUSE          0
#define   ACTION_START          1
#define   ACTION_STOP           2
#define   ACTION_NEW            3
#define   UNKNOWN_ACTION        0xFF

#define   TFT_NAVY              0x000F
#define   TFT_DARKGREEN         0x03E0
#define   TFT_DARKCYAN          0x03EF
#define   TFT_LIGHTGREY         0xC618
#define   TFT_DARKGREY          0x7BEF
#define   TFT_MAGENTA           0xF81F
#define   TFT_ORANGE            0xFDA0
#define   TFT_GREENYELLOW       0xB7E0
#define   TFT_PINK              0xFC9F

extern uint8_t SmallFont[];                                                    // Font
extern uint8_t BigFont[];                                                      // Font
extern uint8_t ButtonsFont[];

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
    void    initButton(uint16_t uiX,          uint16_t uiY,       uint16_t  uiWidth,   uint16_t uiHeight, 
                       uint16_t uiOutlineCol, uint16_t uiFillCol, uint16_t  uiTextCol, 
                       char *pcLabel,         uint8_t  uiAction,  UTFT     *pTFT);
    void    drawButton(bool bInverted = false);
    bool    contains(TS_Point tsP1);
    void    press(bool p);
    bool    isPressed(void);
    bool    justPressed(void);
    bool    justReleased(void);
    bool    contains(uint16_t uiX, uint16_t uiY);
    void    setAction(uint8_t uiAction);
    uint8_t getAction(void);
    void    changeLabel(char *pcLabel);

  private:
    UTFT     *pmTFT;
    uint16_t  muiX, muiY;                                                      // Coordinates of top-left corner
    uint16_t  muiWidth, muiHeight;
    uint8_t   muiAction;
    uint16_t  muiOutlineCol, muiFillCol, muiTextCol;
    char      mcLabel[MAX_LABEL_LEN];
    bool      bCurrState, bLastState;  
 };

#endif