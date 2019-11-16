#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#endif

#include "ThingSpeak.h"
#include "config.h"

void sendDataToThingSpeak(float currentTemperature, float currentPressure, float currentHumidity, int averagePM1, int averagePM25, int averagePM4, int averagePM10, float dewPoint, float averageVcc) {
    if (!(THINGSPEAK_ON)) {
        return;
    }

    WiFiClient client;
    ThingSpeak.begin(client);
	if (strcmp(DUST_MODEL, "Non")) {
		ThingSpeak.setField(1, averagePM1);
    	ThingSpeak.setField(2, averagePM25);
    	ThingSpeak.setField(3, averagePM10);
	}
	if (strcmp(THP_MODEL, "Non")) {
	    ThingSpeak.setField(4, currentTemperature);
	    ThingSpeak.setField(5, currentHumidity);
		ThingSpeak.setField(6, dewPoint);
		ThingSpeak.setField(7, currentPressure);
	}
	ThingSpeak.setField(8, averageVcc);
    ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_API_KEY);
    client.stop();
}
