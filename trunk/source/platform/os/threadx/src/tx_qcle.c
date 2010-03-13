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
/**   Queue (QUE)                                                         */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define TX_SOURCE_CODE


/* Include necessary system files.  */

#include    "../inc/tx_api.h"
#include    "../h/tx_thr.h"
#include    "../h/tx_tim.h"
#include    "../h/tx_que.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _tx_queue_cleanup                                   PORTABLE C      */
/*                                                           3.0f         */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Express Logic, Inc.                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes queue timeout and thread terminate          */
/*    actions that require the queue data structures to be cleaned        */
/*    up.                                                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    thread_ptr                        Pointer to suspended thread's     */
/*                                        control block                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _tx_timer_deactivate              Deactivate timer routine          */
/*    _tx_thread_resume                 Resume thread service             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _tx_thread_timeout                Thread timeout processing         */
/*    _tx_thread_terminate              Thread terminate processing       */
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
VOID    _tx_queue_cleanup(TX_THREAD  *thread_ptr)
{

TX_INTERRUPT_SAVE_AREA

REG_1   TX_QUEUE *queue_ptr;                /* Working queue pointer  */


    /* Setup pointer to queue control block.  */
    queue_ptr =  (TX_QUEUE *) thread_ptr -> tx_suspend_control_block;

    /* Disable interrupts to remove the suspended thread from the queue.  */
    TX_DISABLE

    /* Determine if the cleanup is still required.  */
    if ((thread_ptr -> tx_suspend_cleanup) && (queue_ptr) &&
        (queue_ptr -> tx_queue_id == TX_QUEUE_ID))
    {

        /* Yes, we still have thread suspension!  */

        /* Clear the suspension cleanup flag.  */
        thread_ptr -> tx_suspend_cleanup =  TX_NULL;

        /* Remove the suspended thread from the list.  */

        /* See if this is the only suspended thread on the list.  */
        if (thread_ptr == thread_ptr -> tx_suspended_next)
        {

            /* Yes, the only suspended thread.  */

            /* Update the head pointer.  */
            queue_ptr -> tx_queue_suspension_list =  TX_NULL;
        }
        else
        {

            /* At least one more thread is on the same suspension list.  */

            /* Update the list head pointer.  */
            queue_ptr -> tx_queue_suspension_list =  thread_ptr -> tx_suspended_next;

            /* Update the links of the adjacent threads.  */
            (thread_ptr -> tx_suspended_next) -> tx_suspended_previous =
                                                    thread_ptr -> tx_suspended_previous;
            (thread_ptr -> tx_suspended_previous) -> tx_suspended_next =
                                                    thread_ptr -> tx_suspended_next;
        }

        /* Decrement the suspension count.  */
        queue_ptr -> tx_queue_suspended_count--;

        /* Now we need to determine if this cleanup is from a terminate or a
           timeout.  */
        if (thread_ptr -> tx_state == TX_QUEUE_SUSP)
        {

            /* Thread still suspended on the queue.  Setup return error status and
               resume the thread.  */

            /* Setup return status.  */
            if (queue_ptr -> tx_queue_enqueued)
                thread_ptr -> tx_suspend_status =  TX_QUEUE_FULL;
            else
                thread_ptr -> tx_suspend_status =  TX_QUEUE_EMPTY;

            /* Temporarily disable preemption.  */
            _tx_thread_preempt_disable++;

            /* Restore interrupts.  */
            TX_RESTORE

            /* Resume the thread!  Check for preemption even though we are executing
               from the system timer thread right now which normally executes at the
               highest priority.  */
            if (_tx_thread_resume(thread_ptr))

                /* Preemption is required, transfer control back to
                   system.  */
                _tx_thread_system_return();
        }
        else
        {

            /* Restore interrupts.  */
            TX_RESTORE
        }

        /* Deactivate the timeout timer if necessary.  */
        if (thread_ptr -> tx_thread_timer.tx_list_head)
        {

            /* Deactivate the thread's timeout timer.  */
            _tx_timer_deactivate(&(thread_ptr -> tx_thread_timer));
        }
        else
        {

            /* Clear the remaining time to ensure timer doesn't get activated.  */
            thread_ptr -> tx_thread_timer.tx_remaining_ticks =  0;
        }

        /* Disable interrupts.  */
        TX_DISABLE
    }

    /* Restore interrupts.  */
    TX_RESTORE
}

