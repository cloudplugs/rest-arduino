#include <LGPRS.h>
#include <LGPRSClient.h>
#include <CloudPlugs.h>

#define AUTH_PLUGID "dev-xxxxxxxxxxxxxxxxxx" /**< The device plug ID */
#define AUTH_PASS   "your-password" /**< The master password */
#define AUTH_MASTER false

LGPRSClient gClient;
CloudPlugs client(gClient);
String response;

void setup() {
	// initialize serial communications at 9600 bps
	Serial.begin(9600);

  // connection state
  boolean notConnected = true;

  // Start GPRS connection
  Serial.println("Attach to GPRS network by auto-detect APN setting");
  while (notConnected) {
    if (!LGPRS.attachGPRS()) {
      Serial.println("Not connected");
      delay(1000);
    } else {
      notConnected = false;
    }
  }

	Serial.println("Connected to GPRS network");

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
