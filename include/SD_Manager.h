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

#define FILE_NAME_LEN       14                                                 // Max file name length, 12.3 MSDOS Compatible

class SD_Record
 {
  public:
    SD_Record();
    bool  openNextFile(void);
    bool  write(char *pcMessage);
    void  closeFile(void);
    void  startRecording(void);
    void  stopRecording(void);
    bool  isFileOpen(void);
    bool  isRecording(void);
    char *getFileName(void);
    File  mFile;

  private:
    bool  mbOpen,
          mbRecording;
    char  mcFileName[FILE_NAME_LEN];
 };

bool      setupSD_Card(void);

#endif