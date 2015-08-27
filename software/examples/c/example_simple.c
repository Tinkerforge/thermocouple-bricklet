#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_thermocouple.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

int main(void) {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	Thermocouple t;
	thermocouple_create(&t, UID, &ipcon);

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		return 1;
	}
	// Don't use device before ipcon is connected

	// Get current temperature (unit is °C/100)
	int32_t temperature;
	if(thermocouple_get_temperature(&t, &temperature) < 0) {
		fprintf(stderr, "Could not get temperature, probably timeout\n");
		return 1;
	}

	printf("Temperature: %f °C\n", temperature/100.0);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}
