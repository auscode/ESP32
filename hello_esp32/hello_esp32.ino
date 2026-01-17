bool printed = false;

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (!printed) {
    Serial.println("Hello from ESP32");
    printed = true;   // run only once
  }
}

