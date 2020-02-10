//Observing Linux Behavior
//Veronica Gonzalez

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //*Added* for part C 
#include <sys/time.h> //*Added* for part D
#include <time.h> //*Added* for part D

#define LB_SIZE 400
char lineBuf[LB_SIZE+1];

FILE* proc_File;
FILE* stat_File;
//Initialization
struct timeval now;

//Main Function
int main(int argc, char *argv[]) {
    char repType[16];
    int interval;    //*added* initialized for part C and D
    int duration;    //*added* initialized 
    char c1, c2;
    char delim[]= " ";
    int init_size = strlen(lineBuf);
    char *ptr = strtok(lineBuf, delim);
    
	
    /* Determine report type */
    //char reportType = STANDARD;	
      
    strcpy(repType, "Standard");
    void c_files(); 
    if (argc > 1){ 
	sscanf(argv[1], "%c%c", &c1, &c2);
    	if (c1 != '-') {
		fprintf(stderr, "usage: observer [-s][-l int dur]\n");
		exit(1);
    	}
   	if (c2 == 's') {
	//////////////////////////////////////PART C///////////////////////////////////	
	
		//reportType = SHORT;
		strcpy(repType, "Short");

		/*************************Disk Requests made*****************************/
    		proc_File = fopen("/proc/diskstats", "r");

		int i=0;
		while(fgets(lineBuf, LB_SIZE+1,proc_File)!= NULL){
	   		i++;
	   		lineBuf[strlen(lineBuf)-1] = '\0';	
			if(i==26|i==30|i==32)
				printf("Disk Requests Made: %s\n", lineBuf);
		}
		fclose(proc_File); 
	
		/////////////////////////////////////////	
		proc_File = fopen("/proc/stat", "r");
		int count =0;
		char delim[]= " ";

		while(fgets(lineBuf, LB_SIZE+1,proc_File)!= NULL){
	   		count++;
	   		lineBuf[strlen(lineBuf)-1] = '\0';
		
			/********AMOUNT OF TIME CPU SPENT IN USER MODE, SYS MODE, AND IDLE**********/
			if(count==1){  //line 1 of /proc/stat file
				int init_size = strlen(lineBuf);
    				char *ptr = strtok(lineBuf, delim);
				ptr=strtok(NULL,delim);
				printf("Time spent in User mode: %s, ",ptr);
 				ptr=strtok(NULL,delim);
				printf("System mode: %s, ",ptr);
				ptr=strtok(NULL,delim);
				printf("Idle mode: %s\n",ptr);

			}
			/**************Number of context switches**************************/
	   		if(count==9) //line 9
	 			printf("Number of context switches: %s\n", lineBuf);
			/********************Time system was last booted******************/
	   		if(count==10) //line10
	 			printf("System last booted: %s\n", lineBuf);
			/**********************Number of processes created by kernel******/
	   		if(count==11) //line11
	 			printf("Number of processes created: %s\n", lineBuf);	 
		}	 
		fclose(stat_File); 
	
	}
	
    	if (c2 == 'l') {
		//reportType = LONG;
		strcpy(repType, "Long");
		
		////////////////////////////////////PART D/////////////////////////////////////

		/***************THE AMOUNT OF MEM CONFIGURED INTO THE COMPUTER****************/
		proc_File = fopen("/proc/meminfo", "r");
	
		int i=0;
		while(fgets(lineBuf, LB_SIZE+1,proc_File)!= NULL){
	   		i++;
	   		lineBuf[strlen(lineBuf)-1] = '\0';	
				//Total memory and available memory
				if(i==1|i==2)
					printf("%s\n", lineBuf);
		}

		fclose(proc_File);

	
		//interval = atoi(argv[2]);
		//duration = atoi(argv[3]);

		/********A LIST OF LOAD AVERAGES(each averaged over the last minute)**********/
		/*
    			Need a param to indicate when the load average should be read from the kernel
    			Need a param to indicate the time intervel over which the load avg should be read
		*/
		//float sampleLoadAvg(){
	
		/*
		while (iteration < duration){
			sleep(interval);
			sampleLoadAvg();
			iteration += interval;
		}*/
	
    	}
    }
    else{	 
	gettimeofday(&now, NULL);
	printf("Status report type %s at %s\n", repType, ctime(&(now.tv_sec)));
	//Get the host filename and print it
	proc_File = fopen("/proc/sys/kernel/hostname", "r");
	fgets(lineBuf, LB_SIZE+1, proc_File);
	printf("Machine hostname: %s\n", lineBuf);
	fclose(proc_File);

	/***************************Read CPU type and model***************************/
	proc_File = fopen("/proc/cpuinfo", "r");
	int count =0;
	while(fgets(lineBuf, LB_SIZE+1,proc_File)!= NULL){
	   count++;
	   lineBuf[strlen(lineBuf)-1] = '\0';
	   if(count==2)
		printf("%s\n\n", lineBuf);
	   if(count==5)
		printf("%s\n\n", lineBuf);
	}
	fclose(proc_File); 
	/***************************READ KERNEL VERSION*******************************/
	proc_File = fopen("/proc/version", "r");
	fgets(lineBuf, LB_SIZE+1, proc_File);
	printf("%s\n\n", lineBuf);
	fclose(proc_File);
	/****************AMOUNT OF TIME SINCE LAST BOOT /\dd:hh:mm:ss/\***************/
	proc_File= fopen("/proc/uptime", "r");
		fgets(lineBuf, LB_SIZE+1, proc_File);
	printf("Time since last boot: %s\n", lineBuf);
	fclose(proc_File);
    }

 

    exit(0);    
}

