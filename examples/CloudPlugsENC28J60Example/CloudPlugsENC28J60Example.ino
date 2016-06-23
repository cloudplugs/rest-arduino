// UIPEthernet for ENC28J60: https://github.com/ntruchsess/arduino_uip
#include <UIPEthernet.h>
#include <CloudPlugs.h>

#define AUTH_PLUGID "dev-xxxxxxxxxxxxxxxxxx" /**< The device plug ID */
#define AUTH_PASS   "your-password" /**< The master password */
#define AUTH_MASTER true

// ENC28J60 using fixed IP Address. DHCP is too big for the sketch.
byte ip[] = { 192, 168, 1, 125 };

// Enter a MAC address for your controller below.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient eClient;
CloudPlugs client(eClient);
String response;

void setup() {
	// initialize serial communications at 9600 bps
	Serial.begin(9600);
  
	Serial.print("Connecting to network ... ");
	while (Ethernet.begin(mac, IPAddress(ip)) != 1) {
		Serial.println("Error setting up network, trying again ...");
		delay(5000);
	}

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
