/* industrial-ptc-bricklet
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define PTC_V2_THRESHOLD_OPTION_OFF 'x'
#define PTC_V2_THRESHOLD_OPTION_OUTSIDE 'o'
#define PTC_V2_THRESHOLD_OPTION_INSIDE 'i'
#define PTC_V2_THRESHOLD_OPTION_SMALLER '<'
#define PTC_V2_THRESHOLD_OPTION_GREATER '>'

#define PTC_V2_FILTER_OPTION_50HZ 0
#define PTC_V2_FILTER_OPTION_60HZ 1

#define PTC_V2_WIRE_MODE_2 2
#define PTC_V2_WIRE_MODE_3 3
#define PTC_V2_WIRE_MODE_4 4

#define PTC_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define PTC_V2_BOOTLOADER_MODE_FIRMWARE 1
#define PTC_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define PTC_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define PTC_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define PTC_V2_BOOTLOADER_STATUS_OK 0
#define PTC_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define PTC_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define PTC_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define PTC_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define PTC_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define PTC_V2_STATUS_LED_CONFIG_OFF 0
#define PTC_V2_STATUS_LED_CONFIG_ON 1
#define PTC_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define PTC_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_GET_TEMPERATURE 1
#define FID_SET_TEMPERATURE_CALLBACK_CONFIGURATION 2
#define FID_GET_TEMPERATURE_CALLBACK_CONFIGURATION 3
#define FID_GET_RESISTANCE 5
#define FID_SET_RESISTANCE_CALLBACK_CONFIGURATION 6
#define FID_GET_RESISTANCE_CALLBACK_CONFIGURATION 7
#define FID_SET_NOISE_REJECTION_FILTER 9
#define FID_GET_NOISE_REJECTION_FILTER 10
#define FID_IS_SENSOR_CONNECTED 11
#define FID_SET_WIRE_MODE 12
#define FID_GET_WIRE_MODE 13
#define FID_SET_MOVING_AVERAGE_CONFIGURATION 14
#define FID_GET_MOVING_AVERAGE_CONFIGURATION 15
#define FID_SET_SENSOR_CONNECTED_CALLBACK_CONFIGURATION 16
#define FID_GET_SENSOR_CONNECTED_CALLBACK_CONFIGURATION 17

#define FID_CALLBACK_TEMPERATURE 4
#define FID_CALLBACK_RESISTANCE 8
#define FID_CALLBACK_SENSOR_CONNECTED 18

typedef struct {
	TFPMessageHeader header;
	uint8_t filter;
} __attribute__((__packed__)) SetNoiseRejectionFilter;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetNoiseRejectionFilter;

typedef struct {
	TFPMessageHeader header;
	uint8_t filter;
} __attribute__((__packed__)) GetNoiseRejectionFilter_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) IsSensorConnected;

typedef struct {
	TFPMessageHeader header;
	bool connected;
} __attribute__((__packed__)) IsSensorConnected_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t mode;
} __attribute__((__packed__)) SetWireMode;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetWireMode;

typedef struct {
	TFPMessageHeader header;
	uint8_t mode;
} __attribute__((__packed__)) GetWireMode_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t moving_average_length_resistance;
	uint16_t moving_average_length_temperature;
} __attribute__((__packed__)) SetMovingAverageConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetMovingAverageConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint16_t moving_average_length_resistance;
	uint16_t moving_average_length_temperature;
} __attribute__((__packed__)) GetMovingAverageConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	bool enabled;
} __attribute__((__packed__)) SetSensorConnectedCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetSensorConnectedCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	bool enabled;
} __attribute__((__packed__)) GetSensorConnectedCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	bool connected;
} __attribute__((__packed__)) SensorConnected_Callback;


// Function prototypes
BootloaderHandleMessageResponse set_noise_rejection_filter(const SetNoiseRejectionFilter *data);
BootloaderHandleMessageResponse get_noise_rejection_filter(const GetNoiseRejectionFilter *data, GetNoiseRejectionFilter_Response *response);
BootloaderHandleMessageResponse is_sensor_connected(const IsSensorConnected *data, IsSensorConnected_Response *response);
BootloaderHandleMessageResponse set_wire_mode(const SetWireMode *data);
BootloaderHandleMessageResponse get_wire_mode(const GetWireMode *data, GetWireMode_Response *response);
BootloaderHandleMessageResponse set_moving_average_configuration(const SetMovingAverageConfiguration *data);
BootloaderHandleMessageResponse get_moving_average_configuration(const GetMovingAverageConfiguration *data, GetMovingAverageConfiguration_Response *response);
BootloaderHandleMessageResponse set_sensor_connected_callback_configuration(const SetSensorConnectedCallbackConfiguration *data);
BootloaderHandleMessageResponse get_sensor_connected_callback_configuration(const GetSensorConnectedCallbackConfiguration *data, GetSensorConnectedCallbackConfiguration_Response *response);

// Callbacks
bool handle_temperature_callback(void);
bool handle_resistance_callback(void);
bool handle_sensor_connected_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 3
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_temperature_callback, \
	handle_resistance_callback, \
	handle_sensor_connected_callback, \


#endif
