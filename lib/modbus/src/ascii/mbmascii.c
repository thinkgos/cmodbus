
#include "port.h"
#include "mbutils.h"
#include "mbascii.h"

#if MB_ASCII_ENABLED > 0 &&  MB_MASTER_ENABLED > 0

MbErrorCode_t MbmASCIIInit(void *dev, uint8_t ucPort,uint32_t ulBaudRate, MbParity_t eParity )
{
    MbErrorCode_t eStatus = MB_ENOERR;
    (void)dev;
    
    ENTER_CRITICAL_SECTION();

    if(MbPortSerialInit( ucPort, ulBaudRate, 7, eParity) != TRUE){
        eStatus = MB_EPORTERR;
    }
    else if(MbPortTimersInit(ucPort, MBS_ASCII_TIMEOUT_SEC * 20000UL) != TRUE){
        eStatus = MB_EPORTERR;
    }
    
    EXIT_CRITICAL_SECTION();

    return eStatus;
}

void MbmASCIIStart(void *dev)
{
    ENTER_CRITICAL_SECTION();

    ((MbmDev_t *)dev)->sndrcvState = STATE_ASCII_RX_IDLE;
    MbPortSerialEnable(((MbmDev_t *)dev)->port, TRUE, FALSE);
    
    EXIT_CRITICAL_SECTION();
}

void MbmASCIIStop(void *dev)
{
    ENTER_CRITICAL_SECTION();
    
    MbPortSerialEnable(((MbmDev_t *)dev)->port, FALSE, FALSE);
    MbPortTimersDisable(((MbmDev_t *)dev)->port);
    
    EXIT_CRITICAL_SECTION();
}
void MbmASCIIClose(void *dev)
{

}
MbReqResult_t MbmASCIIReceive(void *dev,MbHeader_t *phead,uint8_t *pfunCode, uint8_t **premain, uint16_t *premainLength)
{
    MbReqResult_t result = MBR_ENOERR;
    MbmDev_t *pdev = (MbmDev_t *)dev;

    ENTER_CRITICAL_SECTION();

    /* Length and LRC check */
    if((pdev->rcvAduBufPos >= 4)/* addr+funcode+(other >= 1)+lrc(1)  */
        && (MbLRC((uint8_t *)pdev->AduBuf, pdev->rcvAduBufPos) == 0)){
        phead->introute.slaveid = pdev->AduBuf[MB_SER_ADU_ADDR_OFFSET];
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        *pfunCode = pdev->AduBuf[MB_SER_ADU_PDU_OFFSET + MB_PDU_FUNCODE_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *premainLength = (uint16_t)(pdev->rcvAduBufPos - MB_SER_ADU_SIZE_ADDR - MB_PDU_SIZE_FUNCODE - MB_SER_ADU_SIZE_LRC);

        /* Return the start of the Modbus PDU to the caller. */
        *premain = (uint8_t *) & pdev->AduBuf[MB_SER_ADU_PDU_OFFSET + MB_PDU_DATA_OFF];
    }
    else if(pdev->rcvAduBufPos < 4){
        result = MBR_MISSBYTE;
    }else{
        result = MBR_ECHECK;
    }
    
    EXIT_CRITICAL_SECTION();
    
    return result;
}

MbReqResult_t MbmASCIISend(void *dev,const uint8_t *pAdu, uint16_t usAduLength)
{
    MbReqResult_t result = MBR_ENOERR;
    uint8_t ucByte;
    MbmDev_t *pdev = (MbmDev_t *)dev;
    
    ENTER_CRITICAL_SECTION(  );
    /* Check if the receiver is still in idle state. If not we where too
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if(pdev->sndrcvState == STATE_ASCII_RX_IDLE){
        // copy to sendbuff
        pdev->sndAduBufCount = usAduLength;
        memcpy((uint8_t *)pdev->AduBuf,pAdu,usAduLength);

        /* Activate the transmitter. */
        pdev->sndrcvState = STATE_ASCII_TX_START;

        /* start the first transmitter then into serial tc interrupt */
        ucByte = ':';
        MbPortSerialPutByte(pdev->port,(char)ucByte);
        pdev->sndAduBufPos = 0;
        pdev->AsciiBytePos = BYTE_HIGH_NIBBLE;
        pdev->sndrcvState = STATE_ASCII_TX_DATA;
      
        MbPortSerialEnable(pdev->port, FALSE, TRUE );
    }
    else{
        result = MBR_BUSY;
    }
    EXIT_CRITICAL_SECTION();
    
    return result;
}

