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
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("\tDCC Snipper\n\tInitialization process started.");
  if (!setupSD_Card())
  {
   Serial.println("\n\tEntering infinite loop");
   while (1);
  } 
  Serial.println("Module initialized");
 }

void loop()
 {

 }