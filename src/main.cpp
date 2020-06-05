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
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("\n\t\tDCC Snipper\n\tInitialization process started.");
  if (!setupSD_Card())
   {
    Serial.println("\n\tEntering infinite loop");
    while (1);
   } 
  setupTFT(SCREEN_ORIENTATION);
  splachScreen((char *)APP_VERSION);
  delay(3000);
  drawFrame();

  Serial.println("\n\tModule initialized - \tStart sniffing DCC messages\n\n");
 }

void loop()
 {
  uint8_t uiAction = getScreenAction();
  
  if (uiAction != 0xFF) Serial.println("Button : " + String(uiAction));
 }