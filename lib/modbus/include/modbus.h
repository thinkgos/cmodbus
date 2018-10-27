
#ifndef __MODBUS_H_
#define __MODBUS_H_

#include "mb.h"

#if MB_RTU_ENABLED > 0 ||  MB_ASCII_ENABLED > 0
//public
uint32_t MbRegBufSizeCal(     uint16_t reg_holding_num,
                               uint16_t reg_input_num,
                               uint16_t reg_coils_num,
                               uint16_t reg_discrete_num);

#define MbGetRegsHoldPtr(pReg)       ((uint16_t *)(((MbReg_t *)(pReg))->pReghold))
#define MbGetRegsInputPtr(pReg)      ((uint16_t *)(((MbReg_t *)(pReg))->pReginput))
#define MbGetRegsCoilPtr(pReg)       ((uint8_t *)(((MbReg_t *)(pReg))->pRegCoil))
#define MbGetRegsDiscPtr(pReg)       ((uint8_t *)(((MbReg_t *)(pReg))->pRegDisc))

#define MbGetResgHoldNum(pReg)       (((MbReg_t *)(pReg))->reg_holding_num)
#define MbGetRegsInputNum(pReg)      (((MbReg_t *)(pReg))->reg_input_num)
#define MbGetRegsCoilNum(pReg)       (((MbReg_t *)(pReg))->reg_coils_num)
#define MbGetRegsDiscNum(pReg)       (((MbReg_t *)(pReg))->reg_discrete_num)

#if MB_MASTER_ENABLED

/* TODO implement modbus master */
MbmDev_t *MbmNew(MbMode_t eMode, uint8_t ucPort, uint32_t ulBaudRate, MbParity_t eParity);
void MbmFree(uint8_t ucPort);
MbErrorCode_t MbmSetPara(MbmDev_t *dev, 
                                    uint8_t retry,uint32_t replytimeout,
                                    uint32_t delaypolltime, uint32_t broadcastturntime);
MbmNode_t *MbmNodeNew(uint8_t slaveaddr,
                                uint16_t reg_holding_addr_start,
                                uint16_t reg_holding_num,
                                uint16_t reg_input_addr_start,
                                uint16_t reg_input_num,
                                uint16_t reg_coils_addr_start,
                                uint16_t reg_coils_num,
                                uint16_t reg_discrete_addr_start,
                                uint16_t reg_discrete_num);
void MbmNodeFree(MbmNode_t *node);
void MbmNodeCallBackAssign(MbmNode_t *node, pfnReqResultCB cb, void *arg);
MbErrorCode_t MbmAddNode(MbmDev_t *dev, MbmNode_t *node);
MbErrorCode_t MbmRemoveNode(MbmDev_t *dev, uint8_t slaveaddr);
MbmNode_t *MbmSearchNode(MbmDev_t *dev,uint8_t slaveaddr);
MbErrorCode_t MbmStart(MbmDev_t *dev);
MbErrorCode_t MbmStop(MbmDev_t *dev);
MbErrorCode_t MbmClose(MbmDev_t *dev);
void MbmPoll(void);

/* for bits */
/* for request */
MbReqResult_t MbmReqRdCoils(MbmDev_t *Mdev, uint8_t slaveaddr, 
                                        uint16_t RegStartAddr, uint16_t Coilcnt, uint16_t scanrate);
MbReqResult_t MbmReqWrCoil(MbmDev_t *Mdev, uint8_t slaveaddr, 
                                        uint16_t RegAddr, uint16_t val);
MbReqResult_t MbmReqWrMulCoils(MbmDev_t *Mdev, uint8_t slaveaddr, 
                                        uint16_t RegStartAddr, uint16_t Coilcnt,
                                        uint8_t *valbuf, uint16_t valcnt);
MbReqResult_t MbmReqRdDiscreteInputs(MbmDev_t *Mdev, uint8_t slaveaddr, 
                                        uint16_t RegStartAddr, uint16_t Discnt, uint16_t scanrate);

/* for register */                                    
/* for request */
MbReqResult_t MbmReqRdHoldingRegister(MbmDev_t *Mdev, uint8_t slaveaddr, 
                                                uint16_t RegStartAddr, uint16_t Regcnt, uint16_t scanrate);
MbReqResult_t MbmReqWrHoldingRegister(MbmDev_t *Mdev, uint8_t slaveaddr, 
                                                uint16_t RegAddr, uint16_t val);
MbReqResult_t MbmReqWrMulHoldingRegister(MbmDev_t *Mdev, uint8_t slaveaddr, 
                                                uint16_t RegStartAddr, uint16_t Regcnt,
                                                uint16_t *valbuf, uint16_t valcnt);
