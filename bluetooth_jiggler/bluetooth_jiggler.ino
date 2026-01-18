#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int LED_PIN = 2;

// LED state & mode
bool ledState = false;
bool blinkingEnabled = false;

// timers
unsigned long lastBlink = 0;
const unsigned long blinkInterval = 500; // ms

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(115200);
    SerialBT.begin("ESP32_BT");

    Serial.println("ESP32 ready via USB");
    Serial.println("Bluetooth name: ESP32_BT");
    SerialBT.println("Commands: on, off, blink, noblink, status");
}

void loop()
{
    unsigned long now = millis();

    // ---- blinking task ----
    if (blinkingEnabled && now - lastBlink >= blinkInterval)
    {
        lastBlink = now;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }

    // ---- Bluetooth input ----
    if (SerialBT.available())
    {
        String cmd = SerialBT.readStringUntil('\n');
        cmd.trim();
        cmd.toLowerCase();

        if (cmd.length() == 0)
            return;

        if (cmd == "on")
        {
            blinkingEnabled = false;
            ledState = true;
            digitalWrite(LED_PIN, HIGH);
            SerialBT.println("LED forced ON");
        }
        else if (cmd == "off")
        {
            blinkingEnabled = false;
            ledState = false;
            digitalWrite(LED_PIN, LOW);
            SerialBT.println("LED forced OFF");
        }
        else if (cmd == "blink")
        {
            blinkingEnabled = true;
            SerialBT.println("Blinking enabled");
        }
        else if (cmd == "noblink")
        {
            blinkingEnabled = false;
            SerialBT.println("Blinking disabled");
        }
        else if (cmd == "status")
        {
            SerialBT.print("LED: ");
            SerialBT.print(ledState ? "ON" : "OFF");
            SerialBT.print(" | Mode: ");
            SerialBT.println(blinkingEnabled ? "BLINK" : "MANUAL");
        }
        else
        {
            SerialBT.println("Unknown command");
        }
    }
}
