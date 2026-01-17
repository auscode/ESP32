const int LED_PIN = 2;

// LED state
bool ledState = false;
bool blinkingEnabled = true;

// timers
unsigned long lastBlink = 0;
unsigned long lastUptime = 0;

const unsigned long blinkInterval = 500;   // ms
const unsigned long uptimeInterval = 1000; // ms

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);

    Serial.println("ESP32 multitasking demo");
    Serial.println("Commands: on, off, blink, noblink, status");
}

void loop()
{
    unsigned long now = millis();

    // ---- LED blinking task ----
    if (blinkingEnabled && now - lastBlink >= blinkInterval)
    {
        lastBlink = now;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }

    // ---- uptime task ----
    if (now - lastUptime >= uptimeInterval)
    {
        lastUptime = now;
        Serial.print("Uptime (ms): ");
        Serial.println(now);
    }

    // ---- serial commands ----
    if (Serial.available())
    {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        cmd.toLowerCase();

        if (cmd.length() == 0)
            return;

        if (cmd == "on")
        {
            blinkingEnabled = false;
            ledState = true;
            digitalWrite(LED_PIN, HIGH);
            Serial.println("LED forced ON");
        }
        else if (cmd == "off")
        {
            blinkingEnabled = false;
            ledState = false;
            digitalWrite(LED_PIN, LOW);
            Serial.println("LED forced OFF");
        }
        else if (cmd == "blink")
        {
            blinkingEnabled = true;
            Serial.println("Blinking enabled");
        }
        else if (cmd == "noblink")
        {
            blinkingEnabled = false;
            Serial.println("Blinking disabled");
        }
        else if (cmd == "status")
        {
            Serial.print("LED: ");
            Serial.print(ledState ? "ON" : "OFF");
            Serial.print(" | Mode: ");
            Serial.println(blinkingEnabled ? "BLINK" : "MANUAL");
        }
        else
        {
            Serial.println("Unknown command");
        }
    }
}
