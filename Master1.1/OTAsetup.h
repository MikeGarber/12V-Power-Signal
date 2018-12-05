#pragma once
#include <ArduinoOTA.h>


void OTAsetup()
{
		static int lastPercent=0;
	  ArduinoOTA.onStart([]() {
		Serial.println("OTA Start");
	  });
	  ArduinoOTA.onEnd([]() {
		Serial.println("\nOTA End");
	  });
	  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
	    int pct = progress / (total / 100);
		if (abs(pct-lastPercent)>=10){
			Serial.printf("OTA Progress: %u%%\n", (pct));
			lastPercent = pct;}
	  });
	  ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf(" !!!OTA Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("OTA Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("OTA Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("OTA Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("OTA Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("OTA End Failed");
	  });
	  ArduinoOTA.begin();
  }