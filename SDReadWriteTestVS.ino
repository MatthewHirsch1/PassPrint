/* SD Card Read/Write Test Sketch
 * PassPrint - Senior Design Project
 * by Matthew Hirsch
 * 2/18/19 
 * 
 * Pins for SD card reader on Arduino Mega 2560:
 * CS - pin 47
 * SCK - pin 52
 * MOSI - pin 51
 * MISO - pin 50
 */
#include <SPI.h>
#include <SD.h>

File myFile; // Define myFile as a File type object
String testFile = "test.txt"; // Define test file name as a global String

int numCreds = 0; // Number of credentials in the file

void setup() {
  Serial.begin(9600); // Open serial communications and wait for port to open:
  while (!Serial) { ; } // Wait for serial port to connect. Needed for native USB port only

  //Serial.print("Initializing SD card..."); // Provide feedback on initialization
  if (!SD.begin(47)) { // If CS pin is not connected to pin 47
    //Serial.println("Initialization failed!");
    while (1);
  }
  //Serial.println("Initialization done.");

  //Serial.println("Removing " + testFile);
  SD.remove(testFile); // Remove file each time so file contents are controlled
  
  addOneCredential(testFile, "gmail.com:bob123@gmail.com:123:");
  addOneCredential(testFile, "yahoo.com:bob123@yahoo.com:456:");
  addOneCredential(testFile, "outlook.com:bob123@outlook.com:321:");

  //Serial.println("Reading Contents:--------------------");
  //readContents(testFile);

  addOneCredential(testFile, "aol.com:bob123@aol.com:789:");
  //Serial.println("Reading Contents:--------------------");
  //readContents(testFile);
  //Serial.println("NumCreds: " + String(numCreds));
  //retrieveOneCredential(testFile,"yahoo.com");

  removeOneCredential(testFile, "outlook.com","bob123@outlook.com","321");
  //Serial.println("NumCreds: " + String(numCreds));

  //Serial.println("Reading Contents after removal:--------------------");
  //readContents(testFile);
//  Serial.println("NumCreds: " + String(numCreds));
//
//  removeOneCredential(testFile, "gmail.com", "bob123@gmail.com", "123");
//
//  Serial.println("Reading Contents after removal:--------------------");
//  readContents(testFile);
//  Serial.println("NumCreds: " + String(numCreds));

}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('%');
    data.trim();
    if (data == "Add Credential") {
      String newAccount = Serial.readStringUntil(':');
      String newUname = Serial.readStringUntil(':');
      String newPwd = Serial.readStringUntil(':');
      addOneCredential(testFile, newAccount + ":" + newUname + ":" + newPwd + ":");
    }
    if (data == "Retrieve Credentials") {
      //Serial.flush();
      Serial.println(numCreds);
      readContents(testFile);
    }
    if (data == "Save Credential") {
      String oldAccount = Serial.readStringUntil(':');
      String oldUname = Serial.readStringUntil(':');
      String oldPwd = Serial.readStringUntil(':');
      removeOneCredential(testFile, oldAccount, oldUname, oldPwd);
      String newAccount = Serial.readStringUntil(':');
      String newUname = Serial.readStringUntil(':');
      String newPwd = Serial.readStringUntil(':');
      addOneCredential(testFile, newAccount + ":" + newUname + ":" + newPwd + ":");
    }
    if (data == "Delete Credential") {
      String oldAccount = Serial.readStringUntil(':');
      String oldUname = Serial.readStringUntil(':');
      String oldPwd = Serial.readStringUntil(':');
      removeOneCredential(testFile, oldAccount, oldUname, oldPwd);
    }
  
  
  
  }
} // End of loop

// Read the whole contents of the file
void readContents(String fileName) {
  myFile = SD.open(fileName); // Open the file for reading
  if (myFile) {    
    // Read bytes from the file until there's nothing left
    //Serial.println("File is open!!!");
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
    // Initialize Strings to hold website name, username, and password
    String site;
    String uname;
    String pwd;
    while (myFile.available() && !site.equals(siteName)) { // Find credential for desired website
      site = myFile.readStringUntil(':');
      site.trim();
    }
    if (myFile.available()) { // Read the username and password for the desired website
      uname = myFile.readStringUntil(':');
      uname.trim();
      pwd = myFile.readStringUntil(':');
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
void addOneCredential(String fileName, String cred) {
  myFile = SD.open(fileName, FILE_WRITE); // Open the file for writing
  if (myFile) {    
    // Add user credentials to SD card database (Encrypt it Sprint 3)
    myFile.println(cred);
    
    myFile.close(); // Close the file
    numCreds++;
    delay(100);
    //Serial.println("num after add: " + String(numCreds));
  } else { // If the file didn't open, print an error
    Serial.println("Error opening " + fileName);
  }
}

// Removes one credential (website, username, and password) from the database
void removeOneCredential(String fileName, String siteName, String username, String password) {
  int newNumCreds = numCreds - 1;
  //Serial.println("newNumCreds: " + String(newNumCreds));
  String arrCreds[newNumCreds];
  if (newNumCreds != 0) {
    myFile = SD.open(fileName); // Open the file for reading
    // need numcreds check
    //Serial.println("numCreds: " + String(numCreds));
    //int newNumCreds = numCreds - 1;
    //Serial.println("newNumCreds: " + String(newNumCreds));
    //String arrCreds[newNumCreds];
    if (myFile) {
      // Initialize Strings to hold website name, username, and password
      String site;
      String uname;
      String pwd;
      //String arrCreds[] = [];
      int arrSize = newNumCreds;
      //Serial.println("About to go through file");
      while (myFile.available()) { // Find credential for desired website
        site = myFile.readStringUntil(':');
        site.trim();
        uname = myFile.readStringUntil(':');
        uname.trim();
        pwd = myFile.readStringUntil(':');
        pwd.trim();
        if (site == siteName && uname == username && pwd == password) { // Skip credential to be removed
          //Serial.println("Found the matching cred");
          //if (Serial.read() == "\n") {Serial.println("Newline found");}
          if (myFile.peek() != -1) { //Serial.println("There is more data to read"); 
            continue;}
          else {
            //Serial.println("Reached end of file"); 
            break;}
        } else { // Record valid credentials
          String cred = site + ":" + uname + ":" + pwd + ":";
          //Serial.println("Valid cred: " + cred);
          //Serial.println("Insert into index: " + String(newNumCreds - arrSize));
          arrCreds[newNumCreds - arrSize] = cred;
          arrSize--;
          if (arrSize == 0) {break;}
        }
      }
    }
    myFile.close(); 
    //Serial.println("File closed");
  } // end of if newNumCreds != 0  
  // Remove old file and rewrite all valid credentials into new file
  //myFile.close(); 
  //Serial.println("File closed");
  SD.remove(fileName);
  //Serial.println("File removed");
  myFile = SD.open(fileName, FILE_WRITE);
  //Serial.println("File open for writing");
  myFile.close();
  numCreds = 0;
  for (int i = 0; i < newNumCreds; i++) {
    addOneCredential(fileName, arrCreds[i]);
  }
}
