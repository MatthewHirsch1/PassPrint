/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

File myFile;


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(47)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");


  Serial.println("Removing test.txt");
  SD.remove("test.txt");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("gmail.com, bob123@gmail.com, 123456");
    myFile.println("yahoo.com, bob123@yahoo.com, 123456"); 
    // Add user credentials to SD card database (Encrypt it Sprint 3)
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  readContents("test.txt");

  // if the file opened okay, write to it:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("yahoo.com, bob123@yahoo.com, 123456"); 
    // Add user credentials to SD card database (Encrypt it Sprint 3)
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  
  readContents("test.txt");


  Serial.println("testing one cred retrieval...");
  retrieveOneCred("test.txt","yahoo.com");
}

void loop() {
  // nothing happens after setup  
}

void readContents(String file) {
  // re-open the file for reading: ADDED MORE CREDENTIALS
  myFile = SD.open(file);
  if (myFile) {
    Serial.println(file + ":");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening " + file);
  }
}  

// Retrieve one set of credentials from the SD card database
// file is the file on the SD card where the credentials are stored
// siteName is the website name listed with the credentials in the file
void retrieveOneCred(String file, String siteName) {
  myFile = SD.open(file);
  if (myFile) {
    //Serial.println(file + ":");

    // read from the file until there's nothing else in it:
    while (myFile.available() && myFile.peek() != 10) {// peek returns the next character without incrementing the read index
      int cred = myFile.read();
      //Serial.write(myFile.read());
      Serial.write(cred);
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening " + file);
  }




  
}
