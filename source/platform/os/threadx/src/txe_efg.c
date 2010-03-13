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
/**   Event Flags (EVE)                                                   */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define TX_SOURCE_CODE


/* Include necessary system files.  */

#include    "../inc/tx_api.h"
#include    "../h/tx_ini.h"
#include    "../h/tx_thr.h"
#include    "../h/tx_tim.h"
#include    "../h/tx_eve.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _txe_event_flags_get                                PORTABLE C      */
/*                                                           3.0f         */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Express Logic, Inc.                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the event flags get function     */
/*    call.                                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    group_ptr                         Pointer to group control block    */
/*    requested_event_flags             Event flags requested             */
/*    get_option                        Specifies and/or and clear options*/
/*    actual_flags_ptr                  Pointer to place the actual flags */
/*                                        the service retrieved           */
/*    wait_option                       Suspension option                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    TX_GROUP_ERROR                    Invalid event flags group pointer */
/*    TX_PTR_ERROR                      Invalid actual flags pointer      */
/*    TX_WAIT_ERROR                     Invalid wait option               */
/*    TX_OPTION_ERROR                   Invalid get option                */
/*    TX_CALLER_ERROR                   Invalid caller of this function   */
/*    status                            Actual completion status          */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _tx_event_flags_get               Actual event flags get function   */
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
/*  11-01-1999     William E. Lamie         Modified comment(s), and      */
/*                                            removed error checking for  */
/*                                            ISR callers, resulting in   */
/*                                            version 3.0f.               */
/*                                                                        */
/**************************************************************************/
UINT        _txe_event_flags_get(TX_EVENT_FLAGS_GROUP *group_ptr, ULONG requested_flags,
                    UINT get_option, ULONG *actual_flags_ptr, ULONG wait_option)
{

REG_1   UINT        status;                 /* Return status           */


    /* First, check for an invalid event flag group pointer.  */
    if ((!group_ptr) || (group_ptr -> tx_event_flags_id != TX_EVENT_FLAGS_ID))

        /* Event flags group pointer is invalid, return appropriate error code.  */
        return(TX_GROUP_ERROR);

    /* Check for an invalid destination for actual flags.  */
    if (!actual_flags_ptr)

        /* Null destination pointer, return appropriate error.  */
        return(TX_PTR_ERROR);

    /* Check for a wait option error.  Only threads are allowed any form of
       suspension.  */
    if ((wait_option) && ((_tx_thread_system_state) || (_tx_thread_current_ptr == &_tx_timer_thread)))

        /* A non-thread is trying to suspend, return appropriate error code.  */
        return(TX_WAIT_ERROR);

    /* Check for invalid get option.  */
    if (get_option > TX_AND_CLEAR)

        /* Invalid get events option, return appropriate error.  */
        return(TX_OPTION_ERROR);

    /* Check for invalid caller of this function.  */
    if (_tx_thread_current_ptr == &_tx_timer_thread)

        /* Invalid caller of this function, return appropriate error code.  */
        return(TX_CALLER_ERROR);

    /* Call actual event flags get function.  */
    status =  _tx_event_flags_get(group_ptr, requested_flags, get_option,
                                                    actual_flags_ptr, wait_option);

    /* Return actual completion status.  */
    return(status);
}

