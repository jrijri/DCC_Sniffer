/**
 *  DCC manager
 * 
 * Version: 1.0.0 Date 2020-06-07
 * 
 * Author: Jérôme Rioublanc
 *
 */

#include "DCC_Manager.h"

static char cSignal[5];

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
  cSignal[4] = '\0';
  cSignal[3] = 0x55;  // for test
  cSignal[2] = 0xAA;  // for test
  cSignal[1] = 0x33;  // for test
  return(bStatus);
 }

/**
 * @brief Default constructor
 */
DCC_Message::DCC_Message()
 {
  muiCommand = 0x00;
  for (uint8_t uiInd = 0; uiInd < MAX_ARGS; uiInd++)
    muiArg[uiInd] = 0x00;
 }

/**
 * @brief .
 */
void DCC_Message::pulling(void)
 {
  
 }


/**
 * @brief Return the command Mnemo
 * 
 * @return the value as uint8_t
 */
uint8_t DCC_Message::getCommandMnemo(void) { return(muiCommand); }

/**
 * @brief return the nth argument of the message
 * 
 * @param uiIndex as uint8_t is the index in the argument list
 * @return the nth argument as uint8_t or 0xff is the index is invalid
 */
uint8_t DCC_Message::getArgument(uint8_t uiIndex)
 {
  if ((uiIndex >= 0) && (uiIndex < MAX_ARGS)) return(muiArg[uiIndex]);
  else                                        return(0xFF);
 }
