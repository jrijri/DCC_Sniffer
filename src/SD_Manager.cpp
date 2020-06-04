/**
 *  SD Card manager for
 *  HX8357 TFT/SD board
 * 
 * Version: 1.0.0 Date 2020-06-04
 * 
 * Author: Jérôme Rioublanc
 *
 */

#include "SD_Manager.h"

// set up variables using the SD utility library functions:
Sd2Card        sdCard;
SdVolume       sdVolume;
SdFile         sdRoot;
uint32_t       uiVolumesize;


// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8

bool setupSD_Card(void)
 {
  bool    bStatus   = false;

  Serial.print("\nInitializing SD card...");
  pinMode(SS, OUTPUT);                                                         // Set the SS pin for the Card reader
  if (!sdCard.init(SPI_HALF_SPEED, SD_SS_PIN)) Serial.println(" Failed. Check hardware\n");
  else
   {
    Serial.print("\nCard type: ");                                             // print the type of card
    switch(sdCard.type())
     {
      case SD_CARD_TYPE_SD1:
        Serial.println("SD1");
        break;
      case SD_CARD_TYPE_SD2:
        Serial.println("SD2");
        break;
      case SD_CARD_TYPE_SDHC:
        Serial.println("SDHC");
        break;
      default:
        Serial.println("Unknown");
     }
    if (!sdVolume.init(sdCard)) Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    else
     {
      Serial.print("\nVolume type is FAT");
      Serial.println(sdVolume.fatType(), DEC);
      Serial.println();
      
      uiVolumesize = sdVolume.blocksPerCluster();                                // clusters are collections of blocks
      uiVolumesize *= sdVolume.clusterCount();                                   // we'll have a lot of clusters
      uiVolumesize *= 512;                                                       // SD card blocks are always 512 bytes
      Serial.print("Volume size (bytes): ");
      Serial.println(uiVolumesize);
      Serial.print("Volume size (Kbytes): ");
      uiVolumesize /= 1024;
      Serial.println(uiVolumesize);
      Serial.print("Volume size (Mbytes): ");
      uiVolumesize /= 1024;
      Serial.println(uiVolumesize);
      Serial.println("\nFiles found on the card (name, date and size in bytes): ");
      sdRoot.openRoot(sdVolume);
      sdRoot.ls(LS_R | LS_DATE | LS_SIZE);                                       // list all files in the card with date and size
      bStatus = true;
     }
   }
 return (bStatus);
}
