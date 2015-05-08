#ifndef _IPC_H_
#define _IPC_H_

/**
 * Call the functions that will initialize the IPC.  All this really does is call a 
 * private function to initialize the data buffer.   
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
 * Create a message queue that will pass a 4KB buffer of data between a child and parent
 * process in blocks of 128 bytes and then perform a data integrity check on the data
 * received by the child.
 * @return void
 */
void init_message_queue();

/**
 * Create a pop and use it to send a 4KB buffer split into equal 128 byte blocks from a
 * parent process to the child process.  Have the child process verify the data.
 * @return void
 */
void init_pipe();

#endif
