// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_industrial_ptc.h"

#define UID "XYZ" // Change XYZ to the UID of your Industrial PTC Bricklet

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


static TF_IndustrialPTC ip;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_industrial_ptc_create(&ip, UID, hal), "create device object");

	// Get current temperature
	int32_t temperature;
	check(tf_industrial_ptc_get_temperature(&ip, &temperature), "get temperature");

	tf_hal_printf("Temperature: %d 1/%d °C\n", temperature, 100);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
