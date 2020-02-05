//#include <arduino_secrets.h>

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

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  wifi_connection();
  websocket_connection();

}

void loop() {

  if (client.connected()) {
    Serial.println("Sending request");
    // send a hello #
    client.beginMessage(TYPE_TEXT);
    sprintf(message,"%s%s",message_prefix, identifiant);
    client.print(message);
    client.endMessage();
    delay(1000);
  } else {
    Serial.println("disconnected");

  }
}


void wifi_connection(){
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

void websocket_connection(){
  while (!client.connected()) {
    Serial.println("starting WebSocket client");
    client.begin();
  }
}
