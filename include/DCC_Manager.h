/**
 * Header DCC manager.
 * Used to call all the dependencies
 * 
 * Version: 1.0.0 Date: 2020-06-07
 * 
 * Author: Jérôme Rioublanc
 * 
 * Based on article: https://locoduino.org/spip.php?article39
 * 
 */

#ifndef __DCC_MANANGER_H__
#define __DCC_MANANGER_H__ 1

#include "NmraDcc.h"

#define   DCC_INPUT_PIN             2                                          // Arduino pin for the DCC signal input

#define   MAX_ARGS                  6                                          // Maximum arguments in the DCC message

class DCC_Packet
 {
  public:
    DCC_Packet();
    void     pulling(void);
    uint8_t  getCommandMnemo(void);
    uint8_t  getArgument(uint8_t uiIndex);

  private:
    uint8_t muiCommand;
    uint8_t muiArg[MAX_ARGS];
 };

bool  setupDCC(void);

#endif