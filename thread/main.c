#include <wiringPi.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t lock_cds;
pthread_mutex_t lock_dist;
pthread_t child_thread[10];

int music(int stat){
	pid_t pid = fork();
	int status;
	if(pid>0){
		return pid;
		//waitpid(pid, &status, 0);
		//printf("(music %d) complete\n", stat);
	}
	else if(pid == 0){
		char buf[1024];
		sprintf(buf,"%d",stat); 
		char* cmd[] = {"tone", buf, NULL};
		execv(cmd[0], cmd);
	}
	else{
		printf("(music) fork error...\n");
	}
}

void door(int stat){
	pid_t pid = fork();
	int status;
	if(pid>0){
		pid_t pid2 = music(stat);
		waitpid(pid, &status, 0);
		waitpid(pid2, &status, 0);
		printf("(door %d) complete\n", stat);
	}
	else if(pid == 0){
		char* cmd[] = {"stepper", "90", stat==0?"0":"1", NULL};
		execv(cmd[0], cmd);
	}
	else{
		printf("(door) fork error...\n");
	}
}

void lock(int stat){
	pid_t pid = fork();
	int status;
	if(pid>0){
		usleep(800000);
		waitpid(pid, &status, 0);
		printf("(lock %d) complete\n", stat);
	}
	else if(pid == 0){
		char* cmd[] = {"servo", stat==0?"90":"0", NULL};
		execv(cmd[0], cmd);
	}
	else{
		printf("(lock) fork error...\n");
	}
}

void oled(int stat){
	pid_t pid = fork();
	int status;
	if(pid>0){
		waitpid(pid, &status, 0);
		printf("(oled %d) complete\n", stat);
	}
	else if(pid == 0){
		char buf[1024];
		sprintf(buf,"%d",stat); 
		char* cmd[] = {"oled", buf, NULL};
		execv(cmd[0], cmd);
	}
	else{
		printf("(oled) fork error...\n");
	}
}

void* ultraSonic(void* argv){
	printf("hello, parent!\n");
}

int main(){
	pthread_mutex_init(&lock_cds, NULL);
	pthread_mutex_init(&lock_dist, NULL);
	int rc;
	rc = pthread_create(&child_thread[0],NULL,ultraSonic,NULL);
	if(rc){
		perror("Thread Creation failed...\n");
		pthread_exit(NULL);
	}
	pthread_join(child_thread[0], NULL);
	printf("goodbye child.\n");

	oled(2);
	lock(1);
	door(0);

	lock(1);
	door(1);
	lock(1);
	lock(0);
	oled(1);


	return 0;
}

