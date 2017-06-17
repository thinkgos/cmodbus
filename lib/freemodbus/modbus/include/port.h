
#ifndef _MB_PORT_H
#define _MB_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mbcpu.h"

/*! \ingroup modbus
 * \brief Parity used for characters in serial mode.
 *
 * The parity which should be applied to the characters sent over the serial
 * link. Please note that this values are actually passed to the porting
 * layer and therefore not all parity modes might be available.
 */
typedef enum
{
    MB_PAR_NONE,                /*!< No parity. */
    MB_PAR_ODD,                 /*!< Odd parity. */
    MB_PAR_EVEN                 /*!< Even parity. */
} eMBParity;

/* ------------ Serial port functions ----------------------------*/
bool xMBPortSerialInit(uint8_t port, uint32_t ulBaudRate, uint8_t ucDataBits, eMBParity eParity);

void vMBPortClose(uint8_t port);

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


/* ----------------------- TCP port functions -------------------------------*/
bool xMBTCPPortInit( uint16_t usTCPPort );

void vMBTCPPortClose( void );

void vMBTCPPortDisable( void );

bool xMBTCPPortGetRequest( uint8_t **ppucMBTCPFrame, uint16_t * usTCPLength );

bool xMBTCPPortSendResponse( const uint8_t *pucMBTCPFrame, uint16_t usTCPLength );

#ifdef __cplusplus
}
#endif
#endif