/*
 * Reads EEPROM and stores data in a txt file on an sd card.
 * Notepad file name has to be input manually.
 * Example code used to initialise sd card and for basic read/write files.
 */

#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
 
File root;
File myFile;

/*************************************************************************************************************************************************************/

void setup()
{
  Serial.begin(9600);                                                       // Open serial communications and wait for port to open:
  while (!Serial)
  {
    ;                                                                       // wait for serial port to connect. Needed for native USB port only
  }
 
  pinMode(10, OUTPUT);                                                      // set the SS pin as an output (necessary!)
  digitalWrite(10, HIGH);                                                   // but turn off the W5100 chip

  Serial.print("Initializing SD card...");                                  // prints connection status to SD card.
 
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  root = SD.open("/");                                                      // sets root to the root of the SD card.

  printDirectory(root, 0);                                                  // prints all files/folders in root.
 
  Serial.println("done!");

  // ********************************************** // End of list setup

  myFile = SD.open("DEX131.txt", FILE_WRITE);                                // open file, must be closed before opening another. Manually change txt file name here
  if (myFile)                                                               // if the file is opened okay
  {
    Serial.println("Writing to DEX131.txt...");                              // manually change txt file name here
    for (int i = 0; i < 44+11+11; i++)                                      // prints values from EEPROM to txt file.
    {
      myFile.print(EEPROM.read(i));
      myFile.print(" ");
    }
    myFile.close();                                                         // close the file
    Serial.println("Done");
  }
  else
  {
    Serial.println("error in opening WRITE.txt");
  }
}

/*************************************************************************************************************************************************************/

void loop()
{
  // nothing happens after setup finishes.
}

/*************************************************************************************************************************************************************/
 
void printDirectory(File dir, int numTabs)                                  // passes directory and number of tabs
{
  while (true)                                                              // do
  {
    File entry =  dir.openNextFile();                                       // opens next file in directory and stores name into 'entry'
    if (! entry)                                                            // if there's no more files end process
    {
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++)                                   // for every number of tabs, print a tab
    {
      Serial.print('\t');
    }
    Serial.print(entry.name());                                             // print 'entry' name
    if (entry.isDirectory())                                                // if 'entry' is a directory print /, new line, printDirectory and increment numTabs
    {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    }
    else
    {
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);                                    // print file sizes, directories do not have sizes.
    }
    entry.close();                                                          // close 'entry'
  }
}
