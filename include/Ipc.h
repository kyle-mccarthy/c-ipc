#ifndef _IPC_H_
#define _IPC_H_

/**
 *
 * @return void
 */
void init_ipc();

/**
 * This function will clear the current data in the data buffer and then call the private
 * _init_buffer function to ressed the buffer as a random 4KB buffer of integers
 * @return void
 */
void reset_ipc();

/**
 *
 * @return void
 */
void init_message_queue();

#endif
