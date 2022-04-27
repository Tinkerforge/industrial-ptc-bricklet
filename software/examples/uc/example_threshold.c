// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_industrial_ptc.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

// Callback function for temperature callback
static void temperature_handler(TF_IndustrialPTC *device, int32_t temperature,
                                void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Temperature: %d 1/%d °C\n", temperature, 100);
}

static TF_IndustrialPTC ptc;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_industrial_ptc_create(&ptc, NULL, hal), "create device object");

	// Register temperature callback to function temperature_handler
	tf_industrial_ptc_register_temperature_callback(&ptc,
	                                                temperature_handler,
	                                                NULL);

	// Configure threshold for temperature "greater than 30 °C"
	// with a debounce period of 1s (1000ms)
	tf_industrial_ptc_set_temperature_callback_configuration(&ptc, 1000, false, '>', 30*100, 0);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
