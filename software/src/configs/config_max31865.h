/* industrial-ptc-bricklet
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_max31865.h: Configuration for MAX31865
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

#ifndef CONFIG_MAX31865_H
#define CONFIG_MAX31865_H

#include "xmc_gpio.h"
#include "xmc_spi.h"

#define MAX31865_FIX_LED_PIN          P1_1

#define MAX31865_USIC_CHANNEL         USIC0_CH1
#define MAX31865_USIC                 XMC_SPI0_CH1

#define MAX31865_SCLK_PIN             P0_8
#define MAX31865_SCLK_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_8_AF_U0C1_SCLKOUT)

#define MAX31865_SELECT_PIN           P0_9
#define MAX31865_SELECT_PIN_AF        (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_9_AF_U0C1_SELO0)

#define MAX31865_MOSI_PIN             P0_7
#define MAX31865_MOSI_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_7_AF_U0C1_DOUT0)

#define MAX31865_MISO_PIN             P0_6
#define MAX31865_MISO_INPUT           XMC_USIC_CH_INPUT_DX0
#define MAX31865_MISO_SOURCE          0b010 // DX0C

#define MAX31865_SERVICE_REQUEST_RX   2
#define MAX31865_SERVICE_REQUEST_TX   3

#define MAX31865_IRQ_RX               11
#define MAX31865_IRQ_RX_PRIORITY      2
#define MAX31865_IRQ_TX               12
#define MAX31865_IRQ_TX_PRIORITY      3

#endif