void MbmASCIIReceiveFSM(MbmDev_t *dev)
{
    uint8_t ucByte;
    uint8_t ucResult;

    (void)MbPortSerialGetByte(dev->port, (char *)&ucByte );
    switch (dev->sndrcvState){
        /* A new character is received. If the character is a ':' the input
         * buffer is cleared. A CR-character signals the end of the data
         * block. Other characters are part of the data block and their
         * ASCII value is converted back to a binary representation.
         */
    case STATE_ASCII_RX_RCV:
        /* Enable timer for character timeout. */
        MbPortTimersEnable(dev->port);
        if( ucByte == ':' ){
            /* Empty receive buffer. */
            dev->AsciiBytePos = BYTE_HIGH_NIBBLE;
            dev->rcvAduBufPos = 0;
        }
        else if( ucByte == MB_ASCII_DEFAULT_CR ){
            dev->sndrcvState = STATE_ASCII_RX_WAIT_EOF;
        }
        else{
            ucResult = MbChar2Bin( ucByte );
            switch (dev->AsciiBytePos){
                /* High nibble of the byte comes first. We check for
                 * a buffer overflow here. */
            case BYTE_HIGH_NIBBLE:
                if( dev->rcvAduBufPos < MB_ADU_SIZE_MAX ){
                    dev->AduBuf[dev->rcvAduBufPos] = (uint8_t)(ucResult << 4);
                    dev->AsciiBytePos = BYTE_LOW_NIBBLE;
                    break;
                }
                else{
                    /* not handled in Modbus specification but seems
                     * a resonable implementation. */
                    dev->sndrcvState = STATE_ASCII_RX_IDLE;
                    /* Disable previously activated timer because of error state. */
                    MbPortTimersDisable(dev->port);
                }
                break;

            case BYTE_LOW_NIBBLE:
                dev->AduBuf[dev->rcvAduBufPos] |= ucResult;
                dev->rcvAduBufPos++;
                dev->AsciiBytePos = BYTE_HIGH_NIBBLE;
                break;
            }
        }
        break;

    case STATE_ASCII_RX_WAIT_EOF:
        if( ucByte == MB_ASCII_DEFAULT_LF ){
            /* Disable character timeout timer because all characters are
             * received. */
            MbPortTimersDisable(dev->port);
            /* Receiver is again in idle state. */
            dev->sndrcvState = STATE_ASCII_RX_IDLE;

            /* Notify the caller of MbsASCIIReceive that a new frame was received. */
            if(dev->Pollstate == MBM_WAITRSP);
                MbmSetPollmode(dev, MBM_RSPEXCUTE);
         }
        else if( ucByte == ':' ){
            /* Empty receive buffer and back to receive state. */
            dev->AsciiBytePos = BYTE_HIGH_NIBBLE;
            dev->rcvAduBufPos = 0;
            dev->sndrcvState = STATE_ASCII_RX_RCV;

            /* Enable timer for character timeout. */
            MbPortTimersEnable(dev->port);
        }
        else{
            /* Frame is not okay. Delete entire frame. */
            dev->sndrcvState = STATE_ASCII_RX_IDLE;
        }
        break;

    case STATE_ASCII_RX_IDLE:
        if( ucByte == ':' ){
            /* Enable timer for character timeout. */
            MbPortTimersEnable(dev->port);
            /* Reset the input buffers to store the frame. */
            dev->rcvAduBufPos = 0;
            dev->AsciiBytePos = BYTE_HIGH_NIBBLE;
            dev->sndrcvState = STATE_ASCII_RX_RCV;
        }
        break;
    }
}

void MbmASCIITransmitFSM(MbmDev_t *dev)
{
    uint8_t ucByte;
    
    switch(dev->sndrcvState){
        /* Start of transmission. The start of a frame is defined by sending
         * the character ':'. */
    case STATE_ASCII_TX_START:
        ucByte = ':';
        MbPortSerialPutByte(dev->port, (char)ucByte );
        dev->sndAduBufPos = 0;
        dev->AsciiBytePos = BYTE_HIGH_NIBBLE;
        dev->sndrcvState = STATE_ASCII_TX_DATA;
        break;

        /* Send the data block. Each data byte is encoded as a character hex
         * stream with the high nibble sent first and the low nibble sent
         * last. If all data bytes are exhausted we send a '\r' character
         * to end the transmission. */
    case STATE_ASCII_TX_DATA:
        if( dev->sndAduBufCount > 0 ){
            switch(dev->AsciiBytePos){
            case BYTE_HIGH_NIBBLE:
                ucByte = MbBin2Char((uint8_t)(dev->AduBuf[dev->sndAduBufPos] >> 4));
                MbPortSerialPutByte(dev->port, (char)ucByte);
                dev->AsciiBytePos = BYTE_LOW_NIBBLE;
                break;

            case BYTE_LOW_NIBBLE:
                ucByte = MbBin2Char((uint8_t)(dev->AduBuf[dev->sndAduBufPos] & 0x0F));
                MbPortSerialPutByte(dev->port, (char)ucByte);
                dev->sndAduBufPos++;
                dev->sndAduBufCount--;                
                dev->AsciiBytePos = BYTE_HIGH_NIBBLE;
                break;
            }
        }
        else{
            MbPortSerialPutByte(dev->port, (char)MB_ASCII_DEFAULT_CR);
            dev->sndrcvState = STATE_ASCII_TX_END;
        }
        break;

        /* Finish the frame by sending a LF character. */
    case STATE_ASCII_TX_END:
        MbPortSerialPutByte(dev->port, (char)MB_ASCII_DEFAULT_LF);
        /* We need another state to make sure that the CR character has
         * been sent. */
        dev->sndrcvState = STATE_ASCII_TX_NOTIFY;
        break;

        /* Notify the task which called MbsASCIISend that the frame has
         * been sent. */
    case STATE_ASCII_TX_NOTIFY:
        /* Disable transmitter. This prevents another transmit buffer
         * empty interrupt. */
        MbPortSerialEnable(dev->port, TRUE, FALSE);
        dev->sndrcvState = STATE_ASCII_RX_IDLE;

         if(dev->Pollstate == MBM_XMITING)
            MbmSetPollmode(dev, MBM_WAITRSP); // ������ϣ�����ȴ�Ӧ��
        break;
    }
}

void MbmASCIITimerT1SExpired(MbmDev_t *dev)
{
    /* If we have a timeout we go back to the idle state and wait for
     * the next frame.
     */
    if((dev->sndrcvState == STATE_ASCII_RX_RCV) || (dev->sndrcvState == STATE_ASCII_RX_WAIT_EOF)){
        dev->sndrcvState = STATE_ASCII_RX_IDLE;
    }
    MbPortTimersDisable(dev->port);
}

#endif
