#include <SPI.h>
#include <SD.h>

//Fingerprint section
#include <Adafruit_Fingerprint.h>
SoftwareSerial mySerial(11, 10);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;
File myFile; // Define myFile as a File type object
String testFile = "test.txt"; // Define test file name as a global String

int numCreds = 0; // Number of credentials in the file


void setup() {
  Serial1.begin(9600); // Open Serial1 communications and wait for port to open:
  while (!Serial1) {
    ;  // Wait for Serial1 port to connect. Needed for native USB port only
  }
  finger.begin(57600);

  if (!SD.begin(47)) { // If CS pin is not connected to pin 47
    //Serial1.println("Initialization failed!");
    while (1);
  }
  //Serial1.println("Removing " + testFile);
  SD.remove(testFile); // Remove file each time so file contents are controlled

  addOneCredential(testFile, "gmail.com:bob123@gmail.com:123:");
  addOneCredential(testFile, "yahoo.com:bob123@yahoo.com:456:");
  addOneCredential(testFile, "outlook.com:bob123@outlook.com:321:");

  //Serial1.println("Reading Contents:--------------------");
  //readContents(testFile);

  addOneCredential(testFile, "aol.com:bob123@aol.com:789:");
  removeOneCredential(testFile, "outlook.com", "bob123@outlook.com", "321");

}

void loop() {
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('%');
    data.trim();
    if (data == "Add Credential") {
      String newAccount = Serial1.readStringUntil(':');
      String newUname = Serial1.readStringUntil(':');
      String newPwd = Serial1.readStringUntil(':');
      addOneCredential(testFile, newAccount + ":" + newUname + ":" + newPwd + ":");
    }
    if (data == "Retrieve Credentials") {
      //Serial1.flush();
      Serial1.println(numCreds);
      readContents(testFile);
    }
    if (data == "Save Credential") {
      String oldAccount = Serial1.readStringUntil(':');
      String oldUname = Serial1.readStringUntil(':');
      String oldPwd = Serial1.readStringUntil(':');
      removeOneCredential(testFile, oldAccount, oldUname, oldPwd);
      String newAccount = Serial1.readStringUntil(':');
      String newUname = Serial1.readStringUntil(':');
      String newPwd = Serial1.readStringUntil(':');
      addOneCredential(testFile, newAccount + ":" + newUname + ":" + newPwd + ":");
    }
    if (data == "Delete Credential") {
      String oldAccount = Serial1.readStringUntil(':');
      String oldUname = Serial1.readStringUntil(':');
      String oldPwd = Serial1.readStringUntil(':');
      removeOneCredential(testFile, oldAccount, oldUname, oldPwd);
    }
    if (data == "Choose Fingerprint") {
      //delay(500);
      String ids = Serial1.readStringUntil('%');
      id = ids.toInt();
      //Serial1.println(id);
      //Serial1.println("Message received");

    }
    if (data == "Adding Fingerprint") {
      //Serial1.print("Waiting for valid finger to enroll as #"); Serial1.println(id);
      //      Serial1.println("Image taken");
      getFingerprintEnroll();
    }
    if (data == "Authenticate Fin") {
      authenticateaction();
      //      authenticateaction();
      //getFingerprintIDez();
//      while (true)
//      {
//        //int i = 5;//getFingerprintIDez();
//        if (getFingerprintIDez() != -1)
//        {
//          //Serial1.println("FINGERPRINT APPROVED");
//          Serial1.println("OK");
//          break;
//        }
//      }

    }

  }
} // End of loop

// Read the whole contents of the file
void readContents(String fileName) {
  myFile = SD.open(fileName); // Open the file for reading
  if (myFile) {
    // Read bytes from the file until there's nothing left
    //Serial1.println("File is open!!!");
    while (myFile.available()) {
      Serial1.write(myFile.read());
      //Serial1.write(myFile.read());
    }

    myFile.close(); // Close the file
  } else { // If the file didn't open, print an error
    Serial1.println("Error opening " + fileName);
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
    Serial1.println(site);
    Serial1.println(uname);
    Serial1.println(pwd);

    myFile.close(); // Close the file
  } else { // If the file didn't open, print an error
    Serial1.println("Error opening " + fileName);
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
    //Serial1.println("num after add: " + String(numCreds));
  } else { // If the file didn't open, print an error
    Serial1.println("Error opening " + fileName);
  }
}

