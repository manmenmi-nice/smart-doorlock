#include "recognize.h"
#include <stdio.h>
#include <mqueue.h>
#include <pthread.h>
#include <string.h>

#define MSG_RECOGNIZE_REQUEST "MSG_REC_REQ"
#define MSG_RECOGNIZE_OK "MSG_REC_OK"
#define MSG_RECOGNIZE_FAIL "MSG_REC_FAIL"
#define MSG_PHOTO_TAKEN "MSG_PHOTO_TAKEN"

#define QUEUE_SEND_NAME "/queue_recognize_mosi"
#define QUEUE_RECEIVE_NAME "/queue_recognize_miso"

#define BUF_SIZE 100

extern char** environ;

struct mq_attr attr = {
    .mq_flags = 0,
    .mq_maxmsg = 10,
    .mq_msgsize = BUF_SIZE,
    .mq_curmsgs = 0
};

mqd_t rec_mq_send;
mqd_t rec_mq_receive;
pthread_t pth;

RECOGNIZE_SET_ON_DONE_CALLBACK cb;

void* msgThread(void* arg){
    printf("[recognize.msgThread] msgThread started\n");
    char buf[BUF_SIZE];
    while(1){
        memset(buf, 0, sizeof(buf));
        mq_receive(rec_mq_receive, buf, sizeof(buf), NULL);

        if (buf[0])
            printf("[recognize.msgThread] Received: %s\n", buf);

        if (strcmp(buf, MSG_RECOGNIZE_FAIL)==0){
            if (cb) cb(0);
            continue;
        }

        if (strcmp(buf, MSG_RECOGNIZE_OK)==0){
            if (cb) cb(1);
            continue;
        }

        if (strcmp(buf, MSG_PHOTO_TAKEN)==0){
            if (cb) cb(2);
            continue;
        }
    }
}

pid_t recognize_init(){

    rec_mq_send = mq_open(QUEUE_SEND_NAME, O_CREAT|O_WRONLY, 0644, &attr);
    rec_mq_receive = mq_open(QUEUE_RECEIVE_NAME, O_CREAT|O_RDONLY, 0644, &attr);

    pid_t pid;
    char* argv[] = {"python", "recognize.py", NULL};

    if (posix_spawn(&pid, "/usr/bin/python", NULL, NULL, argv, environ)){
        perror("[recognize] Posix_spawn failed");
        return 1;
    }

    printf("[recognize] Python Started: pid %d\n",pid);

    if (pthread_create(&pth, NULL, msgThread, NULL)!=0){
        perror("[recognize] failed to create message worker thread\n");
        return 1;
    }

    return pid;
}

int recognize_start(){
    mq_send(rec_mq_send, MSG_RECOGNIZE_REQUEST, strlen(MSG_RECOGNIZE_REQUEST), 0);
}

int recognize_setOnDoneCallback(RECOGNIZE_SET_ON_DONE_CALLBACK callback){
    cb = callback;
}

void recognize_release(){
    pthread_cancel(pth);
    mq_close(rec_mq_send);
    mq_unlink(QUEUE_SEND_NAME);
    mq_close(rec_mq_receive);
    mq_unlink(QUEUE_RECEIVE_NAME);
}