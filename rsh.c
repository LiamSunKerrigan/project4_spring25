#line 0"rsh.c"

#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

typedef struct Message{
char source[50];
char target[50];
char msg[200];}
Message;

extern char **environ;

void terminate(int sig){
printf("Exiting....\n");
fflush(stdout);
exit(0);}

int server;

void* messageListener(void *username){
for(;;){
int client = open(username, O_RDONLY);
Message temp;
if(read(client, &temp, sizeof temp) == sizeof temp){
printf("Incoming message from %s: %s", temp.source, temp.msg);}
close(client);}}

int main(int argc, char **argv){

if(argc != 2){
printf("Usage: ./rsh <username>\n");
exit(1);}
signal(SIGINT, terminate);

pthread_t temp;
pthread_create(&temp, NULL, messageListener, argv[1]);
server = open("serverFIFO", O_WRONLY);

for(;;){
fprintf(stderr, "rsh>");
char line[256];
if(fgets(line, sizeof line, stdin) && strcmp(line, "\n")){
{
char *end = line + strlen(line) - 1;
*end = ' ';
while(*--end == ' '){
end[1] = 0;}}
char *argv2[20], **argEnd = argv2;
for(char *arg = line; *(*argEnd++ = arg); ++arg){
while(*arg != ' '){
++arg;}
*arg = 0;}
*--argEnd = 0;
if(!(strcmp(line,"cp") && strcmp(line,"touch") && strcmp(line,"mkdir") && strcmp(line,"ls") && strcmp(line,"cat") && strcmp(line,"grep") && strcmp(line,"chmod") && strcmp(line,"diff"))){
posix_spawnattr_t attr;
posix_spawnattr_init(&attr);
pid_t pid;
posix_spawnp(&pid, *argv2, NULL, &attr, argv2, environ);
waitpid(pid, (int[1]){}, 0);
posix_spawnattr_destroy(&attr);}
else if(!strcmp(line, "cd")){
if(argEnd - argv2 > 2){
printf("-rsh: cd: too many arguments\n");}
else{
chdir(argv2[1]);}}
else if(!strcmp(line, "exit")){
return 0;}
else if(!strcmp(line, "help")){
printf("The allowed commands are:\n1: cp\n2: touch\n3: mkdir\n4: ls\n5: pwd\n6: cat\n7: grep\n8: chmod\n9: diff\n10: cd\n11: exit\n12: help\n");}













else if(!strcmp(line, "sendmsg")){
if(argEnd - argv2 < 2){
printf("sendmsg: you have to specify target user\n");}
else if(argEnd - argv2 < 3){
printf("sendmsg: you have to enter a message\n");}
else{
Message temp;
strcpy(temp.source, argv[1]);
strcpy(temp.target, argv2[1]);
char *w = temp.msg;
for(char **arg = argv2 + 2; arg != argEnd; ++arg){
strcpy(w, *arg);
w += strlen(w);
*w++ = ' ';}
w[-1] = 0;
write(server, &temp, sizeof temp);}}
else{
printf("NOT ALLOWED!\n");}
continue;}}
close(server);}
