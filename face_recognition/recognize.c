#include "recognize.h"
#include <spawn.h>
#include <sys/wait.h>
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MSG_RECOGNIZE_REQUEST "MSG_REC_REQ"
#define MSG_RECOGNIZE_OK "MSG_REC_OK"
#define MSG_RECOGNIZE_FAIL "MSG_REC_FAIL"

#define QUEUE_SEND_NAME "/queue_recognize_mosi"
#define QUEUE_RECEIVE_NAME "/queue_recognize_miso"

#define BUF_SIZE 100

extern char** environ;

struct mq_attr attr{
    .mq_flags = 0,
    .mq_maxmsg = 10,
    .mq_msgsize = BUF_SIZE,
    .mq_curmsgs = 0
};

mqd_t rec_mq_send;
mqd_t rec_mq_receive;

RECOGNIZE_SET_ON_DONE_CALLBACK cb;

void* msgThread(void* arg){
    char buf[BUF_SIZE];
    int n;
    while(1){
        memset(buf, 0, sizeof(buf));
        n = mq_receive(rec_mq_receive, buf, sizeof(buf), NULL);

        printf("[MessageWorker] Received: %s\n", buf);

        if (strcmp(buf, MSG_RECOGNIZE_OK)){
            (if cb) cb(1);
            continue;
        }

        if (strcmp(buf, MSG_RECOGNIZE_FAIL)){
            (if cb) cb(0);
            continue;
        }
    }
}

int recognize_init(){
    rec_mq_send = mq_open(QUEUE_SEND_NAME, O_CREAT|O_WRONLY, 0644, &attr);
    rec_mq_receive = mq_open(QUEUE_NAME, O_CREAT|O_RDONLY, 0644, &attr);

    if (pthread_create(NULL, NULL, msgThread, NULL)<0){
        perror("Could not create message worker thread\n");
        return 0;
    }

    pid_t pid;
    int status;
    char* argv[] = {"python3", "recognize.py", NULL};

    posix_spawn(&pid, "python3", NULL, NULL, argv, environ);
    if (waitpid(pid, &status, 0) < 0) printf("waitpid error\n");

    printf("Face recognizer (%d) exited: %d", pid, status);
}

int recognize_start(){
    mq_send(rec_mq_send, MSG_RECOGNIZE_REQUEST, strlen(MSG_RECOGNIZE_REQUEST), 0);
}

int recognize_setOnDoneCallback(RECOGNIZE_SET_ON_DONE_CALLBACK callback){
    cb = callback;
}
