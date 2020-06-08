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

UTFT       TFT(HX8347I,38,39,40,41);
URTouch    TS(6, 5, 4, 3, 2);
TS_Point   TPoint;
TFT_Button butCmd[3];

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

TS_Point::TS_Point()
 {
  muiX        = 0;
  muiY        = 0;
  muiPressure = 0;
 }

/*!
 *  @brief  TS_Point constructor
 *  @param  x0
 *          Initial x
 *  @param  y0
 *          Initial y
 *  @param  z0
 *          Initial z
 */
TS_Point::TS_Point(uint16_t uiX, uint16_t uiY, uint16_t uiPressure)
 {
  muiX        = uiX;
  muiY        = uiY;
  muiPressure = uiPressure;
 }

/*!
 *  @brief  Equality operator for TS_Point
 *  @return True if points are equal
 */
bool TS_Point::operator==(TS_Point tsP1)
 {
  return ((tsP1.muiX == muiX) && (tsP1.muiY == muiY) && (tsP1.muiPressure == muiPressure));
 }

/*!
 *  @brief  Non-equality operator for TS_Point
 *  @return True if points are not equal
 */
bool TS_Point::operator!=(TS_Point tsP1)
 {
  return ((tsP1.muiX != muiX) || (tsP1.muiY != muiY) || (tsP1.muiPressure != muiPressure));
 }

TFT_Button::TFT_Button()
 {
  muiX          = 0;
  muiY          = 0;
  muiWidth      = 0;
  muiHeight     = 0;
  muiOutlineCol = 0;
  muiFillCol    = 0;
  muiTextCol    = 0;
  muiTextSize   = 0;
  *mcLabel      = '\0';
 }

void TFT_Button::press(boolean bStatus)
 {
  bLastState = bCurrState;
  bCurrState = bStatus;
 }

bool TFT_Button::justPressed(void)  { return (bCurrState && !bLastState); }
bool TFT_Button::justReleased(void) { return (!bCurrState && bLastState); }
bool TFT_Button::isPressed(void)    { return bCurrState; }
bool TFT_Button::contains(uint16_t uiX, uint16_t uiY)
 {
  return ((uiX >= muiX) && (uiX < (muiX + muiWidth)) && (uiY >= muiY) && (uiY < (muiY + muiHeight)));
 }

void TFT_Button::initButton(uint16_t uiX,          uint16_t uiY,       uint16_t uiWidth,   uint16_t uiHeight, 
                            uint16_t uiOutlineCol, uint16_t uiFillCol, uint16_t uiTextCol, 
                            char *cLabel,          uint8_t uiTextSize)
 {
  muiX          = uiX;
  muiY          = uiY;
  muiWidth      = uiWidth;
  muiHeight     = uiHeight;
  muiOutlineCol = uiOutlineCol;
  muiFillCol    = uiFillCol;
  muiTextCol    = uiTextCol;
  muiTextSize   = uiTextSize;
  strncpy(mcLabel, cLabel, MAX_STRING_LEN - 1);
 }

void TFT_Button::drawButton(boolean bInverted)
 {
  uint16_t uiFillCol,
           uiOutLineCol,
           uiTextCol;

  if (!bInverted)
   {
    uiFillCol    = muiFillCol;
    uiOutLineCol = muiOutlineCol;
    uiTextCol    = muiTextCol;
   }
  else
   {
    uiFillCol    = muiTextCol;
    uiOutLineCol = muiOutlineCol;
    uiTextCol    = muiFillCol;
   }
//  uiRadius = min(muiWidth, muiHeight) / 4;                                     // Corner radius
  TFT.setColor(uiFillCol);
  TFT.fillRoundRect(muiX, muiY, muiWidth + muiX, muiHeight + muiY);
  TFT.setColor(uiOutLineCol);
  TFT.drawRoundRect(muiX, muiY, muiWidth + muiX, muiHeight + muiY);
  TFT.setBackColor(uiFillCol);
  TFT.setColor(uiTextCol);
  TFT.print(mcLabel, muiX + (muiWidth  / 2) - (strlen(mcLabel) * 3 * muiTextSize),
                     muiY + (muiHeight / 2) - (4 * muiTextSize));
 }