// Removes one credential (website, username, and password) from the database
void removeOneCredential(String fileName, String siteName, String username, String password) {
  int newNumCreds = numCreds - 1;
  //Serial1.println("newNumCreds: " + String(newNumCreds));
  String arrCreds[newNumCreds];
  if (newNumCreds != 0) {
    myFile = SD.open(fileName); // Open the file for reading
    // need numcreds check
    //Serial1.println("numCreds: " + String(numCreds));
    //int newNumCreds = numCreds - 1;
    //Serial1.println("newNumCreds: " + String(newNumCreds));
    //String arrCreds[newNumCreds];
    if (myFile) {
      // Initialize Strings to hold website name, username, and password
      String site;
      String uname;
      String pwd;
      //String arrCreds[] = [];
      int arrSize = newNumCreds;
      //Serial1.println("About to go through file");
      while (myFile.available()) { // Find credential for desired website
        site = myFile.readStringUntil(':');
        site.trim();
        uname = myFile.readStringUntil(':');
        uname.trim();
        pwd = myFile.readStringUntil(':');
        pwd.trim();
        if (site == siteName && uname == username && pwd == password) { // Skip credential to be removed
          //Serial1.println("Found the matching cred");
          //if (Serial1.read() == "\n") {Serial1.println("Newline found");}
          if (myFile.peek() != -1) { //Serial1.println("There is more data to read");
            continue;
          }
          else {
            //Serial1.println("Reached end of file");
            break;
          }
        } else { // Record valid credentials
          String cred = site + ":" + uname + ":" + pwd + ":";
          //Serial1.println("Valid cred: " + cred);
          //Serial1.println("Insert into index: " + String(newNumCreds - arrSize));
          arrCreds[newNumCreds - arrSize] = cred;
          arrSize--;
          if (arrSize == 0) {
            break;
          }
        }
      }
    }
    myFile.close();
    //Serial1.println("File closed");
  } // end of if newNumCreds != 0
  // Remove old file and rewrite all valid credentials into new file
  //myFile.close();
  //Serial1.println("File closed");
  SD.remove(fileName);
  //Serial1.println("File removed");
  myFile = SD.open(fileName, FILE_WRITE);
  //Serial1.println("File open for writing");
  myFile.close();
  numCreds = 0;
  for (int i = 0; i < newNumCreds; i++) {
    addOneCredential(fileName, arrCreds[i]);
  }
}
uint8_t getFingerprintEnroll() {

  int p = -1;

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    //Serial1.print("p before switch "); Serial1.println(p);
    switch (p) {
      case FINGERPRINT_OK:
        //Serial1.print("p switch OK "); Serial1.println(p);
        //        Serial1.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //        Serial1.print("p switch NOFINGER "); Serial1.println(p);
        //        Serial1.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        //        Serial1.print("p switch packeterr "); Serial1.println(p);
        Serial1.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        //        Serial1.print("p switch imgfail "); Serial1.println(p);
        //        Serial1.println("Imaging error");
        break;
      default:
        //        Serial1.print("p default "); Serial1.println(p);
        //        Serial1.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      //      Serial1.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //      Serial1.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //      Serial1.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //      Serial1.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //      Serial1.println("Could not find fingerprint features");
      return p;
    default:
      //      Serial1.println("Unknown error");
      return p;
  }

  //  Serial1.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  //  Serial1.print("ID "); Serial1.println(id);
  p = -1;
  //  Serial1.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        //        Serial1.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //        Serial1.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        //        Serial1.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        //        Serial1.println("Imaging error");
        break;
      default:
        //        Serial1.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial1.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial1.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial1.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial1.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial1.println("Could not find fingerprint features");
      return p;
    default:
      //Serial1.println("Unknown error");
      return p;
  }

  // OK converted!
  //Serial1.print("Creating model for #");  Serial1.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //    Serial1.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial1.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    //Serial1.println("Fingerprints did not match");
    return p;
  } else {
    //Serial1.println("Unknown error");
    return p;
  }

  //Serial1.print("ID "); Serial1.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial1.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial1.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    //Serial1.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //Serial1.println("Error writing to flash");
    return p;
  } else {
    //Serial1.println("Unknown error");
    return p;
  }
}


//returns -1 if failed otherwise ID
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  return finger.fingerID;

}

void authenticateaction()
{
  while (true)
  {
    if (getFingerprintIDez() != -1)
    {
      Serial1.print("ok\n");
      break;
    }
  }
}
