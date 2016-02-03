#include <SPI.h>
#include <GSM.h>
#include <CloudPlugs.h>

#define AUTH_PLUGID "dev-xxxxxxxxxxxxxxxxxx" /**< The device plug ID */
#define AUTH_PASS   "your-password" /**< The master password */
#define AUTH_MASTER true

// PIN Number for the SIM card
#define PINNUMBER      "0000"

// APN data
#define GPRS_APN       "GPRS_APN" // replace your GPRS APN
#define GPRS_LOGIN     "login"    // replace with your GPRS login
#define GPRS_PASSWORD  "password" // replace with your GPRS password

GSMClient gClient;
GPRS gprs;
GSM gsmAccess;         // include a 'true' parameter for debug enabled
CloudPlugs client(gClient);
String response;

void setup() {
	// initialize serial communications at 9600 bps
	Serial.begin(9600);

	// connection state
	boolean notConnected = true;

	// Start GSM shield
	// If your SIM has PIN, pass it as a parameter of begin() in quotes
	while (notConnected) {
		if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &
		    (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
			notConnected = false;
		} else {
			Serial.println("Not connected");
			delay(1000);
		}
	}

	Serial.println("Connected to GPRS network");

	// Get IP
	IPAddress LocalIP = gprs.getIPAddress();
	Serial.println("Server IP address: ");
	Serial.println(LocalIP);

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
