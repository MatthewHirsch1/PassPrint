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
 
/* Pins for SD card reader on Arduino Mega
 * CS - pin 47
 * SCK - pin 52
 * MOSI - pin 51
 * MISO - pin 50
 */
#include <SPI.h>
#include <SD.h>

File myFile; // Define myFile as a File type object
String testFile = "test.txt"; // Define test file name as a global String

void setup() {
  Serial.begin(9600); // Open serial communications and wait for port to open:
  while (!Serial) { ; } // Wait for serial port to connect. Needed for native USB port only

  Serial.print("Initializing SD card..."); // Provide feedback on initialization
  if (!SD.begin(47)) { // If CS pin is not connected to pin 47
    Serial.println("Initialization failed!");
    while (1);
  }
  Serial.println("Initialization done.");

  Serial.println("Removing " + testFile);
  SD.remove(testFile); // Remove file each time so file contents are controlled

  addOneCredential(testFile, "gmail.com", "bob123@gmail.com", "123");
  addOneCredential(testFile, "yahoo.com", "bob123@yahoo.com", "456");  
  addOneCredential(testFile, "outlook.com", "bob123@outlook.com", "321");

  readContents(testFile);

  addOneCredential(testFile, "aol.com", "bob123@aol.com", "789");
  
  readContents(testFile);

  //Serial.println("\nTesting one credential retrieval from " + testFile + "...");
  retrieveOneCredential(testFile,"yahoo.com");
}

void loop() {} // Nothing done in loop

// Read the whole contents of the file
void readContents(String fileName) {
  myFile = SD.open(fileName); // Open the file for reading
  if (myFile) {
    Serial.println("\nContents of " + fileName + ":");
    
    // Read bytes from the file until there's nothing left
    while (myFile.available()) { Serial.write(myFile.read()); }
    
    myFile.close(); // Close the file
  } else { // If the file didn't open, print an error
    Serial.println("Error opening " + fileName);
  }
}  

// Retrieve one set of credentials from the SD card database.
// 'fileName' is the file name on the SD card where the credentials are stored.
// 'siteName' is the website name listed with the credentials in the file.
void retrieveOneCredential(String fileName, String siteName) {
  myFile = SD.open(fileName); // Open the file for reading
  if (myFile) {
    Serial.println("\nRetrieving the credential for " + siteName + " from " + fileName + ":");
    // Initialize Strings to hold website name, username, and password
    String site;
    String uname;
    String pwd;
    while (myFile.available() && !site.equals(siteName)) { // Find credential for desired website
      site = myFile.readStringUntil(',');
      site.trim();
    }
    if (myFile.available()) { // Read the username and password for the desired website
      uname = myFile.readStringUntil(',');
      uname.trim();
      pwd = myFile.readStringUntil(',');
      pwd.trim();
    }
    Serial.println(site);
    Serial.println(uname);
    Serial.println(pwd);
  
    myFile.close(); // Close the file
  } else { // If the file didn't open, print an error
    Serial.println("Error opening " + fileName);
  }
}
 
// Adds one credential (website, username, and password) to the database
void addOneCredential(String fileName, String siteName, String uname, String pwd) {
  myFile = SD.open(testFile, FILE_WRITE); // Open the file for writing
  if (myFile) {
    Serial.print("Writing to " + testFile + "...");
    
    // Add user credentials to SD card database (Encrypt it Sprint 3)
    myFile.println(siteName + ", " + uname + ", " + pwd + ",");
    
    myFile.close(); // Close the file
    Serial.println("done.");
  } else { // If the file didn't open, print an error
    Serial.println("Error opening " + testFile);
  }
}
