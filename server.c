#line 0"server.c"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

typedef struct Message{
char source[50];
char target[50];
char msg[200];}
Message;

void terminate(int sig){
printf("Exiting....\n");
fflush(stdout);
exit(0);}

int main(){

signal(SIGPIPE, SIG_IGN);
signal(SIGINT, terminate);

int server = open("serverFIFO", O_RDONLY);

for(;;){
Message temp;
if(read(server, &temp, sizeof temp) != sizeof temp){
continue;}
printf("Received a request from %s to send the message %s to %s.\n", temp.source, temp.msg, temp.target);

int target = open(temp.target, O_WRONLY);
if(target == -1){
printf("FRICK");}
write(target, &temp, sizeof temp);
close(target);}}
