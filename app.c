#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


//maximum bash bash_texts length is set to 50
//minimum allocation is set to 1000
//max allocation is set to 1000000


pid_t processid;
void stack_increaser(int x) {
    if (x == 0) {
        usleep(3000000);
        char bash_text[50]; 
	    sprintf(bash_text, "sudo insmod kernel-experiment.ko processid=%d", processid);
	    system(bash_text);
	    system("sudo rmmod kernel-experiment.o");
	    system("sudo dmesg --read-clear");
	    puts("");
    }
    else { 
        stack_increaser(x-1);
    }
}

int main(int argc, char **argv) {
    processid = getpid();
	
    char bash_text[50]; 
    sprintf(bash_text, "sudo insmod kernel-experiment.ko processid=%d", processid);
    system(bash_text);
    system("sudo rmmod kernel-experiment.o");
    system("sudo dmesg --read-clear");
    puts("");
	
	//Loop to increase heap usage
    int i;
	i = 1000;
	while(i <= 1000000){
		char *m = malloc(i*sizeof(char));
        //m[i-1] = 'A';
        printf("the state of virtual memory for %d allocations from heap\n", i);
        usleep(3000000);
        char bash_text[50]; 
		sprintf(bash_text, "sudo insmod kernel-experiment.ko processid=%d", processid);
		system(bash_text);
		system("sudo rmmod kernel-experiment.o");
		system("sudo dmesg --read-clear");
		puts("");
        free(m);
		i = i * 10;
	}
	
	//Loop to increase stack usage
	i = 1000;
	while(i <= 1000000){
		printf("the state of virtual memory for %d allocations from function call stack\n", i);
        stack_increaser(i);
		i = i * 10;
	}
    
   
    while(1);
    return 0;
}
