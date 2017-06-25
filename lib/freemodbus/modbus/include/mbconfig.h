
#ifndef __MB_CONFIG_H
#define __MB_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
/********************* common defined  ************************/
#define MB_MASTER_ENABLED        (0)
#define MB_SLAVE_ENABLED         (1)

/* dynamic memory allocation ENABLE*/
#define MB_DYNAMIC_MEMORY_ALLOC_ENABLED    (0)

#if MB_DYNAMIC_MEMORY_ALLOC_ENABLED > 0 || MB_MASTER_ENABLED > 0
#define mb_malloc pvPortMalloc
#define mb_free vfree
#endif

/*! \defgroup modbus_cfg Modbus Configuration
 *
 * Most modules in the protocol stack are completly optional and can be
 * excluded. This is specially important if target resources are very small
 * and program memory space should be saved.<br>
 *
 * All of these settings are available in the file <code>mbconfig.h</code>
 */
/*! \addtogroup modbus_cfg
 *  @{
 */
/*! \brief If Modbus RTU support is enabled. */
#define MB_RTU_ENABLED                          (  1 )
/*! \brief If Modbus ASCII support is enabled. */
#define MB_ASCII_ENABLED                        (  0 )
/*! \brief If Modbus TCP support is enabled. */
#define MB_TCP_ENABLED                          (  0 )


/*-------------------------------------------------------------------------*/
/*------------------             for slave defined    ---------------------*/
/*-------------------------------------------------------------------------*/
/*! \brief The character timeout value for Modbus ASCII.
 *
 * The character timeout value is not fixed for Modbus ASCII and is therefore
 * a configuration option. It should be set to the maximum expected delay
 * time of the network.
 */
#define MB_ASCII_TIMEOUT_SEC                    (  1 )
/*! \brief Timeout to wait in ASCII prior to enabling transmitter.
 *
 * If defined the function calls vMBPortSerialDelay with the argument
 * MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS to allow for a delay before
 * the serial transmitter is enabled. This is required because some
 * targets are so fast that there is no time between receiving and
 * transmitting the frame. If the master is to slow with enabling its 
 * receiver then he will not receive the response correctly.
 */
#ifndef MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS
#define MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS    ( 0 )
#endif
/*! \brief Maximum number of Modbus functions codes the protocol stack
 *    should support.
 *
 * The maximum number of supported Modbus functions must be greater than
 * the sum of all enabled functions in this file and custom function
 * handlers. If set to small adding more functions will fail.
 */
#define MB_FUNC_HANDLERS_MAX                    ( 16 )
/*! \brief Number of bytes which should be allocated for the <em>Report Slave ID
 *    </em>command.
 *
 * This number limits the maximum size of the additional segment in the
 * report slave id function. See eMBSetSlaveID(  ) for more information on
 * how to set this value. It is only used if MB_FUNC_OTHER_REP_SLAVEID_ENABLED
 * is set to <code>1</code>.
 */
#define MB_FUNC_OTHER_REP_SLAVEID_BUF           ( 32 )

/*! \brief If the <em>Report Slave ID</em> function should be enabled. */
#define MB_FUNC_OTHER_REP_SLAVEID_ENABLED       (  1 )

/*! \brief If the <em>Read Holding Registers</em> function should be enabled. */
#define MB_FUNC_READ_HOLDING_ENABLED            (  1 )
/*! \brief If the <em>Write Single Register</em> function should be enabled. */
#define MB_FUNC_WRITE_HOLDING_ENABLED           (  1 )
/*! \brief If the <em>Write Multiple registers</em> function should be enabled. */
#define MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED  (  1 )
/*! \brief If the <em>Read/Write Multiple Registers</em> function should be enabled. */
#define MB_FUNC_READWRITE_HOLDING_ENABLED       (  1 )

/*! \brief If the <em>Read Input Registers</em> function should be enabled. */
#define MB_FUNC_READ_INPUT_ENABLED              (  1 )
/*! \brief If the <em>Read Coils</em> function should be enabled. */
#define MB_FUNC_READ_COILS_ENABLED              (  1 )
/*! \brief If the <em>Write Coils</em> function should be enabled. */
#define MB_FUNC_WRITE_COIL_ENABLED              (  1 )
/*! \brief If the <em>Write Multiple Coils</em> function should be enabled. */
#define MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED    (  1 )

/*! \brief If the <em>Read Discrete Inputs</em> function should be enabled. */
#define MB_FUNC_READ_DISCRETE_INPUTS_ENABLED    (  1 )

#if MB_DYNAMIC_MEMORY_ALLOC_ENABLED == 0
/*! \brief the number of support multiple slaves  range(1 - 8 )*/
#define MB_SUPPORT_MULTIPLE_NUMBER              ( 1 )
#endif

/*--------------------------------------------------------------------------*/
/*------------------             for master defined    ---------------------*/
/*-------------------------------------------------------------------------*/

/*! \brief Maximum number of Modbus functions codes the protocol stack
 *    should support.
 *
 * The maximum number of supported Modbus functions must be greater than
 * the sum of all enabled functions in this file and custom function
 * handlers. If set to small adding more functions will fail.
 */
#define MB_PARSE_RSP_HANDLERS_MAX               ( 16 )

/*! \brief Number of bytes which should be allocated for the <em>Report Slave ID
 *    </em>command.
 *
 * This number limits the maximum size of the additional segment in the
 * report slave id function. See eMBSetSlaveID(  ) for more information on
 * how to set this value. It is only used if MB_FUNC_OTHER_REP_SLAVEID_ENABLED
 * is set to <code>1</code>.
 */
#define MB_PARSE_RSP_OTHER_REP_SLAVEID_BUF           ( 32 )

/*! \brief If the <em>Report Slave ID</em> function should be enabled. */
#define MB_PARSE_RSP_OTHER_REP_SLAVEID_ENABLED       (  1 )

/*! \brief If the <em>Read Holding Registers</em> function should be enabled. */
#define MB_PARSE_RSP_READ_HOLDING_ENABLED            (  1 )
/*! \brief If the <em>Write Single Register</em> function should be enabled. */
#define MB_PARSE_RSP_WRITE_HOLDING_ENABLED           (  1 )
/*! \brief If the <em>Write Multiple registers</em> function should be enabled. */
#define MB_PARSE_RSP_WRITE_MULTIPLE_HOLDING_ENABLED  (  1 )
/*! \brief If the <em>Read/Write Multiple Registers</em> function should be enabled. */
#define MB_PARSE_RSP_READWRITE_HOLDING_ENABLED       (  1 )

/*! \brief If the <em>Read Input Registers</em> function should be enabled. */
#define MB_PARSE_RSP_READ_INPUT_ENABLED              (  1 )
/*! \brief If the <em>Read Coils</em> function should be enabled. */
#define MB_PARSE_RSP_READ_COILS_ENABLED              (  1 )
/*! \brief If the <em>Write Coils</em> function should be enabled. */
#define MB_PARSE_RSP_WRITE_COIL_ENABLED              (  1 )
/*! \brief If the <em>Write Multiple Coils</em> function should be enabled. */
#define MB_PARSE_RSP_WRITE_MULTIPLE_COILS_ENABLED    (  1 )

/*! \brief If the <em>Read Discrete Inputs</em> function should be enabled. */
#define MB_PARSE_RSP_READ_DISCRETE_INPUTS_ENABLED    (  1 )


/*! @} */
#ifdef __cplusplus
}
#endif
#endif
