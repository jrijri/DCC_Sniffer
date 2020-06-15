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

/**
 * @brief Default constructor
 */
DCC_Packet::DCC_Packet()
 {
  muiCommand = 0x00;
  for (uint8_t uiInd = 0; uiInd < MAX_ARGS; uiInd++)
    muiArg[uiInd] = 0x00;
 }

/**
 * @brief .
 */
void DCC_Packet::pulling(void)
 {
  
 }


/**
 * @brief Return the command Mnemo
 * 
 * @return the value as uint8_t
 */
uint8_t DCC_Packet::getCommandMnemo(void) { return(muiCommand); }

/**
 * @brief return the nth argument of the message
 * 
 * @param uiIndex as uint8_t is the index in the argument list
 * @return the nth argument as uint8_t or 0xff is the index is invalid
 */
uint8_t DCC_Packet::getArgument(uint8_t uiIndex)
 {
  if ((uiIndex >= 0) && (uiIndex < MAX_ARGS)) return(muiArg[uiIndex]);
  else                                        return(0xFF);
 }
