#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "Nothing";
const char *password = "";

const int LED_PIN = 2;

bool ledState = false;
bool blinking = false;

unsigned long lastBlink = 0;
const unsigned long blinkInterval = 500;

WebServer server(80);

// ---------- Web handlers ----------
void handleRoot()
{
    String page =
        "<h1>ESP32 Control</h1>"
        "<a href='/on'><button>ON</button></a><br><br>"
        "<a href='/off'><button>OFF</button></a><br><br>"
        "<a href='/blink'><button>BLINK</button></a><br><br>"
        "<a href='/noblink'><button>NO BLINK</button></a><br><br>"
        "<a href='/status'><button>STATUS</button></a>";
    server.send(200, "text/html", page);
}

void handleOn()
{
    blinking = false;
    ledState = true;
    digitalWrite(LED_PIN, HIGH);
    server.send(200, "text/plain", "LED ON");
}

void handleOff()
{
    blinking = false;
    ledState = false;
    digitalWrite(LED_PIN, LOW);
    server.send(200, "text/plain", "LED OFF");
}

void handleBlink()
{
    blinking = true;
    server.send(200, "text/plain", "BLINKING");
}

void handleNoBlink()
{
    blinking = false;
    server.send(200, "text/plain", "BLINK STOPPED");
}

void handleStatus()
{
    String s = "LED=";
    s += ledState ? "ON" : "OFF";
    s += " MODE=";
    s += blinking ? "BLINK" : "MANUAL";
    server.send(200, "text/plain", s);
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);

    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) // wait max 10s
    {
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\nWiFi connected!");
        Serial.print("Open browser at: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("\nWiFi failed, continuing anyway.");
    }

    server.on("/", handleRoot);
    server.on("/on", handleOn);
    server.on("/off", handleOff);
    server.on("/blink", handleBlink);
    server.on("/noblink", handleNoBlink);
    server.on("/status", handleStatus);

    server.begin();
}

void loop()
{
    server.handleClient();

    unsigned long now = millis();
    if (blinking && now - lastBlink >= blinkInterval)
    {
        lastBlink = now;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }
}