bool setupTFT(uint8_t uiOrientation)
 {
  bool      bStatus   = true;

  Serial.print("\nInitializing the TFT ...\n");
  TFT.InitLCD(uiOrientation);
  TS.InitTouch();
  TS.setPrecision(PREC_MEDIUM);
  TFT.setFont(SmallFont);
  butCmd[0].initButton( 20, 125, 60, 40, TFT_RED, TFT_MAROON, TFT_RED, (char *)"||", 1);
  butCmd[1].initButton(130, 125, 60, 40, TFT_RED, TFT_MAROON, TFT_RED, (char *)"|>", 1);
  butCmd[2].initButton(240, 125, 60, 40, TFT_RED, TFT_MAROON, TFT_RED, (char *)"[]", 1);
  return(bStatus);
 }

/**
 * @brief Draw the frame of the main screen
 */
void drawFrame(char *pcFileName)
 {
  cleanScreen(TFT_BLACK, TFT_YELLOW);
  TFT.setColor(TFT_YELLOW);
  TFT.drawHLine(3,  35, TFT.getDisplayXSize() - 6);
  TFT.drawHLine(3,  70, TFT.getDisplayXSize() - 6);
  TFT.drawHLine(3, 105, TFT.getDisplayXSize() - 6);
  writeText(100, 13, TFT_GREEN, TFT_BLACK, (char *)"Flux DCC++");
  writeText(10, 47, TFT_WHITE, TFT_BLACK, (char *)"> ");
  writeText(10, 82, TFT_GREENYELLOW, TFT_BLACK, (char *)strcat((char *)"Fichier : ", pcFileName));
  butCmd[0].drawButton(false);
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
void writeText(int16_t iX, int16_t iY, uint16_t iFrColor, uint16_t iBkColor, /*uint8_t iSize, const GFXfont *fFont,*/ char *pcText)
 {
  //  TFT.setFont(fFont);
  TFT.setBackColor(iBkColor);
  TFT.setColor(iFrColor);
  TFT.print(pcText, iX, iY);
 }

/**
 * @brief Read the Touchscreen status and set the Touched point
 *        and update the status
 */
bool isPressed(void)
 {
  bool  bState = false;

  if (TS.dataAvailable())
   {
    TS.read();
    bState = true;
    TPoint =  TS_Point(TS.getX(), TS.getY(), 0);
   }
  return bState;
 }

/**
 * @brief clean the screen and
 *        draw the line around the screen
 *        
 */
void cleanScreen(uint16_t uiBk_Color, uint16_t uiFr_Color)
 {
  TFT.setBackColor(uiBk_Color);
  TFT.clrScr();
  TFT.setColor(uiFr_Color);
  TFT.drawHLine(1,            1,            SCREEN_W - 2);
  TFT.drawVLine(SCREEN_W - 2, 1,            SCREEN_H - 2);
  TFT.drawHLine(SCREEN_W - 2, SCREEN_H - 2, -(SCREEN_W -2));
  TFT.drawVLine(1,            SCREEN_H - 2, -(SCREEN_H - 2));
 }

/**
 * @brief display the Splach screen during the initialization step
 * @param pcVersion as char* is the name of the current version
 */
void splachScreen(char *pcVersion)
 {
  char      cMessage [32];
  uint16_t  uiX_Pos;

  cleanScreen(TFT_BLACK, TFT_YELLOW);
  sprintf(cMessage, "Version : %s", pcVersion);
  uiX_Pos = (SCREEN_W - (strlen(cMessage) * 16)) / 2;
  Serial.println(uiX_Pos);
  TFT.setFont(BigFont);
  writeText(72,  50, TFT_RED, TFT_BLACK, (char *)"DCC Snipper");
  writeText(uiX_Pos, 110, TFT_WHITE, TFT_BLACK, cMessage);
  TFT.setFont(SmallFont);
  writeText(104, 150, TFT_RED, TFT_BLACK, (char *)"Initialisation");
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

  if (isPressed())
   {
    for (uint8_t uiInd = 0; uiInd < 3; uiInd++)
      {
       butCmd[uiInd].press(butCmd[uiInd].contains(TPoint.muiX, TPoint.muiY));
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
