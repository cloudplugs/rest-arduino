#include <LWiFi.h>
#include <LWiFiClient.h>
#include <CloudPlugs.h>

#define AUTH_MODEL  "mod-xxxxxxxxxxxxxxxxxx" /**< The template model ID */
#define AUTH_HWID   "your-device-hwid" /**< The device hwid */
#define AUTH_PASS   "enroll-password" /**< The device enrollment password */
#define AUTH_MASTER false

char ssid[] = "YOUR_SSID";		/* your network SSID (name) */
char pass[] = "YOUR_WIFI_PASSPHRASE";	/* your network password (use for WPA, or use as key for WEP) */
LWiFiEncryption wmode   = LWIFI_WPA; /* choose from LWIFI_OPEN, LWIFI_WPA, LWIFI_WEP */
int status  = -1;

LWiFiClient wClient;
CloudPlugs client(wClient);
String response;

void printWifiStatus() {
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(LWiFi.SSID());

	// print your WiFi shield's IP address:
	IPAddress ip = LWiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);

	// print the received signal strength:
	long rssi = LWiFi.RSSI();
	Serial.print("signal strength (RSSI): ");
	Serial.print(rssi);
	Serial.println(" dBm \n");
}

void setup() {
	// initialize serial communications at 9600 bps
	Serial.begin(9600);

	// initializes LinkIt ONE WiFi module
	LWiFi.begin();

	/* Attempt to connect to Wifi network */
	while (status != 0) {
		Serial.print("Attempting to connect to SSID: ");
		Serial.println(ssid);
		status = LWiFi.connect(ssid, LWiFiLoginInfo(wmode, pass));
		delay(10000);
	}

	Serial.println("Connected to wifi");
	printWifiStatus();

	response = "";
	bool res = client.enrollProduct("{\"model\":\""AUTH_MODEL"\",\"hwid\":\""AUTH_HWID"\", \"pass\":\""AUTH_PASS"\"}", response);
	debug("ENROLL", res, response);

	char *plugauth = extract_plugid(response.c_str(), "auth");
	Serial.print("plugauth: ");
	Serial.println(plugauth);

	char *plugid = extract_plugid(response.c_str(), "id");
	Serial.print("plugid: ");
	Serial.println(plugid);

	client.setAuth(plugid, plugauth, false);
}

char *extract_plugid(const char* s, char *needle) {
	char *start = strstr(s ,needle);
	if (start) {
		start = strchr(start, ':');
		start = strchr(start, '\"');
		start++;
		char *end = strchr(start, '\"');
		*(end) = '\0';
	}
	return start;
}

void debug(const char* tag, bool res, String& response){
	Serial.print(tag);
	if (res) Serial.print(" ERROR: "); else Serial.print(": ");
	Serial.print(client.getLastHttpResult());
	Serial.print(" - ");
	Serial.println(response);
}

void loop() {
	long rssi = LWiFi.RSSI();
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
