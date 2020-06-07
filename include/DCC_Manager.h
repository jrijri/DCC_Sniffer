/**
 * Header DCC manager.
 * Used to call all the dependencies
 * 
 * Version: 1.0.0 Date: 2020-06-07
 * 
 * Author: Jérôme Rioublanc
 * 
 */

#ifndef __DCC_MANANGER_H__
#define __DCC_MANANGER_H__ 1

#include "NmraDcc.h"

#define DCC_INPUT_PIN               2                                          // Arduino pin for the DCC signal input


bool setupDCC(void);

#endif