/**
 * Header for the SD card manager.
 * Used to call all the dependencies
 * 
 * Version: 1.0.0 Date: 2020-06-04
 * 
 * Author: Jérôme Rioublanc
 * 
 */

#ifndef __SD_MANANGER_H__
#define __SD_MANANGER_H__ 1

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define SD_SS_PIN           10


bool      setupSD_Card(void);
char     *createNextFile(void);
void      appendToFile(char * pcMessage);
void      closeFile(void);
bool      isFileOpen(void);

#endif