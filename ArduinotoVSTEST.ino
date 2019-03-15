// Upload this sketch first, then run Visual Studio Project EXE
int LED = 13;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('%');
    data.trim();
    Serial.flush();
    //Serial.println(data);
    if (data == "a") { 
      digitalWrite(LED, HIGH);
      String incData = Serial.readString();
      Serial.flush();
      String outgoingData = "This is BT ON. You sent me: " + incData + "\n";
      Serial.print(outgoingData);
      Serial.flush();
    }
    if (data == "b") { 
      digitalWrite(LED, LOW);
      String incData = Serial.readString();
      Serial.flush();
      String outgoingData = "This is BT OFF. You sent me: " + incData;
      Serial.println(outgoingData);
      Serial.flush();
    }
    if (data == "Adding Credentials") {
      String incData = Serial.readString();
      Serial.flush();
      String outgoingData = "I read data from another form: " + incData;
      Serial.println(outgoingData);
      Serial.flush();
    }
    if (data == "menu") {
      String incData = Serial.readString();
      Serial.flush();
      String outgoingData = "two open serials in vs at once: " + incData;
      Serial.println(outgoingData);
      Serial.flush();
    }
  }
}
