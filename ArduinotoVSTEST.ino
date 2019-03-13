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
      Serial.write("Hello from Arduino! LED is on.\n");
    }
    if (data == 'b') { 
      digitalWrite(LED, LOW);
      Serial.write("Hello from Arduino! LED is off.\n");
    }
  }
}
