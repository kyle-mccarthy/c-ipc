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
#define KEY 6161

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

/**
 *
 *
 */
void _init_message_queue();

/**
 *
 *
 */
void _mq_parent(message_t* message);

/**
 *
 *
 */
void _mq_child(message_t* response);

/**********************************************************
 * FUNCTION IMPLEMENTATION
 **********************************************************/

void init_ipc() {
    _init_buffer(&data_buffer);
    _init_message_queue();
}

void _init_buffer(buffer_t* buffer) {
    // Go through for i = 0 to i < 4KB and fill the buffer
    buffer->index = 0;
    for (unsigned int i = 0; i < BUFFER_SIZE; i++) {
        buffer->data[i] = rand();
    }
}

void _init_message_queue() {
    // create the message queue
    int mqid = msgget(KEY, MSG_PERM|IPC_CREAT|IPC_EXCL);

    // check to make sure the message was created
    if (mqid < 0) {
        perror(strerror(errno));
        printf("%s\n", "ERROR: Failed to create msg queue.");
        exit(1);
    }

    pid_t pid = fork();
    int status;

    message_t message;
    message_t response;

    // child process
    if (pid == 0) {
        for (int c = 0; c < (BUFFER_SIZE/MSG_BUFFER); c++) {
            if (msgrcv(mqid, &response, sizeof(message_buffer_t), 1, 0) != MSG_BUFFER) {
                perror(strerror(errno));
                printf("%s\n", "ERROR: Failed to receive message.");
                exit(1);
            }
        }
        exit(1);
    } else if (pid > 0) {
        for (int c = 0; c < (BUFFER_SIZE/MSG_BUFFER); c++) {
            // set the message
            message.type = 1;
            for (int i = 0; i < MSG_BUFFER; i++) {
                message.data[i] = data_buffer.data[i];
                data_buffer.index++;
            }
            // send the message
            if (msgsnd(mqid, &message, sizeof(message_buffer_t), 0) != 0) {
                perror(strerror(errno));
                printf("%s\n", "ERROR: Failed to send message.");
                exit(1);
            }
        }
    }

    waitpid(pid, &status, WUNTRACED | WCONTINUED);

    // clear the message queue
    if (msgctl(mqid, IPC_RMID, NULL) < 0) {
        perror(strerror(errno));
        printf("%s\n", "ERROR: Failed to clear msg queue.");
        exit(1);
    }
}







