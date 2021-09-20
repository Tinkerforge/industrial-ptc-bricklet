/* industrial-ptc-bricklet
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/utility/callback_value.h"

#include "max31865.h"

extern MAX31865 max31865;

CallbackValue_int32_t callback_value_temperature;
CallbackValue_int32_t callback_value_resistance;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_TEMPERATURE: return get_callback_value_int32_t(message, response, &callback_value_temperature);
		case FID_SET_TEMPERATURE_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_int32_t(message, &callback_value_temperature);
		case FID_GET_TEMPERATURE_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_int32_t(message, response, &callback_value_temperature);
		case FID_GET_RESISTANCE: return get_callback_value_int32_t(message, response, &callback_value_resistance);
		case FID_SET_RESISTANCE_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_int32_t(message, &callback_value_resistance);
		case FID_GET_RESISTANCE_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_int32_t(message, response, &callback_value_resistance);
		case FID_SET_NOISE_REJECTION_FILTER: return set_noise_rejection_filter(message);
		case FID_GET_NOISE_REJECTION_FILTER: return get_noise_rejection_filter(message, response);
		case FID_IS_SENSOR_CONNECTED: return is_sensor_connected(message, response);
		case FID_SET_WIRE_MODE: return set_wire_mode(message);
		case FID_GET_WIRE_MODE: return get_wire_mode(message, response);
		case FID_SET_MOVING_AVERAGE_CONFIGURATION: return set_moving_average_configuration(message);
		case FID_GET_MOVING_AVERAGE_CONFIGURATION: return get_moving_average_configuration(message, response);
		case FID_SET_SENSOR_CONNECTED_CALLBACK_CONFIGURATION: return set_sensor_connected_callback_configuration(message);
		case FID_GET_SENSOR_CONNECTED_CALLBACK_CONFIGURATION: return get_sensor_connected_callback_configuration(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse set_noise_rejection_filter(const SetNoiseRejectionFilter *data) {
	if(data->filter > PTC_V2_FILTER_OPTION_60HZ) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	max31865.noise_filter     = data->filter;
	max31865.new_noise_filter = true;
	if(data->filter == 0) {
		max31865.current_configuration |= REG_CONF_50HZ_FILTER;
	} else {
		max31865.current_configuration &= ~REG_CONF_50HZ_FILTER;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_noise_rejection_filter(const GetNoiseRejectionFilter *data, GetNoiseRejectionFilter_Response *response) {
	response->header.length = sizeof(GetNoiseRejectionFilter_Response);
	response->filter        = max31865.noise_filter;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse is_sensor_connected(const IsSensorConnected *data, IsSensorConnected_Response *response) {
	response->header.length = sizeof(IsSensorConnected_Response);
	response->connected     = !max31865.fault;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_wire_mode(const SetWireMode *data) {
	if(data->mode < PTC_V2_WIRE_MODE_2 || data->mode > PTC_V2_WIRE_MODE_4) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	max31865.wire_mode     = data->mode;
	max31865.new_wire_mode = true;

	if(data->mode == 3) {
		max31865.current_configuration |= REG_CONF_3WIRE_RTD;
	} else {
		max31865.current_configuration &= ~REG_CONF_3WIRE_RTD;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_wire_mode(const GetWireMode *data, GetWireMode_Response *response) {
	response->header.length = sizeof(GetWireMode_Response);
	response->mode          = max31865.wire_mode;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_moving_average_configuration(const SetMovingAverageConfiguration *data) {
	if((data->moving_average_length_resistance  < 1) || (data->moving_average_length_resistance  > 1000) ||
	   (data->moving_average_length_temperature < 1) || (data->moving_average_length_temperature > 1000)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	moving_average_new_length(&max31865.moving_average_resistance,  data->moving_average_length_resistance);
	moving_average_new_length(&max31865.moving_average_temperature, data->moving_average_length_temperature);

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_moving_average_configuration(const GetMovingAverageConfiguration *data, GetMovingAverageConfiguration_Response *response) {
	response->header.length                     = sizeof(GetMovingAverageConfiguration_Response);
	response->moving_average_length_resistance  = max31865.moving_average_resistance.length;
	response->moving_average_length_temperature = max31865.moving_average_temperature.length;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_sensor_connected_callback_configuration(const SetSensorConnectedCallbackConfiguration *data) {
	max31865.sensor_connected_callback_enabled = data->enabled;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_sensor_connected_callback_configuration(const GetSensorConnectedCallbackConfiguration *data, GetSensorConnectedCallbackConfiguration_Response *response) {
	response->header.length = sizeof(GetSensorConnectedCallbackConfiguration_Response);
	response->enabled       = max31865.sensor_connected_callback_enabled;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_temperature_callback(void) {
	return handle_callback_value_callback_int32_t(&callback_value_temperature, FID_CALLBACK_TEMPERATURE);
}

bool handle_resistance_callback(void) {
	return handle_callback_value_callback_int32_t(&callback_value_resistance, FID_CALLBACK_RESISTANCE);
}

bool handle_sensor_connected_callback(void) {
	static bool is_buffered = false;
	static SensorConnected_Callback cb;
	static bool last_fault = true;

	if(!is_buffered) {
		if(!max31865.sensor_connected_callback_enabled) {
			return false;
		}

		if(last_fault == max31865.fault) {
			return false;
		}

		last_fault = max31865.fault;
		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(SensorConnected_Callback), FID_CALLBACK_SENSOR_CONNECTED);
		cb.connected = !last_fault;
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(SensorConnected_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	callback_value_init_int32_t(&callback_value_temperature, max31865_get_temperature);
	callback_value_init_int32_t(&callback_value_resistance, max31865_get_resistance);

	communication_callback_init();
}
