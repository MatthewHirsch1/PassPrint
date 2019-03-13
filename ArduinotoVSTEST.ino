// Upload this sketch first, then run "LEDTestForm" Visual Studio Project EXE
int LED = 13;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    int data = Serial.read();
    if (data == 'a') { 
      digitalWrite(LED, HIGH);
      String incData = Serial.readString();
      String outgoingData = "This is BT. You sent me: " + incData + "\n";
      Serial.print(outgoingData);
    }
    if (data == 'b') { 
      digitalWrite(LED, LOW);
      String incData = Serial.readString();
      String outgoingData = "This is BT. You sent me: " + incData;
      Serial.println(outgoingData);
    }
  }
}
