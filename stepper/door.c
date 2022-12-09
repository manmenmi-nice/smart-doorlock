//
// Created by dfkdream on 22. 12. 9.
//

#include "door.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void door(int stat){
    pid_t pid = fork();
    int status;
    if(pid>0){
        waitpid(pid, &status, 0);
    }
    else if(pid == 0){
        char* cmd[] = {"stepper", "90", stat==0?"0":"1", NULL};
        execv(cmd[0], cmd);
    }
    else{
        printf("[door] fork error...\n");
    }
}

