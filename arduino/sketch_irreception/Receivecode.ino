/*
   IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
   An IR detector/demodulator must be connected to the input RECV_PIN.
   Version 0.1 July, 2009
   Copyright 2009 Ken Shirriff
   http://arcfn.com
*/

#include <IRremote.h>
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>

char server[] = "192.168.43.30";
const uint16_t port = 8080;

char ssid[] = "Bouillette Network v2"; //  your network SSID (name)
char pass[] = "jaibeaucoupdegiga";    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;

char identifiant[] = "touche";
char message[80];
char message_prefix[] = "MSG,";

WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, server, port);

int RECV_PIN = 11;
int LED_CONNECT = 10;
int LED_SHOT = 2;
int BUZZER = 3;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  pinMode(LED_CONNECT, OUTPUT);
  digitalWrite(LED_CONNECT, LOW);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_CONNECT, OUTPUT);
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");

  wifi_connection();
  websocket_connection();

}

void loop() {
  
  if (irrecv.decode(&results) && client.connected()) {
    digitalWrite(LED_CONNECT, HIGH);
    tone(BUZZER, 1000); // Send 1KHz sound signal...
    delay(500);        // ...for 1 sec
    noTone(BUZZER);     // Stop sound...
    digitalWrite(LED_CONNECT, LOW);
    Serial.println("Sending request");
    // send a hello #
    client.beginMessage(TYPE_TEXT);
    sprintf(message, "%s%s", message_prefix, identifiant);
    client.print(message);
    client.endMessage();
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value

  }



}

void wifi_connection() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 1 seconds for connection:
    delay(1000);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void websocket_connection() {
  digitalWrite(LED_CONNECT, HIGH);
  while (!client.connected()) {
    Serial.println("starting WebSocket client");
    client.begin();
  }
  Serial.println("Connection websocket établie");
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(LED_CONNECT, LOW);
    delay(1000);
    digitalWrite(LED_CONNECT, HIGH);
    delay(1000);
    digitalWrite(LED_CONNECT, LOW);

  }
}
