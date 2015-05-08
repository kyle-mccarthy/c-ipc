#include "../include/ipc.h"
#include <stdio.h>

int main(int argc, char** argv) {
    
    // demonstrate the message queue
    printf("%s\n", "----- Message Queue -----");
    printf("%s\n", "initializing ipc....");
    init_ipc();
    printf("%s\n", "starting message queue....");
    init_message_queue();
    printf("%s\n", "reseting ipc....");
    reset_ipc();

    // demonstrate the pipe
    printf("%s\n", "----- Pipe -----");
    printf("%s\n", "initializing ipc....");
    init_ipc();
    printf("%s\n", "starting pipe....");
    init_pipe();
    printf("%s\n", "reseting ipc....");
    reset_ipc();
}