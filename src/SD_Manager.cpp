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

/**
 * @brief Default constructor
 */
SD_Record::SD_Record()
 {
  mbOpen      = false;
  mbRecording = false;
 }

/**
 * @brief Look for the next available file number
 *        Files for records are names DCCnnn.DAT
 *        With nnn from  000 to 999
 *        Only the root directory is used
 * 
 * @return the open status failed == false
 */
bool SD_Record::openNextFile(void)
 {
  uint16_t uiNum = 0;

  if(mbOpen) return(false);                                                    // Error case, can't open multiple files
  do
   {
    sprintf(mcFileName, "DCC%03u.DAT", uiNum);
    if (SD.exists(mcFileName)) uiNum++;                                        // Set for the next number's try
    else
     {
      mFile = SD.open(mcFileName, FILE_WRITE);                                 // Create the file for future uses
      if (!mFile) Serial.println("Error opening the file " + String(mcFileName));
      else        mbOpen = true;
     }
   }
  while ((uiNum < 1000) && !mbOpen);
  mbRecording = false;                                                         // Not recording at opening
  return(mbOpen);
 }

/**
  * @brief Write a message
  * 
  * @param pcMessage as char * is the string to be written
  * @return bStatus as bool is the write status failed == false
  */
bool SD_Record::write(char *pcMessage)
 {
  bool   bStatus = false;
  size_t uiSize;

  if(mbOpen && mbRecording)
   {
    uiSize = mFile.println(pcMessage);                                         // println add <CR> and <LF> to the string
//    Serial.println(String(uiSize) + "/" + String(strlen(pcMessage) + 2));      // Used to check the message length
    if (uiSize == strlen(pcMessage) + 2) bStatus = true;                       // Add <CR> and <LF> the the size comparison
    mFile.flush();                                                             // Automatically save the last written data
   }
  return(bStatus);
 }

/**
  * @brief Close an open file
  */
void SD_Record::closeFile(void)
 {
  if (mbOpen) mFile.close();
  mbOpen      = false;
  mbRecording = false;
 }

/**
 * @brief Set the file as recorded/not recorded
 */
void SD_Record::startRecording(void) { mbRecording = true;  }
void SD_Record::stopRecording(void)  { mbRecording = false; }

/**
  * @brief return requested boolean status
  */
bool SD_Record::isFileOpen(void)  { return(mbOpen);      }
bool SD_Record::isRecording(void) { return(mbRecording); }

/**
  * @brief return the file name
  */
char *SD_Record::getFileName(void) { return(mcFileName); }


/**
 * @brief Initialize the SD card interface
 * 
 * @return bStatus as boolean: no error == true
 */
bool setupSD_Card(void)
 {
  bool        bStatus      = false;
  Sd2Card     sdCard;
  SdVolume    sdVolume;
  SdFile      sdRoot;
  uint32_t    uiVolumesize;

  Serial.print("\nInitializing the SD card...");
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
    if (!sdVolume.init(sdCard)) Serial.println("Failed. No partition.");
    else
     {
      Serial.print("Volume type is FAT");
      Serial.println(sdVolume.fatType(), DEC);
      uiVolumesize = sdVolume.blocksPerCluster();                                // clusters are collections of blocks
      uiVolumesize *= sdVolume.clusterCount();                                   // we'll have a lot of clusters
      uiVolumesize *= 512;                                                       // SD card blocks are always 512 bytes
      Serial.print("Volume size (bytes): ");
      Serial.println(uiVolumesize);
      Serial.println("\nFiles found on the card (name, date and size in bytes): ");
      sdRoot.openRoot(sdVolume);
      sdRoot.ls(LS_R | LS_DATE | LS_SIZE);                                       // list all files in the card with date and size
      if(!SD.begin())                                                            // Shall be called prior to any file use
       {
        Serial.println("Unable to initialize the SD communication!");
        while(1);
       }
      Serial.println("Done");
      bStatus = true;
     }
   }
 return (bStatus);
}
