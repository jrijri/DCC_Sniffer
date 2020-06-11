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

/**
 * Global variables
 */
UTFT          TFT(HX8347I,38,39,40,41);                                        // Set the parameters for the TFT connection
URTouch       TS(6, 5, 4, 3, 2);                                               // Parameters for the TouchScreen

TS_Point      TPoint;                                                          // TouchScreen point
TFT_Button    butCmd[3];                                                       // Buttons displayed on the TouchScreen
unsigned long ulLastCmdStamp;                                                  // Time stamp of the last button press
char          cMessages[6][32];                                                // List of messages displayed (DCC commands)



/**
 * @brief Initialize the TFT display
 *        Set the TFT orientation
 *        Set the TouchScreen precision
 *        Set the font and the buttons
 *        Reset the action stamp variable
 * 
 * @return bStatus as boolean - always true
 */
bool setupTFT(uint8_t uiOrientation)
 {
  bool      bStatus   = true;

  Serial.print("\nInitializing the TFT ...\n");
  ulLastCmdStamp = millis();                                                   // Set the time counter
  TFT.InitLCD(uiOrientation);
  TS.InitTouch();
  TS.setPrecision(PREC_MEDIUM);
  TFT.setFont(SmallFont);
  butCmd[0].initButton( 20, 180, 60, 40, VGA_RED, VGA_MAROON, VGA_RED, PAUSE_LABEL, 0, &TFT);
  butCmd[1].initButton(130, 180, 60, 40, VGA_RED, VGA_MAROON, VGA_RED, START_LABEL, 1, &TFT);
  butCmd[2].initButton(240, 180, 60, 40, VGA_RED, VGA_MAROON, VGA_RED, STOP_LABEL, 2, &TFT);
  Serial.println("Done");
  return(bStatus);
 }

/**
 * @brief display the file name in the frame
 * 
 * @param pcFileName as char * is the file name string
 * @param bIsRecording as bool is the recording status
 */
void drawFileName(char *pcFileName, bool bIsRecording)
 {
  char  cPath[32];

  sprintf(cPath, "Fichier : %s", pcFileName);
  TFT.setFont(SmallFont);
  writeText(10, 37, TFT_GREENYELLOW, VGA_BLACK, (char *)cPath);
  if(bIsRecording) TFT.setColor(VGA_RED);
  else             TFT.setColor(VGA_BLACK);
  TFT.fillCircle(300, 43, 6);                                                  // Add a RED disk when recording
 }

/**
 * @brief Draw the frame of the main screen
 */
void drawFrame(char *pcFileName, bool bIsRecording)
 {
  char  pcNum[4];

  cleanScreen(VGA_BLACK, VGA_YELLOW);
  TFT.setColor(VGA_YELLOW);
  TFT.drawHLine(3,  35, TFT.getDisplayXSize() - 6);
  TFT.drawHLine(3,  51, TFT.getDisplayXSize() - 6);
  TFT.drawHLine(3, 160, TFT.getDisplayXSize() - 6);
  TFT.setFont(BigFont);
  writeText(CENTER,  8, TFT_GREENYELLOW, VGA_BLACK, (char *)"Flux DCC++");
  TFT.setFont(SmallFont);
  drawFileName(pcFileName, bIsRecording);
  writeText(5, 145, VGA_WHITE, VGA_BLACK, (char *)" > ");
  for( uint8_t uiInd = 1; uiInd < 6; uiInd++)
   {
    sprintf(pcNum, "-%1u:", uiInd);
    writeText(5, 145 - (uiInd * 16), TFT_LIGHTGREY, VGA_BLACK, pcNum);
   }
  butCmd[0].drawButton(false);
  butCmd[1].drawButton(false);
  butCmd[2].drawButton(false);
 }

/**
 * @brief Push the old commands up and
 *        Add the new one at the bottom
 * 
 * @param pcMessage as char * is the new command string
 */
void pushCmdList(char *pcMessage)
 {
  for (uint8_t uiInd = 5; uiInd > 0; uiInd--)                                  // Move the old messages up
   {
    strcpy(cMessages[uiInd], cMessages[uiInd - 1]);
    writeText(30, 145 - (uiInd * 16), TFT_LIGHTGREY, VGA_BLACK, cMessages[uiInd]);
   }
  strncpy(cMessages[0], pcMessage, 30);
  writeText(30, 145, VGA_WHITE, VGA_BLACK, pcMessage);                         // Display the new message
 }

/**
 * @brief Display a message using coordinates, colors, size and font
 * @param iX is the X coordinate of the left/lower corner
 * @param iY is the Y coordinate of the left/lower corner
 * @param iFrColor is the message color
 * @param iBkColor is the background color, important to clean the previous message
 * @param pcText is a pointer to the text set as (char *)
 */
void writeText(int16_t iX, int16_t iY, uint16_t iFrColor, uint16_t iBkColor, char *pcText)
 {
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
    TPoint =  TS_Point(SCREEN_W - TS.getX(), TS.getY(), 0);
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

  cleanScreen(VGA_BLACK, VGA_YELLOW);
  sprintf(cMessage, "Version : %s", pcVersion);
  TFT.setFont(BigFont);
  writeText(CENTER,  50, VGA_RED, VGA_BLACK, (char *)"DCC Snipper");
  writeText(CENTER, 110, VGA_WHITE, VGA_BLACK, cMessage);
  TFT.setFont(SmallFont);
  writeText(CENTER, 150, VGA_RED, VGA_BLACK, (char *)"Initialisation");
 }

/**
 * @brief Manage the buttons' actions on the main screen
 * 
 * @return the pressed button action as uint8_t
 */
uint8_t getScreenAction(void)
 {
  uint8_t        uiBut = 0xFF;

  if (isPressed())
   {
    if ((millis() - ulLastCmdStamp) > BOUNCING_DELAY)                          // Avoid bouncing effect when pressing TFT
     {
      ulLastCmdStamp = millis();
      for (uint8_t uiInd = 0; uiInd < 3; uiInd++)
       {
        butCmd[uiInd].press(butCmd[uiInd].contains(TPoint));
        if (butCmd[uiInd].justPressed())  
         {
          butCmd[uiInd].drawButton(true);
          uiBut = butCmd[uiInd].getAction();
         }
        if (butCmd[uiInd].justReleased())
         {
          butCmd[uiInd].drawButton();
         }
       }
     }
   }
  return(uiBut);
 }

void checkAction(bool bStatus)
 {
  if (!bStatus)
   {
    TFT.setFont(SmallFont);
    writeText(10, 37, VGA_RED, VGA_BLACK, (char *)"!! ERREUR CARTE SD - BLOQUAGE !!");
    while(1);
   }
 }

void affectButtonAction(uint8_t uiNum, uint8_t uiAction, char *pcLabel)
 {
  if ((uiNum >= 0) && (uiNum < 3))
   {
    butCmd[uiNum].setAction(uiAction);
    butCmd[uiNum].changeLabel(pcLabel);
    butCmd[uiNum].drawButton(false);
   }
 }