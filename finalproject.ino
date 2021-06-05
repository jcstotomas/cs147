// This #include statement was automatically added by the Particle IDE.
#include <HttpClient.h>

// This #include statement was automatically added by the Particle IDE.
#include "SparkFun_RHT03.h"

// This #include statement was automatically added by the Particle IDE.
#include <HttpClient.h>
#include "application.h"
#define LIGHT_SENSOR A0
#define SOUND_SENSOR A1
unsigned int nextTime = 0; // Next time to contact the server
const int RHT03_DATA_PIN = 4; 
HttpClient http;
RHT03 rht; 


// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
// { "Content-Type", "application/json" },
// { "Accept" , "application/json" },
{ "Accept" , "*/*"},
{ NULL, NULL } // NOTE: Always terminate headers will NULL
};
http_request_t request;
http_response_t response;



void setup() {
Serial.begin(9600);
int sensorValue = analogRead(LIGHT_SENSOR); 
rht.begin(RHT03_DATA_PIN);
}
void loop() {
if (nextTime > millis()) {
return;
}

	int updateRet = rht.update();
	
	// If successful, the update() function will return 1.
	// If update fails, it will return a value <0
	if (updateRet == 1)
	{

		float latestHumidity = rht.humidity();
		float latestTempF = rht.tempF();
		int sensorValue = analogRead(LIGHT_SENSOR);
		int voltage = map(sensorValue, 0, 1023, 0, 5);
		long sum = 0;
        for(int i=0; i<32; i++)
        {
            sum += analogRead(SOUND_SENSOR);
        }
     
        sum >>= 5;
     
        Serial.println(sum);
		Serial.println(latestHumidity);
		Serial.println(latestTempF);
		Serial.println(voltage);
        Serial.println("Application>\tStart of Loop.");
        // Request path and body can be set at runtime or at setup.
        request.hostname = "3.20.223.22";
        request.port = 5000;
        // request.path = "/?temp=" + String(latestTempF, 1) + "&" + "humid=" + String(latestHumidity, 1);
        // The library also supports sending a body with your request:
        //request.body = "{\"key\":\"value\"}";
        // Get request
        http.get(request, response, headers);
        Serial.print("Application>\tResponse status: ");
        Serial.println(response.status);
        Serial.print("Application>\tHTTP Response Body: ");
        Serial.println(response.body);
        nextTime = millis() + 10000;
	}
	else
	{
		// If the update failed, try delaying for RHT_READ_INTERVAL_MS ms before
		// trying again.
		delay(RHT_READ_INTERVAL_MS);
	}
}
