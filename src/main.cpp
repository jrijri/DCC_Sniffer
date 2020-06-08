/**
 *  DCC++ commands sniffer
 * 
 * Version: 1.0.0 Date 2020-06-04
 * 
 * Author: Jérôme Rioublanc
 *
 */

#include "main.h"


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
  writeText(10, 220, TFT_GREEN, TFT_BLACK, (char *)"TFT : OK");
  if (!setupSD_Card())
   {
    bStatus = false;
    writeText(100, 220, TFT_RED, TFT_BLACK, (char *)"Carte SD : Erreur");
   }
  else
   {
    writeText(100, 220, TFT_GREEN, TFT_BLACK, (char *)"Carte SD : OK");
    if(!setupDCC)
     {
      bStatus = false;
      writeText(230, 220, TFT_RED, TFT_BLACK, (char *)"DCC : Erreur");
     }
    else writeText(230, 220, TFT_GREEN, TFT_BLACK, (char *)"DCC : OK");
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
    Serial.print("0 - ");
    break;
  case 1:                                                                      // On-hold sniffing
    Serial.print("1 - ");
    break;
  case 2:                                                                      // Stop sniffing
    Serial.print("2 - ");
    closeFile();
    break;
  default:
    break;
  }
 }