/**
 *  DCC++ commands sniffer
 * 
 * Version: 1.0.0 Date 2020-06-04
 * 
 * Author: Jérôme Rioublanc
 *
 */

#include "main.h"

SD_Record   mySD;
DCC_Packet  myDCC;

/**
 * @brief Arduino setup function
 */
void setup()
 {
  bool  bStatus = true;
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("\n\t\tDCC Snipper\n\tInitialization process started.");
  setupTFT(SCREEN_ORIENTATION);
  splachScreen((char *)APP_VERSION);
  writeText(10, 220, VGA_GREEN, VGA_BLACK, (char *)"TFT : OK -");
  if (!setupSD_Card())
   {
    bStatus = false;
    writeText(100, 220, VGA_RED, VGA_BLACK, (char *)"Carte SD : Erreur");
   }
  else
   {
    writeText(100, 220, VGA_GREEN, VGA_BLACK, (char *)"Carte SD : OK -");
    if(!setupDCC)
     {
      bStatus = false;
      writeText(230, 220, VGA_RED, VGA_BLACK, (char *)"DCC : Erreur");
     }
    else writeText(230, 220, VGA_GREEN, VGA_BLACK, (char *)"DCC : OK");
   }
  if (!bStatus) while (1);                                                     // Infinite loop if the status isn't OK
  delay(3000);
  mySD.openNextFile();
  drawFrame(mySD.getFileName(), mySD.isRecording());
  Serial.println("\n\tModule initialized - \tStart sniffing DCC messages\n\n");
 }

/**
 * @brief Arduino loop function
 *        reads the TFT actions thanks to the buttons
 *        pull out the incomming DCC++ messages for record
 */
void loop()
 {
  static char     cDCCCmd[16];
  static uint8_t  uiAction;

  if (mySD.isRecording())
   {
    if (sprintf(cDCCCmd, "<%u>%u%u%u%u%u.", myDCC.getCommandMnemo(),
                myDCC.getArgument(0), myDCC.getArgument(1), myDCC.getArgument(2),
                myDCC.getArgument(3), myDCC.getArgument(4)) > 0)               // Is it any received command
     {
      Serial.println("<" + String(cDCCCmd) + ">");
      pushCmdList(cDCCCmd);
      if(mySD.isFileOpen() && mySD.isRecording()) checkAction(mySD.write(cDCCCmd));
     }
   }
  uiAction = getScreenAction();
  switch (uiAction)
   {
    case ACTION_PAUSE:                                                         // Pause recording
      mySD.stopRecording();
      drawFileName(mySD.getFileName(), mySD.isRecording());
      break;
    case ACTION_START:                                                         // Start recording
      mySD.startRecording();
      drawFileName(mySD.getFileName(), mySD.isRecording());
      break;
    case ACTION_STOP:                                                          // Stop recording
      mySD.stopRecording();
      mySD.closeFile();
      drawFileName(mySD.getFileName(), mySD.isRecording());
      setButtons();                                                            // Update the buttons' status when needed
      break;
    case ACTION_NEW:                                                           // Open a new record
      if(mySD.isFileOpen()) mySD.closeFile();
      mySD.openNextFile();
      drawFrame(mySD.getFileName(), mySD.isRecording());                       // Refresh the whole screen
      setButtons();                                                            // Update the buttons' status when needed
      break;
    case UNKNOWN_ACTION:
    default:
      break;
   }
 }


/**
 * @brief Organize the buttons' actions
 * 
 * Available actions:
 *    + Pause the DCC signals record
 *    + Start/Restart the DCC signal record
 *    + Close the current record
 *    + Open a new record
 *    + Unknown action
 */
void setButtons(void)
 {
  if (mySD.isFileOpen())
   {
    affectButtonAction(0, ACTION_PAUSE, (char *)PAUSE_LABEL);  // Set to Pause
    affectButtonAction(1, ACTION_START, (char *)START_LABEL);  // Set to Start
    affectButtonAction(2, ACTION_STOP,  (char *)STOP_LABEL);   // Set to Stop
   }
  else
   {
    affectButtonAction(0, ACTION_PAUSE, (char *)PAUSE_LABEL);  // Set to Pause
    affectButtonAction(1, ACTION_START, (char *)START_LABEL);  // Set to Start
    affectButtonAction(2, ACTION_NEW,   (char *)NEW_LABEL);    // Set to New
   }
 }
