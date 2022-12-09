//
// Created by dfkdream on 22. 12. 9.
//

#include "lock.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void lock(int stat){
    pid_t pid = fork();
    int status;
    if(pid>0){
        usleep(800000);
        waitpid(pid, &status, 0);
        // printf("(lock %d) complete\n", stat);
    }
    else if(pid == 0){
        char* cmd[] = {"servo", stat==0?"90":"0", NULL};
        execv(cmd[0], cmd);
    }
    else{
        printf("[lock] fork error...\n");
    }
}