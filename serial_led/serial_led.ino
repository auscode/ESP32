const int LED_PIN = 2;
bool ledState = false;

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);

    Serial.println("ESP32 ready.");
    Serial.println("Type: on, off, status, help");
}

void loop()
{
    if (Serial.available())
    {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        cmd.toLowerCase();
        if (cmd.length() == 0)
        {
            return;
        }

        if (cmd == "on")
        {
            ledState = true;
            digitalWrite(LED_PIN, HIGH);
            Serial.println("LED is ON");
        }
        else if (cmd == "off")
        {
            ledState = false;
            digitalWrite(LED_PIN, LOW);
            Serial.println("LED is OFF");
        }
        else if (cmd == "status")
        {
            Serial.print("LED state: ");
            Serial.println(ledState ? "ON" : "OFF");
        }
        else if (cmd == "help")
        {
            Serial.println("Commands: on, off, status, help");
        }
        else
        {
            Serial.println("Unknown command");
        }
    }
}
