
#include <WiFiClientSecureAxTLS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;


// WiFi Access Config
#define SSID_1 "skyfah (2.4Gz)"
#define PASS_1 "0623639741"

// LINE Access_Token config
#define LINE_TOKEN "lU4OQ3juHAOYry2uBkd70rn4bCi85Zo9B1tAUiE9F4z"

const int sensor = A0;
String LineText;
String string1 = "แรงดันไฟ ";
String string2 = " V";

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.print("\n\nElectricl Engineering Enterprise Group\n");
  WiFiMulti.addAP(SSID_1, PASS_1);
  WiFiMulti.addAP("AndroidAP", "ifmd0883");

  Serial.println("Connecting Wifi...");
  delay(2000);
  CheckWiFi();
}

void loop() {
  CheckWiFi();
  double volt = mapf(analogRead(A0), 0, 1024, 0, 15);
  delay(250000);
  LineText = string1 + volt + string2;
  Serial.print("Line ");
  Serial.println(LineText);
  LINE_Notify(LineText);
}

double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void CheckWiFi() {
  if (WiFiMulti.run() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    delay(1000);
  }
}

void LINE_Notify(String Message) {
  axTLS::WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;
  } else {}

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + Message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + Message;

  client.print(req);
  delay(20);

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  Serial.println("Send Successfully");
}
