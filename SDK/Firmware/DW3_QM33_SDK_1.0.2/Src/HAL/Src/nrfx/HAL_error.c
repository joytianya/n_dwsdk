/**
 * @file      HAL_error.c
 *
 * @brief     Error handlers
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "nrf_drv_wdt.h"
#include "nrf_delay.h"
#include "HAL_error.h"
#include "boards.h"
#include "FreeRTOS.h"
#include "task.h"

extern nrf_drv_wdt_channel_id m_channel_id;

static error_e lastErrorCode;

void error_handler(int block, error_e err)
{
    lastErrorCode = err;

    /* Flash Error Led. */
    while (block)
    {
        for (int i = err; i > 0; i--)
        {
            for (int j = 3; j > 0; j--)
            {
                /* WDG_Refresh. */
                nrf_drv_wdt_channel_feed(m_channel_id);

                nrf_gpio_pin_write(LED_ERROR, 0);
                nrf_delay_ms(100);
                nrf_gpio_pin_write(LED_ERROR, 1);
                nrf_delay_ms(100);
            }
            nrf_delay_ms(1000);
        }
    }
}

error_e get_lastErrorCode(void)
{
    return lastErrorCode;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    ASSERT(false);
}
