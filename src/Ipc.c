#include "../include/ipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 4096
#define MSG_BUFFER 128
#define MSG_PERM 0600
#define KEY 6165

typedef struct {
    unsigned int index;
    unsigned char data[BUFFER_SIZE];
} buffer_t;

// need to do this so we can create a member variable inside of the message_t struct
// instead of using a pointer
typedef unsigned char message_buffer_t[MSG_BUFFER];
typedef struct {
    long type;
    message_buffer_t data;
} message_t;

static buffer_t data_buffer;

int errno;

/**********************************************************
 * PRIVATE FUNCTION PROTOTYPES
 **********************************************************/

/**
 * Create a 4 KB buffer of random integer values.  This will malloc memory for the data
 * pointer inside of the struct.  Next it will fill the data pointer with BUFFER_SIZE ints
 * @param buffer_t - pointer to the buffer
 * @return void
 */
void _init_buffer(buffer_t* buffer);


/**********************************************************
 * PUBLIC FUNCTION IMPLEMENTATION
 **********************************************************/

void init_ipc() {
    _init_buffer(&data_buffer);
}

void reset_ipc() {
    // reset
    for (int i = 0; i < BUFFER_SIZE; i++) {
        data_buffer.data[i] = 0;
    }
    data_buffer.index = 0;
    // reseed
    _init_buffer(&data_buffer);
}

void init_message_queue() {
    // check integrity of the data
    if (data_buffer.data == NULL) {
        printf("%s\n", "ERROR: NULL data buffer passed to message queue.");
        exit(-1);
    }
    // create the message queue
    int mqid = msgget(KEY, MSG_PERM|IPC_CREAT|IPC_EXCL);

    // check to make sure the message was created
    if (mqid < 0) {
        perror(strerror(errno));
        printf("%s\n", "ERROR: Failed to create msg queue.");
        exit(-1);
    }

    pid_t pid = fork();
    int status;

    message_t message;
    message_t response;

    // child process
    if (pid == 0) {
        // for c blocks -- 4096/128 -- number of messages to send
        for (int c = 0; c < (BUFFER_SIZE/MSG_BUFFER); c++) {
            // get the message
            if (msgrcv(mqid, &response, sizeof(message_buffer_t), 1, 0) != MSG_BUFFER) {
                perror(strerror(errno));
                printf("%s\n", "ERROR: Failed to receive message.");
                exit(-1);
            }
            for (int i = 0; i < MSG_BUFFER; i++) {
                // we need to calcuate the current index for the buffer relative
                // to the current block and the index of that block
                // then we need to validate that and make sure with have a byte by byte match
                if (data_buffer.data[(c * MSG_BUFFER) + i] != response.data[i]) {
                    printf("%s\n", "ERROR: Mismatch data received.");
                    printf("%s %d %s %d %s %d \n", "Index", (c * MSG_BUFFER) + i, ":", 
                        data_buffer.data[(c * MSG_BUFFER) + i], "!=", response.data[i]);
                    exit(-1);
                }
            }
        }
        // the data validation passed the byte by byte test for each 128 byte block
        printf("%s\n", "PASSED: The data received by the child patches the main data_buffer");
        exit(0); // exit the child
    // parent process
    } else if (pid > 0) {
        for (int c = 0; c < (BUFFER_SIZE/MSG_BUFFER); c++) {
            // set the message
            message.type = 1;
            for (int i = 0; i < MSG_BUFFER; i++) {
                // [(c * MSG_BUFFER) + i] is the current offset for the current block in relation
                // to the total data stream, otherwise it will pass the first 128 bytes 8 times
                message.data[i] = data_buffer.data[(c * MSG_BUFFER) + i];
                data_buffer.index++;
            }
            // send the message
            if (msgsnd(mqid, &message, sizeof(message_buffer_t), 0) != 0) {
                perror(strerror(errno));
                printf("%s\n", "ERROR: Failed to send message.");
                exit(-1);
            }
        }
    } else {
        perror(strerror(errno));
        printf("%s\n", "ERROR: Failed to fork in init_message_queue");
        exit(-1);
    }

    waitpid(pid, &status, WUNTRACED | WCONTINUED);

    // clear the message queue
    if (msgctl(mqid, IPC_RMID, NULL) < 0) {
        perror(strerror(errno));
        printf("%s\n", "ERROR: Failed to clear msg queue.");
        exit(-1);
    }

}

void init_pipe() {

}

/**********************************************************
 * PRIVATE FUNCTION IMPLEMENTATION
 **********************************************************/

void _init_buffer(buffer_t* buffer) {
    // Go through for i = 0 to i < 4KB and fill the buffer
    buffer->index = 0;
    for (unsigned int i = 0; i < BUFFER_SIZE; i++) {
        buffer->data[i] = rand();
    }
}







