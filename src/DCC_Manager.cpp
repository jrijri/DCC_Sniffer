/**
 *  DCC manager
 * 
 * Version: 1.0.0 Date 2020-06-07
 * 
 * Author: Jérôme Rioublanc
 *
 */

#include "DCC_Manager.h"

/**
 * @brief Initialize the DCC settings
 * 
 * @return true all the time, no test done in the process
 */
bool setupDCC(void)
 {
  bool bStatus = true;

  Serial.print("\nInitializing the DCC decoder...\n");
  pinMode(DCC_INPUT_PIN, INPUT);
  Serial.println("\tDone.");
  return(bStatus);
 }