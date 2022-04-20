/* industrial-ptc-bricklet
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * max31865.c: MAX31865 driver
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

#include "max31865.h"

#include "configs/config_max31865.h"
#include "bricklib2/os/coop_task.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include "communication.h"

MAX31865 max31865;
CoopTask max31865_task;

extern const int16_t pt_values[];

// Here we assume that length is always smaller then the fifo size of 16 byte
void max31865_transceive_task(uint8_t *data, const uint8_t length) {
	XMC_SPI_CH_EnableSlaveSelect(MAX31865_USIC, XMC_SPI_CH_SLAVE_SELECT_0);

	for(uint8_t i = 0; i < length; i++) {
		MAX31865_USIC->IN[0] = data[i];
	}

	uint8_t i = 0;
	while(i < length) {
		while(!XMC_USIC_CH_RXFIFO_IsEmpty(MAX31865_USIC)) {
			data[i] = MAX31865_USIC->OUTR;
			i++;
		}
		coop_task_yield();
	}

	XMC_SPI_CH_DisableSlaveSelect(MAX31865_USIC);
}

uint16_t max31865_read_resistance(void) {
	uint8_t data[3] = {REG_RTD_MSB, 0, 0};
	max31865_transceive_task(data, 3);

	return data[2] | (data[1] << 8);
}

uint8_t max31865_read_register_task(const uint8_t reg) {
	uint8_t data[2] = {reg, 0};
	max31865_transceive_task(data, 2);

	return data[1];
}

void max31865_write_register_task(const uint8_t reg, const uint8_t value) {
	uint8_t data[2] = {reg | REG_WRITE, value};
	max31865_transceive_task(data, 2);
}


void max31865_init_spi(void) {
	// default pin configuration
	const XMC_GPIO_CONFIG_t default_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	// Start by disabling everything in case of a re-init
	XMC_USIC_CH_Disable(MAX31865_USIC);
	XMC_GPIO_Init(MAX31865_MISO_PIN,   &default_pin_config);
	XMC_GPIO_Init(MAX31865_MOSI_PIN,   &default_pin_config);
	XMC_GPIO_Init(MAX31865_SCLK_PIN,   &default_pin_config);
	XMC_GPIO_Init(MAX31865_SELECT_PIN, &default_pin_config);



	// USIC channel configuration
	const XMC_SPI_CH_CONFIG_t channel_config = {
		.baudrate       = 100000,
		.bus_mode       = XMC_SPI_CH_BUS_MODE_MASTER,
		.selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
		.parity_mode    = XMC_USIC_CH_PARITY_MODE_NONE
	};

	// MOSI pin configuration
	const XMC_GPIO_CONFIG_t mosi_pin_config = {
		.mode             = MAX31865_MOSI_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// MISO pin configuration
	const XMC_GPIO_CONFIG_t miso_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	// SCLK pin configuration
	const XMC_GPIO_CONFIG_t sclk_pin_config = {
		.mode             = MAX31865_SCLK_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// SELECT pin configuration
	const XMC_GPIO_CONFIG_t select_pin_config = {
		.mode             = MAX31865_SELECT_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// Configure MISO pin
	XMC_GPIO_Init(MAX31865_MISO_PIN, &miso_pin_config);

	// Initialize USIC channel in SPI master mode
	XMC_SPI_CH_Init(MAX31865_USIC, &channel_config);
	MAX31865_USIC->SCTR &= ~USIC_CH_SCTR_PDL_Msk; // Set passive data level to 0

	XMC_SPI_CH_SetBitOrderMsbFirst(MAX31865_USIC);

	XMC_SPI_CH_SetWordLength(MAX31865_USIC, (uint8_t)8U);
	XMC_SPI_CH_SetFrameLength(MAX31865_USIC, (uint8_t)64U);

	XMC_SPI_CH_SetTransmitMode(MAX31865_USIC, XMC_SPI_CH_MODE_STANDARD);

	// Configure the clock polarity and clock delay
	XMC_SPI_CH_ConfigureShiftClockOutput(MAX31865_USIC,
	                                     XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_DISABLED,
										 XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);
	// Configure Leading/Trailing delay
	XMC_SPI_CH_SetSlaveSelectDelay(MAX31865_USIC, 2);

	// Set input source path
	XMC_SPI_CH_SetInputSource(MAX31865_USIC, MAX31865_MISO_INPUT, MAX31865_MISO_SOURCE);

	// SPI Mode: CPOL=1 and CPHA=1
	MAX31865_USIC_CHANNEL->DX1CR |= USIC_CH_DX1CR_DPOL_Msk;

	// Configure transmit FIFO
	XMC_USIC_CH_TXFIFO_Configure(MAX31865_USIC, 48, XMC_USIC_CH_FIFO_SIZE_16WORDS, 0);

	// Configure receive FIFO
	XMC_USIC_CH_RXFIFO_Configure(MAX31865_USIC, 32, XMC_USIC_CH_FIFO_SIZE_16WORDS, 0);

	// Start SPI
	XMC_SPI_CH_Start(MAX31865_USIC);

	// Configure SCLK pin
	XMC_GPIO_Init(MAX31865_SCLK_PIN, &sclk_pin_config);

	// Configure slave select pin
	XMC_GPIO_Init(MAX31865_SELECT_PIN, &select_pin_config);

	//Configure MOSI pin
	XMC_GPIO_Init(MAX31865_MOSI_PIN, &mosi_pin_config);

	XMC_USIC_CH_RXFIFO_EnableEvent(MAX31865_USIC, XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD | XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE);
}

void max31865_tick_task(void) {
	max31865_write_register_task(REG_CONFIGURATION, REG_CONF_50HZ_FILTER);
	max31865.current_configuration = REG_CONF_VBIAS_ON | REG_CONF_50HZ_FILTER | REG_CONF_CONVERION_MODE_AUTO;
	max31865_write_register_task(REG_CONFIGURATION, max31865.current_configuration | REG_CONF_FAULT_STATUS_AUTO_CLEAR);

	while(true) {
		if(max31865.new_wire_mode) {
			max31865_write_register_task(REG_CONFIGURATION, max31865.current_configuration | REG_CONF_FAULT_STATUS_AUTO_CLEAR);
			max31865.new_wire_mode = false;
		}

		if(max31865.new_noise_filter) {
			// Turn conversion mode off while changing filter
			max31865_write_register_task(REG_CONFIGURATION, max31865.current_configuration & (~REG_CONF_CONVERION_MODE_AUTO));
			coop_task_sleep_ms(2);

			// Turn conversion mode on again
			max31865_write_register_task(REG_CONFIGURATION, max31865.current_configuration | REG_CONF_FAULT_STATUS_AUTO_CLEAR);
			max31865.new_noise_filter = false;
		}

		uint16_t resistance = max31865_read_resistance();

		if(resistance & REG_RTD_LSB_FAULT) {
			uint8_t fault = max31865_read_register_task(REG_FAULT_STATUS);
			max31865_write_register_task(REG_CONFIGURATION, max31865.current_configuration | REG_CONF_FAULT_STATUS_AUTO_CLEAR);
			max31865.fault = fault != 0;
			max31865.fault_time = system_timer_get_ms();
		} else {
			// 500ms debounce to go from not connected to connected.
			// Otherwise we had some false positives during testing
			if(system_timer_is_time_elapsed_ms(max31865.fault_time, 500)) {
				max31865.fault = 0;
			}

			resistance >>= 1;

			// If resistance is 0, we assume that the MAX31865 is in a broken state
			if(resistance == 0) {
				max31865_init_spi();
				max31865.new_wire_mode    = true;
				max31865.new_noise_filter = true;
			} else {
				int16_t rest = resistance % 100;
				int32_t temperature = (pt_values[resistance/100  ]*(100-rest) +
									pt_values[resistance/100+1]*(rest    ))/25;

				if(max31865.first_value) {
					max31865.first_value = false;
					moving_average_init(&max31865.moving_average_resistance,  resistance,  MAX31865_RESISTANCE_MOVING_AVERAGE_DEFAULT);
					moving_average_init(&max31865.moving_average_temperature, temperature, MAX31865_TEMPERATURE_MOVING_AVERAGE_DEFAULT);
					max31865.current_resistance  = resistance;
					max31865.current_temperature = temperature;
				} else {
					max31865.current_resistance  = moving_average_handle_value(&max31865.moving_average_resistance,  resistance);
					max31865.current_temperature = moving_average_handle_value(&max31865.moving_average_temperature, temperature);
				}
			}
		}

		coop_task_sleep_ms(20);
	}
}

void max31865_init(void) {
	// Initialize the whole struct as zero
	memset(&max31865, 0, sizeof(MAX31865));
	max31865.first_value  = true;
	max31865.wire_mode    = PTC_V2_WIRE_MODE_2;
	max31865.noise_filter = PTC_V2_FILTER_OPTION_50HZ;

	max31865_init_spi();

	coop_task_init(&max31865_task, max31865_tick_task);
}

void max31865_tick(void) {
	coop_task_tick(&max31865_task);
}

int32_t max31865_get_temperature(void) {
	return max31865.current_temperature;
}

int32_t max31865_get_resistance(void) {
	return max31865.current_resistance;
}


