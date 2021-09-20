/* industrial-ptc-bricklet
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * max31865.h: MAX31865 driver
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

#ifndef MAX31865_H
#define MAX31865_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/utility/moving_average.h"

#define MAX31865_DATA_SPI_MAX_LENGTH 16
#define MAX31865_RESISTANCE_MOVING_AVERAGE_DEFAULT 1
#define MAX31865_TEMPERATURE_MOVING_AVERAGE_DEFAULT 40

typedef struct {
	uint8_t noise_filter;
	bool new_noise_filter;
	uint8_t wire_mode;
	bool new_wire_mode;
	uint8_t current_configuration;
	bool fault;
	uint32_t fault_time;

	bool first_value;
	bool sensor_connected_callback_enabled;

	MovingAverage moving_average_temperature;
	MovingAverage moving_average_resistance;

	int32_t current_resistance;
	int32_t current_temperature;
} MAX31865;

void max31865_init(void);
void max31865_tick(void);

int32_t max31865_get_temperature(void);
int32_t max31865_get_resistance(void);


#define REG_WRITE          0x80


#define REG_CONFIGURATION  0x00
#define REG_RTD_MSB        0x01
#define REG_RTD_LSB        0x02
#define REG_HIGH_FAULT_MSB 0x03
#define REG_HIGH_FAULT_LSB 0x04
#define REG_LOW_FAULT_MSB  0x05
#define REG_LOW_FAULT_LSB  0x06
#define REG_FAULT_STATUS   0x07

#define REG_RTD_LSB_FAULT (1 << 0)

#define REG_CONF_VBIAS_ON (1 << 7)
#define REG_CONF_CONVERION_MODE_AUTO (1 << 6)
#define REG_CONF_ONE_SHOT (1 << 5)
#define REG_CONF_3WIRE_RTD (1 << 4)
#define REG_CONF_FAULT_DETECTION
#define REG_CONF_FAULT_STATUS_AUTO_CLEAR (1 << 1)
#define REG_CONF_50HZ_FILTER (1 << 0)

#define REG_FAULT_RTD_HIGH_THRESHOLD (1 << 7)
#define REG_FAULT_RTD_LOW_THRESHOLD (1 << 6)
#define REG_FAULT_RTD_REFIN (1 << 5)
#define REG_FAULT_RTD_REFIN_FORCE (1 << 4)
#define REG_FAULT_RTD_RTDIN_FORCE (1 << 3)
#define REG_FAULT_OVER_UDER_VOLTAGE (1 << 2)

#endif
