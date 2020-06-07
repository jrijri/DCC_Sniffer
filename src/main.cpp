/**
 *  DCC++ commands sniffer
 * 
 * Version: 1.0.0 Date 2020-06-04
 * 
 * Author: Jérôme Rioublanc
 *
 */

#include "main.h"

TouchScreen_kbv       TS(XP, YP, XM, YM, 300);
TSPoint_kbv           TPoint;

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
  writeText(10, 280, TFT_GREEN, TFT_BLACK, 2, (char *)"TFT : OK");
  if (!setupSD_Card())
   {
    bStatus = false;
    writeText(150, 280, TFT_RED, TFT_BLACK, 2, (char *)"Carte SD : Erreur");
   }
  else
   {
    writeText(150, 280, TFT_GREEN, TFT_BLACK, 2, (char *)"Carte SD : OK");
    if(!setupDCC)
     {
      bStatus = false;
      writeText(340, 280, TFT_RED, TFT_BLACK, 2, (char *)"DCC : Erreur");
     }
    else writeText(340, 280, TFT_GREEN, TFT_BLACK, 2, (char *)"DCC : OK");
   }
  if (!bStatus) while (1);                                                     // Infinite loop if the status isn't OK
  delay(3000);
  drawFrame(createNextFile());
  Serial.println("\n\tModule initialized - \tStart sniffing DCC messages\n\n");
 }

void loop()
 {
  uint8_t uiAction = getScreenAction();
  switch (uiAction)
  {
  case 0:                                                                      // Start sniffing
    if(isFileOpen()) appendToFile((char *)&uiAction);
    break;
  case 1:                                                                      // On-hold sniffing
    /* code */
    break;
  case 2:                                                                      // Stop sniffing
    closeFile();
    break;
  default:
    break;
  }
 }