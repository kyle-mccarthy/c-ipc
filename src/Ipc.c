#include "../include/ipc.h"
#include "stdio.h"
#include "stdlib.h"

#define BUFFER_SIZE 4096

typedef struct {
    unsigned char* data;
    unsigned int index;
} buffer_t;

static buffer_t data_buffer;

/**********************************************************
 * PRIVATE FUNCTION PROTOTYPES
 **********************************************************/

/**
 * Create a 4 KB buffer of random integer values 
 *
 */
void _init_buffer(buffer_t* buffer);

/**
 *
 *
 */
void _init_message_queue();

/**********************************************************
 * FUNCTION IMPLEMENTATION
 **********************************************************/

void init_ipc() {
    _init_buffer(&data_buffer);
    for (unsigned int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d", data_buffer.data[i]);
    }
}

void _init_buffer(buffer_t* buffer) {
    // set up the buffer
    buffer->data = malloc(BUFFER_SIZE * sizeof(unsigned int));
    // Go through for i = 0 to i < 4KB and fill the buffer
    for (unsigned int i = 0; i < BUFFER_SIZE; i++) {
        buffer->data[i] = rand();
    }
}

void _init_message_queue() {

}