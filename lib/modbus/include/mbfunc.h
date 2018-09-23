
#ifndef __MB_FUNC_H
#define __MB_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mbconfig.h"
#include "mbproto.h"
#include "mbcpu.h"

#include "mbutils.h"
#include "mb.h"
    
// read holding input coil disc offset in pdu
#define MB_PDU_FUNC_READ_ADDR_OFF           ( MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_READ_REGCNT_OFF         ( MB_PDU_DATA_OFF + 2 ) // only for holding reg
#define MB_PDU_FUNC_READ_BITSCNT_OFF        ( MB_PDU_DATA_OFF + 2 ) // only for coils
#define MB_PDU_FUNC_READ_SIZE               ( 4 )

// write single holding coil offset in pdu
#define MB_PDU_FUNC_WRITE_ADDR_OFF          ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_WRITE_VALUE_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_SIZE              ( 4 )

// write multiple holding coils offset in pdu
#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF      ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF    ( MB_PDU_DATA_OFF + 2 ) // for holding reg
#define MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF   ( MB_PDU_DATA_OFF + 2 ) // for coils
#define MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF   ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_WRITE_MUL_VALUES_OFF    ( MB_PDU_DATA_OFF + 5 )
#define MB_PDU_FUNC_WRITE_MUL_SIZE_MIN      ( 5 )

// readwrite multiple holding offset in pdu
#define MB_PDU_FUNC_READWRITE_READ_ADDR_OFF     ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF   ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF    ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF  ( MB_PDU_DATA_OFF + 6 )
#define MB_PDU_FUNC_READWRITE_BYTECNT_OFF       ( MB_PDU_DATA_OFF + 8 )
#define MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF  ( MB_PDU_DATA_OFF + 9 )
#define MB_PDU_FUNC_READWRITE_SIZE_MIN          ( 9 )

/*! \ingroup modbus
 * \brief If register should be written or read.
 *
 * This value is passed to the callback functions which support either
 * reading or writing register values. Writing means that the application
 * registers should be updated and reading means that the modbus protocol
 * stack needs to know the current register values.
 *
 * \see eMBRegHoldingCB( ), eMBRegCoilsCB( ), eMBRegDiscreteCB( ) and 
 *   eMBRegInputCB( ).
 */
typedef enum
{
    MB_REG_READ,                /*!< Read register values and pass to protocol stack. */
    MB_REG_WRITE                /*!< Update register values. */
} MbRegisterMode_t;

#if MB_MASTER_ENABLED > 0

pMbmParseRspHandler MbmSearchCB(uint8_t ucFunctionCode);

/****************************** for parse response *************************/
/****************************** for bits *******************************/                                    
MbReqResult_t MbmParseRspRdCoils(MbReg_t *regs, 
                                    uint16_t ReqRegAddr, uint16_t ReqRegcnt, 
                                    uint8_t *premain,uint16_t remainLength);
MbReqResult_t MbmParseRspWrCoil(MbReg_t *regs, 
                                    uint16_t ReqRegAddr, uint16_t ReqRegcnt,
                                    uint8_t *premain, uint16_t remainLength);
MbReqResult_t MbmParseRspWrMulCoils(MbReg_t *regs, 
                                    uint16_t ReqRegAddr, uint16_t ReqRegcnt,
                                    uint8_t *premain, uint16_t remainLength);
MbReqResult_t MbmParseRspRdDiscreteInputs(MbReg_t *regs, 
                                    uint16_t ReqRegAddr, uint16_t ReqRegcnt, 
                                    uint8_t *premain, uint16_t remainLength);

/****************************** for register *******************************/                                    
MbReqResult_t MbmParseRspRdHoldingRegister(MbReg_t *regs, 
                                                uint16_t ReqRegAddr, uint16_t ReqRegcnt,
                                                uint8_t *premain, uint16_t remainLength);
MbReqResult_t MbmParseRspWrHoldingRegister(MbReg_t *regs, 
                                                uint16_t ReqRegAddr, uint16_t Regcnt,
                                                uint8_t *premain, uint16_t remainLength);
MbReqResult_t MbmParseRspWrMulHoldingRegister(MbReg_t *regs, 
                                                uint16_t ReqRegAddr,uint16_t ReqRegcnt, 
                                                uint8_t *premain, uint16_t remainLength);
MbReqResult_t MbmParseRspRdWrMulHoldingRegister(MbReg_t *regs, 
                                                uint16_t ReqRegAddr,uint16_t ReqRegcnt, 
                                                uint8_t *premain, uint16_t remainLength);
MbReqResult_t MbmParseRdInputRegister(MbReg_t *regs, 
                                                uint16_t ReqRegAddr, uint16_t ReqRegcnt,
                                                uint8_t *premain, uint16_t remainLength);
#endif


#if MB_SLAVE_ENABLED > 0

#if MBS_FUNC_OTHER_REP_SLAVEID_BUF > 0
MbException_t MbsFuncReportSlaveID(MbReg_t *regs, uint8_t *pPdu, uint16_t * usLen);
#endif

#if MBS_FUNC_READ_INPUT_ENABLED > 0
MbException_t MbsFuncRdInputRegister(MbReg_t *regs, uint8_t *pPdu, uint16_t * usLen);
#endif

#if MBS_FUNC_READ_HOLDING_ENABLED > 0
MbException_t MbsFuncRdHoldingRegister(MbReg_t *regs, uint8_t *pPdu, uint16_t * usLen);
#endif

#if MBS_FUNC_WRITE_HOLDING_ENABLED > 0
MbException_t MbsFuncWrHoldingRegister(MbReg_t *regs, uint8_t *pPdu, uint16_t * usLen);
#endif

#if MBS_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
MbException_t MbsFuncWrMulHoldingRegister(MbReg_t *regs, uint8_t *pPdu, uint16_t *usLen);
#endif

#if MBS_FUNC_READ_COILS_ENABLED > 0
MbException_t MbsFuncRdCoils(MbReg_t *regs, uint8_t *pPdu, uint16_t *usLen );
#endif

#if MBS_FUNC_WRITE_COIL_ENABLED > 0
MbException_t MbsFuncWrCoil(MbReg_t *regs, uint8_t *pPdu, uint16_t *usLen);
#endif

#if MBS_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
MbException_t MbsFuncWrMulCoils(MbReg_t *regs, uint8_t *pPdu, uint16_t *usLen);
#endif

#if MBS_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
MbException_t MbsFuncRdDiscreteInputs(MbReg_t *regs, uint8_t *pPdu, uint16_t *usLen);
#endif

#if MBS_FUNC_READWRITE_HOLDING_ENABLED > 0
MbException_t MbsFuncRdWrMulHoldingRegister(MbReg_t *regs, uint8_t *pPdu, uint16_t *usLen);
#endif

pMbsFunctionHandler MbsSearchCB(uint8_t ucFunctionCode);

#endif

#ifdef __cplusplus
}
#endif
#endif