MbReqResult_t MbmReqRdInputRegister(MbmDev_t *Mdev, uint8_t slaveaddr, 
                                                uint16_t RegStartAddr, uint16_t Regcnt, uint16_t scanrate);

MbReqResult_t MbmReqRdWrMulHoldingRegister(MbmDev_t *Mdev, uint8_t slaveaddr, 
                                                uint16_t RegReadStartAddr, uint16_t RegReadCnt,
                                                uint16_t RegWriteStartAddr, uint16_t RegWriteCnt,
                                                uint16_t *valbuf, uint16_t valcnt);
#define MbmGetReqSlaveID(pReq) (((MbmReq_t *)(pReq))->slaveaddr)
#define MbmGetReqFunCode(pReq) (((MbmReq_t *)(pReq))->funcode)
#define MbmGetReqErrorCnt(pReq) (((MbmReq_t *)(pReq))->errcnt)
#define MbmGetReqRegAddr(pReq) (((MbmReq_t *)(pReq))->regaddr)
#define MbmGetReqRegCnt(pReq) (((MbmReq_t *)(pReq))->regcnt)
#define MbmGetNodePtr(pReq) (((MbmReq_t *)(pReq))->node)
#define MbmGetArgPtr(pReq)      (((MbmReq_t *)(pReq))->node->arg)

#define MbmGetSlaveID(pNode) (((MbmNode_t *)(pNode))->slaveaddr)
#define MbmGetRegsPtr(pNode)  (&(((MbmNode_t *)(pNode))->regs))

#endif

#if MB_SLAVE_ENABLED > 0

#define MbsGetRegsPtr(pdev)  (&(((MbsDev_t *)(pdev))->regs))
// for slave ,get register start address
#define MbsGetRegsHoldPtr(pdev)      ((uint16_t *)(((MbsDev_t *)(pdev))->regs.pReghold))
#define MbsGetRegsInputPtr(pdev)     ((uint16_t *)(((MbsDev_t *)(pdev))->regs.pReginput))
#define MbsGetRegsCoilPtr(pdev)      ((uint8_t *)(((MbsDev_t *)(pdev))->regs.pRegCoil))
#define MbsGetRegsDiscPtr(pdev)      ((uint8_t *)(((MbsDev_t *)(pdev))->regs.pRegDisc))
// for slave ,get register number
#define MbsGetRegsHoldNum(pdev)      (((MbsDev_t *)(pdev))->regs.reg_holding_num)
#define MbsGetRegsInputNum(pdev)     (((MbsDev_t *)(pdev))->regs.reg_input_num)
#define MbsGetRegsCoilNum(pdev)      (((MbsDev_t *)(pdev))->regs.reg_coils_num)
#define MbsGetRegsDiscNum(pdev)      (((MbsDev_t *)(pdev))->regs.reg_discrete_num)


MbErrorCode_t MbsSetSlaveID(MbReg_t *regs, uint8_t ucSlaveID, uint8_t xIsRunning,
                                        uint8_t const *pucAdditional, uint16_t usAdditionalLen );

MbErrorCode_t MbsRegisterCB(uint8_t ucFunctionCode, pMbsFunctionHandler pxHandler);

MbsDev_t *MbsNew(MbMode_t eMode, uint8_t ucSlaveAddress, uint8_t ucPort, uint32_t ulBaudRate, MbParity_t eParity);
void MbsFree(uint8_t ucPort);
MbErrorCode_t MbsRegAssign(MbsDev_t *dev,
                                uint8_t *regstoragebuf,  
                                uint32_t regstoragesize, 
                                uint16_t reg_holding_addr_start,
                                uint16_t reg_holding_num,
                                uint16_t reg_input_addr_start,
                                uint16_t reg_input_num,
                                uint16_t reg_coils_addr_start,
                                uint16_t reg_coils_num,
                                uint16_t reg_discrete_addr_start,
                                uint16_t reg_discrete_num);
MbErrorCode_t MbsRegAssignSingle(MbsDev_t *dev,
                                uint16_t *reg_holdingbuf,  
                                uint16_t reg_holding_addr_start,
                                uint16_t reg_holding_num,
                                uint16_t *reg_inputbuf,  
                                uint16_t reg_input_addr_start,
                                uint16_t reg_input_num,
                                uint8_t *reg_coilsbuf,  
                                uint16_t reg_coils_addr_start,
                                uint16_t reg_coils_num,
                                uint8_t *reg_discretebuf,  
                                uint16_t reg_discrete_addr_start,
                                uint16_t reg_discrete_num);
MbErrorCode_t MbsStart(MbsDev_t *dev);
MbErrorCode_t MbsStop(MbsDev_t *dev);
MbErrorCode_t MbsClose(MbsDev_t *dev);
void MbsPoll(void);

#endif

#endif

#endif

