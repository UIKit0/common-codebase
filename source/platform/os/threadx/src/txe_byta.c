/**************************************************************************/
/*                                                                        */
/*            Copyright (c) 1996-2000 by Express Logic Inc.               */
/*                                                                        */
/*  This software is copyrighted by and is the sole property of Express   */
/*  Logic, Inc.  All rights, title, ownership, or other interests         */
/*  in the software remain the property of Express Logic, Inc.  This      */
/*  software may only be used in accordance with the corresponding        */
/*  license agreement.  Any unauthorized use, duplication, transmission,  */
/*  distribution, or disclosure of this software is expressly forbidden.  */
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */
/*  written consent of Express Logic, Inc.                                */
/*                                                                        */
/*  Express Logic, Inc. reserves the right to modify this software        */
/*  without notice.                                                       */
/*                                                                        */
/*  Express Logic, Inc.                                                   */
/*  11440 West Bernardo Court               info@expresslogic.com         */
/*  Suite 366                               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** ThreadX Component                                                     */
/**                                                                       */
/**   Byte Memory (BYT)                                                   */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define TX_SOURCE_CODE


/* Include necessary system files.  */

#include    "../inc/tx_api.h"
#include    "../h/tx_ini.h"
#include    "../h/tx_thr.h"
#include    "../h/tx_tim.h"
#include    "../h/tx_byt.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _txe_byte_allocate                                  PORTABLE C      */
/*                                                           3.0f         */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Express Logic, Inc.                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in allocate bytes function call.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    pool_ptr                          Pointer to pool control block     */
/*    memory_ptr                        Pointer to place allocated bytes  */
/*                                        pointer                         */
/*    memory_size                       Number of bytes to allocate       */
/*    wait_option                       Suspension option                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    TX_POOL_ERROR                     Invalid memory pool pointer       */
/*    TX_PTR_ERROR                      Invalid destination pointer       */
/*    TX_WAIT_ERROR                     Invalid wait option               */
/*    TX_CALLER_ERROR                   Invalid caller of this function   */
/*    status                            Actual completion status          */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _tx_byte_allocate                 Actual byte allocate function     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-1996     William E. Lamie         Initial Version 3.0           */
/*  11-11-1997     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 3.0b.  */
/*  01-01-1999     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 3.0e.  */
/*  11-01-1999     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 3.0f.  */
/*                                                                        */
/**************************************************************************/
UINT    _txe_byte_allocate(TX_BYTE_POOL *pool_ptr, VOID **memory_ptr,
                                    ULONG memory_size,  ULONG wait_option)
{

REG_1   UINT        status;                 /* Return status           */

    /* First, check for an invalid byte pool pointer.  */
    if ((!pool_ptr) || (pool_ptr -> tx_byte_pool_id != TX_BYTE_POOL_ID))

        /* Byte pool pointer is invalid, return appropriate error code.  */
        return(TX_POOL_ERROR);

    /* Check for an invalid destination for return pointer.  */
    if (!memory_ptr)

        /* Null destination pointer, return appropriate error.  */
        return(TX_PTR_ERROR);

    /* Check for a wait option error.  Only threads are allowed any form of
       suspension.  */
    if ((wait_option) && ((_tx_thread_system_state) || (_tx_thread_current_ptr == &_tx_timer_thread)))

        /* A non-thread is trying to suspend, return appropriate error code.  */
        return(TX_WAIT_ERROR);

    /* Check for invalid caller of this function.  */
    if (((!_tx_thread_current_ptr) && (_tx_thread_system_state != TX_INITIALIZE_IN_PROGRESS)) ||
        ((_tx_thread_current_ptr) && (_tx_thread_system_state)) ||
        (_tx_thread_current_ptr == &_tx_timer_thread))

        /* Invalid caller of this function, return appropriate error code.  */
        return(TX_CALLER_ERROR);

    /* Call actual byte memory allocate function.  */
    status =  _tx_byte_allocate(pool_ptr, memory_ptr, memory_size,  wait_option);

    /* Return actual completion status.  */
    return(status);
}

