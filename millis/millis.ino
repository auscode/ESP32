const int LED_PIN = 2;
bool ledState = false;

// time tracking
unsigned long lastPrint = 0;
const unsigned long interval = 1000; // 1 second

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);

    Serial.println("ESP32 ready.");
    Serial.println("Commands: on, off, status");
}

void loop()
{
    // ---- time-based task ----
    unsigned long now = millis();

    if (now - lastPrint >= interval)
    {
        lastPrint = now;
        Serial.print("Uptime (ms): ");
        Serial.println(now);
    }

    // ---- serial command handling ----
    if (Serial.available())
    {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        cmd.toLowerCase();

        if (cmd.length() == 0)
            return;

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
        else
        {
            Serial.println("Unknown command");
        }
    }
}
