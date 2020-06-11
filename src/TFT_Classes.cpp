/**
 *  TFT classes for
 *  HX8357 TFT/SD board
 * 
 * Version: 1.0.0 Date 2020-06-05
 * 
 * Author: Jérôme Rioublanc
 *
 */

#include "TFT_Classes.h"

/**
 * @brief default TS_Point constructor
 */
TS_Point::TS_Point()
 {
  muiX        = 0;
  muiY        = 0;
  muiPressure = 0;
 }

/**
 *  @brief TS_Point constructor with values setting
 * 
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

/**
 * @brief TFT_Button default constructor
 */
TFT_Button::TFT_Button()
 {
  muiX          = 0;
  muiY          = 0;
  muiWidth      = 0;
  muiHeight     = 0;
  muiOutlineCol = 0;
  muiFillCol    = 0;
  muiTextCol    = 0;
  muiAction     = 0xFF;                                                        // No action set
  *mcLabel      = '\0';
 }

/**
 * @brief Record the press action
 *        Update the boolean used to follow the button status
 * 
 * @param bStatus as bool is the new status of the button pressed == true
 * 
 */
void TFT_Button::press(boolean bStatus)
 {
  bLastState = bCurrState;
  bCurrState = bStatus;
 }

/**
 * @brief Functions returning the current status of the button
 * 
 * @return a boolean giving the requested status true == valid action 
 */
bool TFT_Button::justPressed(void)  { return (bCurrState && !bLastState); }
bool TFT_Button::justReleased(void) { return (!bCurrState && bLastState); }
bool TFT_Button::isPressed(void)    { return bCurrState;                  }

/**
 * @brief Check if the pressed point is part of a button
 * 
 * @param uiX as uint16_t is the X coordinate of the point
 * @param uiY as uint16_t is the Y coordinate of the point
 */
bool TFT_Button::contains(TS_Point tsP1)
 {
  return ((tsP1.muiX >= muiX) && (tsP1.muiX < (muiX + muiWidth)) && 
          (tsP1.muiY >= muiY) && (tsP1.muiY < (muiY + muiHeight)));
 }

/**
 * @brief Set all parameters for the button
 * 
 * @param uiX and uiY as uin16_t are the X and Y coordinates
 * @param uiWidth and uiHeight as uin16_t are the width and height
 * @param uiOutlineCol, uiFillCol and uiTextCol as uint16_t are the colors of the outter line, the body and the label
 * @param pcLabel as char * is the label - limited to MAX_STING_LEN - 1 characters
 * @param uiAvtion as uint8_t is the action code associated to the button
 * @param pTFT as UTFT * is the pointer to the TFT variable
 */
void TFT_Button::initButton(uint16_t uiX,          uint16_t uiY,       uint16_t uiWidth,   uint16_t uiHeight, 
                            uint16_t uiOutlineCol, uint16_t uiFillCol, uint16_t uiTextCol, 
                            char *pcLabel,         uint8_t  uiAction,  UTFT     *pTFT)
 {
  muiX          = uiX;
  muiY          = uiY;
  muiWidth      = uiWidth;
  muiHeight     = uiHeight;
  muiOutlineCol = uiOutlineCol;
  muiFillCol    = uiFillCol;
  muiTextCol    = uiTextCol;
  muiAction     = uiAction;
  pmTFT         = pTFT;
  strncpy(mcLabel, pcLabel, MAX_LABEL_LEN - 1);
 }

/**
 * @brief Set and Get functions related to the action code
 * 
 * @param uiAction as uint8_t is the action code
 * @return uiAction as uint8_t is the action code
 */
void    TFT_Button::setAction(uint8_t uiAction) { muiAction = uiAction; }
uint8_t TFT_Button::getAction(void)             { return (muiAction);   }

/**
 * @brief Change the button's label
 * 
 * @param pcLabel as char * is the new label, limited to MAX_LABEL_LEN - 1
 */
void TFT_Button::changeLabel(char *pcLabel)
 {
  strncpy(mcLabel, pcLabel, MAX_LABEL_LEN - 1);
 }

/**
 * @brief draw the button
 *        the colors are set versus the curent status
 * 
 * @param bInverted as bool used to change the colors
 */
void TFT_Button::drawButton(boolean bInverted)
 {
  uint16_t uiFillCol,
           uiOutLineCol,
           uiTextCol;

  if (!bInverted)
   {
    uiFillCol    = muiFillCol;
    uiTextCol    = muiTextCol;
   }
  else
   {
    uiFillCol    = muiTextCol;
    uiTextCol    = muiFillCol;
   }
  uiOutLineCol = muiOutlineCol;                                                // Never change
  pmTFT->setColor(uiFillCol);
  pmTFT->fillRoundRect(muiX, muiY, muiWidth + muiX, muiHeight + muiY);
  pmTFT->setColor(uiOutLineCol);
  pmTFT->drawRoundRect(muiX, muiY, muiWidth + muiX, muiHeight + muiY);
  pmTFT->setBackColor(uiFillCol);
  pmTFT->setColor(uiTextCol);
  pmTFT->setFont(ButtonsFont);
  pmTFT->print(mcLabel, muiX + (muiWidth  / 2) - (pmTFT->getFontXsize() * strlen(mcLabel) / 2), 
                        muiY + (muiHeight / 2) - (pmTFT->getFontYsize() / 2));
 }
