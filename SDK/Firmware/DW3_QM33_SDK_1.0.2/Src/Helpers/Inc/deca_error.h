/**
 * @file      deca_error.h
 *
 * @brief     Errors definition
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

typedef enum
{
    _NO_ERR = 0,
    _ERR,
    _ERR_Busy,
    _ERR_Timeout,
    _ERR_DEVID,
    _ERR_IWDG,
    _ERR_INSTANCE,
    _ERR_INIT,
    _ERR_IMU_INIT,
    _ERR_TxBuf_Overflow,
    _ERR_RxBuf_Overflow, /* 10 */
    _ERR_Usb_Tx,
    _ERR_Flash_Ob,
    _ERR_Flash_Prog,
    _ERR_Flash_Erase,
    _ERR_Flash_Error,
    _ERR_Flash_Verify,
    _ERR_Flash_Protected,
    _ERR_LSM_R,
    _ERR_LSM_W,
    _ERR_SPI, /* 20 */
    _ERR_SPI_RRX,
    _ERR_SPI_WTX,
    _ERR_SPI_DMA,
    _ERR_UART_DMA,
    _ERR_UART_INIT,
    _ERR_UART_RX,
    _ERR_UART_TX,
    _ERR_UART_RxCplt,
    _ERR_UART_RxCplt_Overflow,
    _ERR_USB_UART_RX, /* 30 */
    _ERR_TWR_CANNOT_START,
    _ERR_MEM_CORRUPTED,
    _ERR_Configure_WKUP_Timer,
    _ERR_PLL,
    /* TWR */
    _ERR_Twr_Bad_State,
    _ERR_Not_Twr_Frame,
    _ERR_Unknown_Tag,
    _ERR_DelayedTX_Late,
    _ERR_Range_Calculation, /* 40 */
    _ERR_Ranging_Config,
    _ERR_RC_Version_Unknown,
    _ERR_Non_Compatible_TWR_Parameters,
    _NO_Err_New_Tag,
    _NO_Err_Tx_Sent,
    _NO_Err_Start_Rx,
    _NO_Err_Final,
    _NO_Err_Ranging_Config,
    _NO_Err_Ranging_Update,
    _NO_Err_Response, /* 50 */
    _NO_Err_Idata,
    _NO_Err_Rdata,
    _NO_Err_Can_Sleep,
    /* USB2SPI */
    _ERR_Usb2Spi_ptr_busy,
    _ERR_Usb2Spi_ptr_alloc,
    /* RTOS */
    _ERR_General_Error,
    _ERR_Create_Task_Bad,
    _ERR_Timer_Create_Bad,
    _ERR_Timer_Start_Bad,
    _ERR_Signal_Bad, /* 60 */
    _ERR_Cannot_Delete_Timer,
    _ERR_Cannot_Delete_Task,
    _ERR_Cannot_Delete_usb2spiTask,
    _ERR_Cannot_Delete_imuTask,
    _ERR_Cannot_Delete_rtlsTask,
    _ERR_Cannot_Delete_rxTask,
    _ERR_Cannot_Delete_calcTask,
    _ERR_Cannot_Delete_twrTask, /* 70 */
    _ERR_Cannot_Delete_commTask,
    _ERR_Cannot_Send_Mail,
    _ERR_Cannot_Alloc_Mail,
    _ERR_Cannot_Alloc_Memory,
    _ERR_Cannot_Alloc_NodeMemory,
    _ERR_Malloc_Failed,
    _ERR_Stack_Overflow,
    _ERR_No_pTwrInfo
} error_e;
