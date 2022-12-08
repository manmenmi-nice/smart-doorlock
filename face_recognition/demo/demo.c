#include <stdio.h>
#include <stdlib.h>
#include "../recognize.h"
#include <wait.h>

void callback(int result){
    printf("[callback] result: %d\n", result);
    recognize_release();
    exit(0);
}

int main(){
    recognize_setOnDoneCallback(callback);
    pid_t pid = recognize_init();
    recognize_start();
    waitpid(pid, NULL, 0);
}