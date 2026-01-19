#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int LED_PIN = 2;
const char *btPin = "1234";

bool ledState = false;
bool blinkingEnabled = false;

unsigned long lastBlink = 0;
const unsigned long blinkInterval = 500;

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(115200);

    // 🔐 Legacy PIN mode (stable)
    SerialBT.setPin(btPin, 4);
    SerialBT.begin("ESP32_BT");

    Serial.println("ESP32 ready");
    Serial.println("Bluetooth name: ESP32_BT");
    Serial.println("PIN: 1234");

    SerialBT.println("Commands: on, off, blink, noblink, status");
}

void loop()
{
    unsigned long now = millis();

    if (blinkingEnabled && now - lastBlink >= blinkInterval)
    {
        lastBlink = now;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }

    if (SerialBT.available())
    {
        String cmd = SerialBT.readStringUntil('\n');
        cmd.trim();
        cmd.toLowerCase();

        if (cmd == "on")
        {
            blinkingEnabled = false;
            ledState = true;
            digitalWrite(LED_PIN, HIGH);
            SerialBT.println("LED ON");
        }
        else if (cmd == "off")
        {
            blinkingEnabled = false;
            ledState = false;
            digitalWrite(LED_PIN, LOW);
            SerialBT.println("LED OFF");
        }
        else if (cmd == "blink")
        {
            blinkingEnabled = true;
            SerialBT.println("Blink ON");
        }
        else if (cmd == "noblink")
        {
            blinkingEnabled = false;
            SerialBT.println("Blink OFF");
        }
        else if (cmd == "status")
        {
            SerialBT.print("LED=");
            SerialBT.print(ledState ? "ON" : "OFF");
            SerialBT.print(", MODE=");
            SerialBT.println(blinkingEnabled ? "BLINK" : "MANUAL");
        }
        else
        {
            SerialBT.println("Unknown");
        }
    }
}
