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
/*    _tx_queue_send                                      PORTABLE C      */
/*                                                           3.0f         */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Express Logic, Inc.                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function places a message into the specified queue.  If there  */
/*    is no room in the queue, this function waits according to the       */
/*    option specified.                                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    queue_ptr                         Pointer to queue control block    */
/*    source_ptr                        Pointer to message source         */
/*    wait_option                       Suspension option                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                            Completion status                 */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _tx_timer_activate                Activate timer routine            */
/*    _tx_timer_deactivate              Deactivate timer routine          */
/*    _tx_thread_resume                 Resume thread routine             */
/*    _tx_thread_suspend                Suspend thread routine            */
/*    _tx_thread_system_return          Return to system routine          */
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
/*  07-04-1997     William E. Lamie         Modified message copy logic   */
/*                                            to avoid casting structure  */
/*                                            pointers, which causes      */
/*                                            compiler warnings in some   */
/*                                            development tools,          */
/*                                            resulting in version 3.0a.  */
/*  11-11-1997     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 3.0b.  */
/*  03-01-1998     William E. Lamie         Optimized post RESTORE        */
/*                                            processing, resulting in    */
/*                                            version 3.0d.               */
/*  01-01-1999     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 3.0e.  */
/*  11-01-1999     William E. Lamie         Modified comment(s),          */
/*                                            added logic to track events,*/
/*                                            resulting in version 3.0f.  */
/*                                                                        */
/**************************************************************************/
UINT    _tx_queue_send(TX_QUEUE *queue_ptr, VOID *source_ptr, ULONG wait_option)
{

TX_INTERRUPT_SAVE_AREA

REG_1   UINT        status;                 /* Return status           */
REG_2   TX_THREAD   *thread_ptr;            /* Working thread pointer  */
REG_3   ULONG       *source;                /* Source pointer          */
REG_4   ULONG       *destination;           /* Destination pointer     */


    /* Disable interrupts to place message in the queue.  */
    TX_DISABLE

    /* Log this kernel call.  */
    TX_EL_QUEUE_SEND_INSERT

    /* Determine if there is room in the queue.  */
    if (queue_ptr -> tx_queue_available_storage)
    {

        /* Now determine if there is a thread waiting for a message.  */
        if (!queue_ptr -> tx_queue_suspension_list)
        {

            /* No thread suspended while waiting for a message from
               this queue.  */

            /* Simply place the message in the queue.  */

            /* Reduce the amount of available storage.  */
            queue_ptr -> tx_queue_available_storage--;

            /* Increase the enqueued count.  */
            queue_ptr -> tx_queue_enqueued++;

            /* Setup source and destination pointers.  */
            source =  (ULONG *) source_ptr;
            destination =  (ULONG *) queue_ptr -> tx_queue_write;

            /* Copy the message into the queue.  */
            if (queue_ptr -> tx_queue_message_size == TX_1_ULONG)
            {
                /* Copy single longword message into the queue.  */
                *destination =  *source;
            }
            else if (queue_ptr -> tx_queue_message_size == TX_2_ULONG)
            {

                /* Copy double longword message into the queue.  */
                *destination++ =  *source++;
                *destination =    *source;
            }
            else if (queue_ptr -> tx_queue_message_size == TX_4_ULONG)
            {

                /* Copy a four longword message into the queue.  */
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination =    *source;
            }
            else if (queue_ptr -> tx_queue_message_size == TX_8_ULONG)
            {

                /* Copy an eight longword message into the queue.  */
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination =    *source;
            }
            else
            {

                /* Copy a sixteen longword message into the queue.  */
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination =    *source;
            }

            /* Adjust the write pointer.  */
            queue_ptr -> tx_queue_write =
                    queue_ptr -> tx_queue_write + queue_ptr -> tx_queue_message_size;

            /* Determine if we are at the end.  */
            if (queue_ptr -> tx_queue_write >= queue_ptr -> tx_queue_end)

                /* Yes, wrap around to the beginning.  */
                queue_ptr -> tx_queue_write =  queue_ptr -> tx_queue_start;

            /* Set status to success.  */
            status =  TX_SUCCESS;
        }
        else
        {

            /* Thread suspended waiting for a message.  Remove it and copy this message
               into its storage area.  */
            thread_ptr =  queue_ptr -> tx_queue_suspension_list;

            /* See if this is the only suspended thread on the list.  */
            if (thread_ptr == thread_ptr -> tx_suspended_next)
            {

                /* Yes, the only suspended thread.  */

                /* Update the head pointer.  */
                queue_ptr -> tx_queue_suspension_list =  TX_NULL;
            }
            else
            {

                /* At least one more thread is on the same expiration list.  */

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

            /* Prepare for resumption of the thread.  */

            /* Clear cleanup routine to avoid timeout.  */
            thread_ptr -> tx_suspend_cleanup =  TX_NULL;

            /* Temporarily disable preemption.  */
            _tx_thread_preempt_disable++;

            /* Restore interrupts.  */
            TX_RESTORE

            /* Setup source and destination pointers.  */
            source =  (ULONG *) source_ptr;
            destination =  (ULONG *) thread_ptr -> tx_additional_suspend_info;

            /* Copy the message into the thread's destination.  */
            if (queue_ptr -> tx_queue_message_size == TX_1_ULONG)
            {
                /* Copy single longword message into the thread's destination.  */
                *destination =  *source;
            }
            else if (queue_ptr -> tx_queue_message_size == TX_2_ULONG)
            {

                /* Copy double longword message into the thread's destination.  */
                *destination++ =  *source++;
                *destination =    *source;
            }
            else if (queue_ptr -> tx_queue_message_size == TX_4_ULONG)
            {

                /* Copy a four longword message into the thread's destination.  */
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination =    *source;
            }
            else if (queue_ptr -> tx_queue_message_size == TX_8_ULONG)
            {

                /* Copy an eight longword message into the thread's destination.  */
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination =    *source;
            }
            else
            {

                /* Copy a sixteen longword message into the thread's destination.  */
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination++ =  *source++;
                *destination =    *source;
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

            /* Put return status into the thread control block.  */
            thread_ptr -> tx_suspend_status =  TX_SUCCESS;

            /* Resume thread.  */
            if (_tx_thread_resume(thread_ptr))

                /* Preemption is required, transfer control back to
                   system.  */
                _tx_thread_system_return();

            /* Return successful status.  */
            return(TX_SUCCESS);
        }
    }
    else
    {

        /* Determine if the request specifies suspension.  */
        if (wait_option)
        {

            /* Prepare for suspension of this thread.  */

            /* Pickup thread pointer.  */
            thread_ptr =  _tx_thread_current_ptr;

            /* Setup cleanup routine pointer.  */
            thread_ptr -> tx_suspend_cleanup =  _tx_queue_cleanup;

            /* Setup cleanup information, i.e. this queue control
               block and the source pointer.  */
            thread_ptr -> tx_suspend_control_block =    (VOID_PTR) queue_ptr;
            thread_ptr -> tx_additional_suspend_info =  (VOID_PTR) source_ptr;

            /* Setup suspension list.  */
            if (queue_ptr -> tx_queue_suspension_list)
            {

                /* This list is not NULL, add current thread to the end. */
                thread_ptr -> tx_suspended_next =
                        queue_ptr -> tx_queue_suspension_list;
                thread_ptr -> tx_suspended_previous =
                        (queue_ptr -> tx_queue_suspension_list) -> tx_suspended_previous;
                ((queue_ptr -> tx_queue_suspension_list) -> tx_suspended_previous) -> tx_suspended_next =
                        thread_ptr;
                (queue_ptr -> tx_queue_suspension_list) -> tx_suspended_previous =   thread_ptr;
            }
            else
            {

                /* No other threads are suspended.  Setup the head pointer and
                   just setup this threads pointers to itself.  */
                queue_ptr -> tx_queue_suspension_list =  thread_ptr;
                thread_ptr -> tx_suspended_next =        thread_ptr;
                thread_ptr -> tx_suspended_previous =    thread_ptr;
            }

            /* Increment the suspended thread count.  */
            queue_ptr -> tx_queue_suspended_count++;

            /* Set the state to suspended.  */
            thread_ptr -> tx_state =    TX_QUEUE_SUSP;

            /* Set the suspending flag.  */
            thread_ptr -> tx_suspending =  TX_TRUE;

            /* Temporarily disable preemption.  */
            _tx_thread_preempt_disable++;

            /* Save the timeout value.  */
            thread_ptr -> tx_thread_timer.tx_remaining_ticks =  wait_option;

            /* Restore interrupts.  */
            TX_RESTORE

            /* See if we need to start a timer.  */
            if (wait_option != TX_WAIT_FOREVER)
            {

                /* A timeout is required.  */

                /* Activate the thread timer for timeout.  */
                _tx_timer_activate(&(thread_ptr -> tx_thread_timer));
            }

            /* Call actual thread suspension routine.  */
            _tx_thread_suspend(thread_ptr);

            /* Return the completion status.  */
            return(thread_ptr -> tx_suspend_status);
        }
        else

            /* Immediate return, return error completion.  */
            status =  TX_QUEUE_FULL;
    }

    /* Restore interrupts.  */
    TX_RESTORE

    /* Return completion status.  */
    return(status);
}

