//
// Created by dfkdream on 22. 12. 9.
//

#include "music.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int music(int stat){
    pid_t pid = fork();
    if(pid>0){
        return pid;
    }
    else if(pid == 0){
        char buf[1024];
        sprintf(buf,"%d",stat);
        char* cmd[] = {"tone", buf, NULL};
        execv(cmd[0], cmd);
    }
    else{
        printf("[music] fork error...\n");
    }
}
