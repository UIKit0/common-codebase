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
/*    _txe_event_flags_create                             PORTABLE C      */
/*                                                           3.0f         */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Express Logic, Inc.                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the event flag creation function */
/*    call.                                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    group_ptr                         Pointer to event flags group      */
/*                                        control block                   */
/*    name_ptr                          Pointer to event flags name       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    TX_GROUP_ERROR                    Invalid event flag group pointer  */
/*    TX_CALLER_ERROR                   Invalid calling function          */
/*    status                            Actual completion status          */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _tx_event_flags_create            Actual create function            */
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
UINT    _txe_event_flags_create(TX_EVENT_FLAGS_GROUP *group_ptr, CHAR *name_ptr)
{

REG_1   UINT        status;                 /* Return status           */


    /* First, check for an invalid event flags group pointer.  */
    if ((!group_ptr) || (group_ptr -> tx_event_flags_id == TX_EVENT_FLAGS_ID))

        /* Event flags group pointer is invalid, return appropriate error code.  */
        return(TX_GROUP_ERROR);

    /* Check for invalid caller of this function.  */
    if (((!_tx_thread_current_ptr) && (_tx_thread_system_state != TX_INITIALIZE_IN_PROGRESS)) ||
        ((_tx_thread_current_ptr) && (_tx_thread_system_state)) ||
        (_tx_thread_current_ptr == &_tx_timer_thread))

        /* Invalid caller of this function, return appropriate error code.  */
        return(TX_CALLER_ERROR);

    /* Call actual event flags create function.  */
    status =  _tx_event_flags_create(group_ptr, name_ptr);

    /* Return actual completion status.  */
    return(status);
}

