#include <M5StickCPlus.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

#define CARDKB_ADDR 0x5F  // I2C address for CardKB

const char* ssid = "Brainwave Generator";
const char* password = NULL;  // Open network (no password)

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

String values[3] = {"", "", ""}; // To hold the values of frequency1, frequency2, and pulse
int fieldIndex = 0; 
bool isPlaying = false;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    // Handle WebSocket event
}

void setup() {
    M5.begin();
    Serial.begin(115200);
    Wire.begin(32, 33);
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(1, 10);
    M5.Lcd.println("Freq 1: ");
    M5.Lcd.println("Freq 2: ");
    M5.Lcd.println("Pulse: ");

    WiFi.softAP(ssid, password);
    server.on("/", HTTP_GET, handleRoot);
    server.begin();
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    server.handleClient();
    webSocket.loop(); 

    static String lastValues[3] = {"", "", ""}; // Keep track of the last values to detect changes

    if (Wire.requestFrom(CARDKB_ADDR, 1)) {
        bool needUpdate = false;  // Flag to check if we need to update display
        char c = Wire.read();

        if (c == '\b') {
            if (values[fieldIndex].length() > 0) {
                values[fieldIndex].remove(values[fieldIndex].length() - 1);
                needUpdate = true;
            }
        } else if (c == '\t') {
            fieldIndex = (fieldIndex + 1) % 3;
            needUpdate = true;
        } else if (c != 0) {  
            values[fieldIndex] += c;  
            needUpdate = true;
        }

        // Update the LCD only if there was a change in the value
        if (needUpdate && (lastValues[0] != values[0] || lastValues[1] != values[1] || lastValues[2] != values[2])) {
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.setCursor(1, 10);
            M5.Lcd.println("Freq 1: " + values[0]);
            M5.Lcd.println("Freq 2: " + values[1]);
            M5.Lcd.println("Pulse: " + values[2]);

            // Update last values for next comparison
            lastValues[0] = values[0];
            lastValues[1] = values[1];
            lastValues[2] = values[2];
        }
    }

    if (M5.BtnA.wasPressed()) {
        isPlaying = !isPlaying;
        String message = String(isPlaying ? "play" : "stop") + "," + values[0] + "," + values[1] + "," + values[2];
        Serial.println("Sending message: " + message);
        webSocket.broadcastTXT(message);
    }

    M5.update();
}

void handleRoot() {
    String html = 
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<title>Brainwave Generator</title>"
    "<script>"
    "var audioCtx = new (window.AudioContext || window.webkitAudioContext)();"
    "var oscillator1, oscillator2, gainNode, lfo;"

    "function startSound(frequency1, frequency2, pulseRate) {"
    "  stopSound();"
    "  oscillator1 = audioCtx.createOscillator();"
    "  oscillator2 = audioCtx.createOscillator();"
    "  gainNode = audioCtx.createGain();"
    "  lfo = audioCtx.createOscillator();"
    "  oscillator1.type = 'sine';"
    "  oscillator2.type = 'sine';"
    "  oscillator1.frequency.setValueAtTime(frequency1, audioCtx.currentTime);"
    "  oscillator2.frequency.setValueAtTime(frequency2, audioCtx.currentTime);"
    "  lfo.frequency.setValueAtTime(pulseRate, audioCtx.currentTime);"
    "  lfo.connect(gainNode.gain);"
    "  oscillator1.connect(gainNode);"
    "  oscillator2.connect(gainNode);"
    "  gainNode.connect(audioCtx.destination);"
    "  oscillator1.start();"
    "  oscillator2.start();"
    "  lfo.start();"
    "}"

    "function stopSound() {"
    "  if (oscillator1) {"
    "    oscillator1.stop();"
    "    oscillator1.disconnect();"
    "  }"
    "  if (oscillator2) {"
    "    oscillator2.stop();"
    "    oscillator2.disconnect();"
    "  }"
    "  if (lfo) {"
    "    lfo.stop();"
    "    lfo.disconnect();"
    "  }"
    "  if (gainNode) {"
    "    gainNode.disconnect();"
    "  }"
    "}"

    "function togglePlay() {"
    "  if(isPlaying) {"
    "    stopSound();"
    "    isPlaying = false;"
    "  } else {"
    "    startSound(parseFloat(values[0]), parseFloat(values[1]), parseFloat(values[2]));"
    "    isPlaying = true;"
    "  }"
    "}"

    "var isPlaying = false;"
    "var ws = new WebSocket('ws://' + window.location.hostname + ':81/');"
    "ws.onmessage = function(event) {"
    "  var parts = event.data.split(',');"
    "  if(parts[0] === 'play') {"
    "    startSound(parseFloat(parts[1]), parseFloat(parts[2]), parseFloat(parts[3]));"
    "  } else if(parts[0] === 'stop') {"
    "    stopSound();"
    "  }"
    "};"
    "</script>"
    "</head>"
    "<body>"
    "<h1>Brainwave Generator</h1>"
    "<p>Adjust the frequencies and pulse on the device.</p>"
    "<button onclick='togglePlay()'>Play</button>"
    "</body>"
    "</html>";

    server.send(200, "text/html", html);
}
