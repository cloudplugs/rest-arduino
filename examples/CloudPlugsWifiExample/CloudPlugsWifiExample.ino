#include <SPI.h>
#include <WiFi.h>
#include <CloudPlugs.h>

#define AUTH_PLUGID "dev-xxxxxxxxxxxxxxxxxx" /**< The device plug ID */
#define AUTH_PASS   "your-password" /**< The master password */
#define AUTH_MASTER true

char ssid[] = "YOUR_SSID";		/* your network SSID (name) */
char pass[] = "YOUR_WIFI_PASSPHRASE";	/* your network password (use for WPA, or use as key for WEP) */

int status = WL_IDLE_STATUS;

WiFiClient wClient;
CloudPlugs client(wClient);
String response;

void printWifiStatus() {
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	// print your WiFi shield's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);

	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI): ");
	Serial.print(rssi);
	Serial.println(" dBm \n");
}

void setup() {
	// initialize serial communications at 9600 bps
	Serial.begin(9600);

	// check for the presence of the shield
	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println("WiFi shield not present");
		// don't continue:
		while (true);
	}

	String fv = WiFi.firmwareVersion();
	Serial.print("WiFi shield firmware version: ");
	Serial.println(fv);
	if ( fv != "1.1.0" ) {
		Serial.println("Please upgrade the firmware");
		while (true);
	}

	/* Attempt to connect to Wifi network */
	while (status != WL_CONNECTED) { 
		Serial.print("Attempting to connect to SSID: ");
		Serial.println(ssid);
		status = WiFi.begin(ssid, pass);
		delay(10000);
	} 

	Serial.println("Connected to wifi");
	printWifiStatus();

	response = "";
	client.setAuth(AUTH_PLUGID, AUTH_PASS, AUTH_MASTER);
}

void debug(const char* tag, bool res, String& response){
	Serial.print(tag);
	if (res) Serial.print(" ERROR: "); else Serial.print(": ");
	Serial.print(client.getLastHttpResult());
	Serial.print(" - ");
	Serial.println(response);
}

void loop() {
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI): ");
	Serial.print(rssi);
	Serial.println(" dBm \n");

	Serial.print("LOOP: ");

	String body = "{\"data\":";
	body.concat(getTemp());
	body.concat("}");

	bool res = client.publishData("temperature", body.c_str(), response);
	debug("PUBLISH", res, response);

	delay(60000);
}

long getTemp() {
	return random(100);
}
