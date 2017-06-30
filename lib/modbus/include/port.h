
#ifndef _MB_PORT_H
#define _MB_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mbcpu.h"
#include "modbus.h"

#if MB_RTU_ENABLED > 0 ||  MB_ASCII_ENABLED > 0

/* ------------ Serial port functions ----------------------------*/
bool xMBPortSerialInit(uint8_t port, uint32_t ulBaudRate, uint8_t ucDataBits, mb_Parity_t eParity);

void xMBPortSerialClose(uint8_t      port);

void vMBPortSerialEnable(uint8_t port, bool xRxEnable, bool xTxEnable );

bool xMBPortSerialGetByte(uint8_t      port, char * pucByte );

bool xMBPortSerialPutByte(uint8_t      port, char ucByte);

/* ------------ Timers functions ---------------------------------*/
bool xMBPortTimersInit(uint8_t port, uint16_t usTimeOut50us );

void xMBPortTimersClose(uint8_t port);

void vMBPortTimersEnable(uint8_t port);

void vMBPortTimersDisable(uint8_t port);

void vMBPortTimersDelay(uint8_t port,uint16_t usTimeOutMS );

uint32_t xMBsys_now(void);
#endif
/* ----------------------- TCP port functions -------------------------------*/
#if MB_TCP_ENABLED > 0

bool xMBTCPPortInit( uint16_t usTCPPort );

void vMBTCPPortClose( void );

void vMBTCPPortDisable( void );

bool xMBTCPPortGetRequest( uint8_t **ppucMBTCPFrame, uint16_t * usTCPLength );

bool xMBTCPPortSendResponse( const uint8_t *pucMBTCPFrame, uint16_t usTCPLength );
#endif

#ifdef __cplusplus
}
#endif
#endif