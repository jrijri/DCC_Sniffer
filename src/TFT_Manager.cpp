/**
 *  TFT manager for
 *  HX8357 TFT/SD board
 * 
 * Version: 1.0.0 Date 2020-06-05
 * 
 * Author: Jérôme Rioublanc
 *
 */

#include "TFT_Manager.h"

MCUFRIEND_kbv TFT;                                                             // Main variable managing the display
Adafruit_GFX_Button butCmd[3];

bool setupTFT(uint8_t uiOrientation)
 {
  bool      bStatus   = true;
  uint16_t  uiID;
  char      cMessage [128];

  Serial.print("\nInitializing the TFT ...\n");
  uiID = TFT.readID();
  sprintf(cMessage, "Found Screen driver: 0x%04X", uiID);
  Serial.println(cMessage);
  TFT.begin(uiID);
  TFT.setRotation(uiOrientation);
  butCmd[0].initButton(&TFT,  90, 200, 80, 60, TFT_MAROON, TFT_MAROON, TFT_RED, (char *)"||", 4);
  butCmd[1].initButton(&TFT, 240, 200, 80, 60, TFT_MAROON, TFT_MAROON, TFT_RED, (char *)"|>", 4);
  butCmd[2].initButton(&TFT, 390, 200, 80, 60, TFT_MAROON, TFT_MAROON, TFT_RED, (char *)"[]", 4);
  return(bStatus);
 }

/**
 * @brief Draw the frame of the main screen
 */
void drawFrame(void)
 {
  cleanScreen(TFT_BLACK, TFT_YELLOW);
  writeText(150, 13, TFT_GREEN, TFT_BLACK, 3, (char *)"Flux DCC++");
  TFT.drawFastHLine(3,  45, SCREEN_W - 6, TFT_YELLOW);
  writeText(10, 57, TFT_WHITE, TFT_BLACK, 3, (char *)">");
  TFT.drawFastHLine(3,  90, SCREEN_W - 6, TFT_YELLOW);
  writeText(10, 102, TFT_GREENYELLOW, TFT_BLACK, 3, (char *)"Fichier : ");
  TFT.drawFastHLine(3, 135, SCREEN_W - 6, TFT_YELLOW);
  butCmd[0].drawButton(true);
  butCmd[1].drawButton(false);
  butCmd[2].drawButton(false);
 }

/**
 * @brief Display a message using coordinates, colors, size and font
 * @param iX is the X coordinate of the left/lower corner
 * @param iY is the Y coordinate of the left/lower corner
 * @param iFrColor is the message color
 * @param iBkColor is the background color, important to clean the previous message
 * @param iSize is the text size, for 1 the char is 6x8 pixels, then use multiples
 * NOT SET @param fFont is a pointer to the font to be used, NULL for the default one
 * @param pcText is a pointer to the text set as (char *)
 */
void writeText(int16_t iX, int16_t iY, uint16_t iFrColor, uint16_t iBkColor, uint8_t iSize,/* const GFXfont *fFont,*/ char *pcText)
 {
//  TFT.setFont(fFont);
  TFT.setCursor(iX, iY);
  TFT.setTextColor(iFrColor, iBkColor);
  TFT.setTextSize(iSize);
  TFT.print(pcText);
 }

/**
 * @brief Read the Touchscreen status and set the Touched point
 */
void readResistiveTouch(void)
 {
  TPoint = TS.getPoint();
  pinMode(YP, OUTPUT);                                                         //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);                                                      //because TFT controls pins too
  digitalWrite(XM, HIGH);
  TPoint.x = map(TPoint.x, MAP_LEFT, MAP_RIGHT, 0, SCREEN_W);                  // Settings for PORTRAIT orientation
  TPoint.y = map(TPoint.y, MAP_BOTTOM, MAP_TOP, 0, SCREEN_H);
 }

/**
 * @brief read the Touchscreen status calling an internal function
 *        and update the status
 */
bool isPressed(void)
 {
  int   iCount      = 0;
  bool  bState,
        bOldState;

  while (iCount < 10)
   {
    readResistiveTouch();
    bState = TPoint.z > 200;                                                   // Value to be adjusted to suit the screen [20 ... 250]
    if (bState == bOldState) iCount++;
    else                     iCount = 0;
    bOldState = bState;
    delay(5);
   }
  return bOldState;
 }

/**
 * @brief clean the screen and
 *        draw the line around the screen
 *        
 */
void cleanScreen(uint16_t uiBk_Color, uint16_t uiFr_Color)
 {
  TFT.fillScreen(uiBk_Color);
  TFT.drawFastHLine(1,            1,            SCREEN_W - 2,    uiFr_Color);
  TFT.drawFastVLine(SCREEN_W - 2, 1,            SCREEN_H - 2,    uiFr_Color);
  TFT.drawFastHLine(SCREEN_W - 2, SCREEN_H - 2, -(SCREEN_W -2),  uiFr_Color);
  TFT.drawFastVLine(1,            SCREEN_H - 2, -(SCREEN_H - 2), uiFr_Color);
 }

/**
 * @brief display the Splach screen during the initialization step
 * @param pcVersion as char* is the name of the current version
 */
void splachScreen(char *pcVersion)
 {
  char      cMessage [128];
  uint16_t  uiX_Pos;

  cleanScreen(TFT_BLACK, TFT_YELLOW);
  sprintf(cMessage, "Version : %s", pcVersion);
  uiX_Pos = (SCREEN_W >> 1) - strlen(cMessage) * 6;
  writeText(141, 100, TFT_RED, TFT_BLACK, 3, (char *)"DCC Snipper");
  writeText(uiX_Pos, 150, TFT_WHITE, TFT_BLACK, 2, cMessage);
  writeText(156, 250, TFT_RED, TFT_BLACK, 2, (char *)"Initialisation");
 }

/**
 * @brief Manage the buttons' actions on the main screen
 * @return the pressed button ID [0..N] as uint8_t
 * 
 * Buttons settings:
 *    - 0x00 = Pause recording
 *    - 0x01 = Start recording
 *    - 0x02 = Stop recording
 *    - 0xFF   Otherwise
 */
uint8_t getScreenAction(void)
 {
  uint8_t        uiBut = 0xFF;
  static uint8_t uiLastAction;

  if (isPressed())
   {
    for (uint8_t uiInd = 0; uiInd < 3; uiInd++)
      {
       butCmd[uiInd].press(butCmd[uiInd].contains(TPoint.x, TPoint.y));
       if (butCmd[uiInd].justPressed())  
        {
         butCmd[uiInd].drawButton(true);
         uiBut = uiInd;
        }
       if (butCmd[uiInd].justReleased())
        {
         butCmd[uiInd].drawButton();
        }
      }
   }
  return(uiBut);
 }
