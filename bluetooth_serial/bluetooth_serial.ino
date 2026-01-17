#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int LED_PIN = 2;
bool ledState = false;

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(115200);
    SerialBT.begin("ESP32_BT");

    Serial.println("ESP32 ready via USB");
    Serial.println("Bluetooth name: ESP32_BT");
}

void loop()
{
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
            ledState = true;
            digitalWrite(LED_PIN, HIGH);
            SerialBT.println("LED is ON");
        }
        else if (cmd == "off")
        {
            ledState = false;
            digitalWrite(LED_PIN, LOW);
            SerialBT.println("LED is OFF");
        }
        else if (cmd == "status")
        {
            SerialBT.print("LED state: ");
            SerialBT.println(ledState ? "ON" : "OFF");
        }
        else
        {
            SerialBT.println("Unknown command");
        }
    }
}
