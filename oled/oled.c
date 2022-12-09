//
// Created by dfkdream on 22. 12. 9.
//

#include "oled.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void oled_set(int value){
    pid_t pid = fork();
    int status;
    if(pid>0){
        waitpid(pid, &status, 0);
        // printf("(oled %d) complete\n", stat);
    }
    else if(pid == 0){
        char buf[1024];
        sprintf(buf,"%d", value);
        char* cmd[] = {"oled", buf, NULL};
        execv(cmd[0], cmd);
    }
    else{
        perror("[oled] fork error...\n");
    }
}