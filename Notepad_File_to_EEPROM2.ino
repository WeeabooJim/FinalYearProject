/*
 * Reads txt file data on an sd card, converts it to an integer array and writes it to EEPROM
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
  Serial.begin(9600);                                                                 // Open serial communications and wait for port to open:
  while (!Serial)
  {
    ;                                                                                 // wait for serial port to connect. Needed for native USB port only
  }
 
  pinMode(10, OUTPUT);                                                                // set the SS pin as an output (necessary!)
  digitalWrite(10, HIGH);                                                             // but turn off the W5100 chip
  
  Serial.print("Initializing SD card...");                                            // prints connection status to SD card.
 
  if (!SD.begin(4))
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
 
  root = SD.open("/");                                                                // sets root to the root of the SD card.
 
  printDirectory(root, 0);                                                            // prints all files/folders in root.
 
  Serial.println("done!");

  // ********************************************** // End of list setup

  myFile = SD.open("DEX006.txt");                                                     // open file to read. Manually change txxt file name here
  if (myFile)                                                                         // if the file is opened okay
  {
    char in_data[myFile.size()-1];                                                    // make char variable the size of the file - 1 to store the data in (stores data as 1 character)
    
    Serial.println("DEX006.txt");                                                     // print name of file to read
    // read from file till nothing left
    while (myFile.available())                                                        // while the file is available
    {
      for(int i=0 ; i<(sizeof(in_data)/sizeof(char)) ; i++)                           // from 0 to the final character
      {
        in_data[i] = myFile.read();                                                   // in_data variable stores the file data
      }
      for(int i=0 ; i<(sizeof(in_data)/sizeof(char)) ; i++)                           // then after the data is transfered to the variable
      {
        Serial.print(in_data[i]);                                                     // print the data on serial monitor
      }
      myFile.close();                                                                 // close the file
     }
    Serial.print('\n');                                                               // print new line
    Serial.println(sizeof(in_data)/sizeof(char));                                     // print number of characters stores
    
    unsigned char num_data[66];                                                       // new variable (66, exact number of individual variables in data) to store seperated data
    char temp[4];                                                                     // new temp char to store max size of single number (3 digits + term chara)
    
    int j = 0;                                                                        // int to increment temp digit
    int k = 0;                                                                        // int to increment num_data digit
    for ( int i = 0 ; i < (sizeof(in_data)/sizeof(char)) ; i++ )                      // for 0 till the final character
    {
      if ( (in_data[i] != ' ') && (i < (sizeof(in_data)-1)) )                         // if in_data character is not a space and we're not at the end yet
      {
        temp[j++] = in_data[i];                                                       // temp variable stores in_data then increments temp digit
      }
      else if ( (in_data[i] == ' ') || ( i == (sizeof(in_data)-1)) )                  // if in_data is a space or we've reached the final character
      {
        temp[j] = '\0';                                                               // add a terminator to temp
        num_data[k] = atoi(temp);                                                     // num_data is set to atoi of temp (character to integer converter)
        j = 0;                                                                        // reset temp digit
        Serial.print(num_data[k]);                                                    // print num_data digit
        k++;                                                                          // increment num_data digit
        Serial.print(' ');                                                            // print a space
      } 
    }

    Serial.print('\n');                                                               // print new line

    for (int i = 0; i < 255; i++)                                                     // from 0 to 255
    {
      EEPROM.write(i, num_data[i]);                                                   // write num_data to EEPROM memory
    }

    for (int i=0; i<44+11+11; i++)                                                    // from 0 to 66 (full Pokemon data)
    {
      Serial.print(EEPROM.read(i));                                                   // print EEPROM memory to serial monitor
      Serial.print(" ");                                                              // print space
    }
  }
  else                                                                                // if it failed
  {
    Serial.println("error in opening DEX151.txt");                                    // print error in opening file
  }
}

/*************************************************************************************************************************************************************/

void loop()
{
  // nothing happens after setup
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
