#include <Adafruit_Fingerprint.h>
SoftwareSerial mySerial(11, 10);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;
int State;

void setup()
{
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  State = 1;
}



uint8_t readnumber(void)
{
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void firstfinger()
{
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in which finger number (from 1 to 10) you want to save this finger as...");
  id = readnumber();
  if (id == 0 || id > 10) { // ID #0 or >10 not allowed, try again!
    return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);

  getFingerprintEnroll();

  State = 2;
}

void notherfinger()
{
  Serial.println("Would you like to enroll another fingerprint?");
  Serial.println("Y or N");
  while (true) {
    if (Serial.available() > 0) {
      // read the incoming byte:
      String incomingByte = Serial.readString();
      incomingByte.trim();
      if (incomingByte == "Y") {
        Serial.print("Received: ");
        Serial.println(incomingByte);
        State = 1;
        break;
      }
      else if (incomingByte == "N") {
        Serial.print("Received: ");
        Serial.println(incomingByte);
        State = 3;
        break;
      }
    }
  }
}

void fingerprinttest()
{
  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  int n = 0;
  while (true)
  {
    if (getFingerprintIDez() != -1)
    {
      State = 4;
      break;
    }
    //    Serial.println(getFingerprintIDez());
  }
  //don't ned to run this at full speed.
}

void authenticateaction()
{
  Serial.println("Waiting for valid finger to authenticate action...");
  int n = 0;
  while (true)
  {
    if (getFingerprintIDez() != -1)
    {
      Serial.println("FINGERPRINT APPROVED");
      break;
    }
  }
}

void nextaction()
{
  while (true) {
    int next = readnumber();
    Serial.println(next);
    if (next == 1) {
      State = 3;
      break;
    } else if (next == 2) {
      State = 1;
      break;
    } else if (next == 3) {
      State = 5;
      break;
    }
  }
}

void deleti()
{
  Serial.println("Please type in the ID # (from 1 to 10) you want to delete...");
  int id = readnumber();
  if (id == 0||id>10) {// ID #0 not allowed, try again!
    return;
  }
  finger.getTemplateCount();
  int count = finger.templateCount;
  Serial.println(count);
  if (count == 1) {
    Serial.println("In order to complete this operation, you must first add another fingerprint.");
    Serial.println("Do you wish to proceed? (Y or N)");
    while (true) {
      if (Serial.available() > 0) {
        // read the incoming byte:
        String incomingByte = Serial.readString();
        incomingByte.trim();
        if (incomingByte == "Y") {
          Serial.print("Received: ");
          Serial.println(incomingByte);
          State = 1;
          break;
        }
        else if (incomingByte == "N") {
          Serial.print("Received: ");
          Serial.println(incomingByte);
          State = 4;
          break;
        }
      }
    }
  } else if (count > 1) {
    Serial.print("Deleting ID #");
    Serial.println(id);
    authenticateaction();
    deleteFingerprint(id);
    finger.getTemplateCount();
    Serial.print("Sensor now contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
}

void loop()                     // run over and over again
{
  while (true) {
    switch (State)
    {
      case 1:
        firstfinger();
        break;
      case 2:
        notherfinger();
        break;
      case 3:
        fingerprinttest();
        break;
      case 4:
        Serial.println("Choose next action : ");
        Serial.println("1. Authenticate another fingerprint?");
        Serial.println("2. Add a new fingerprint?");
        Serial.println("3. Delete a fingerprint?");
        nextaction();
        break;
      case 5:
        deleti();
        break;
    }
  }
// want to print out the id #s of the fingerprints that are currently stored in the system and maybe store names associated with each finger
  delay(100000);
}
