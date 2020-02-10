//Veronica Gonzalez
//CSC 139, Disk Simulation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_SIZE 200

char buffer[BUF_SIZE];

int cylinders[BUF_SIZE];
int tracks=0;
//pointers starting point for distance calculations
int *ptr1=&cylinders[0];
int *ptr2=&cylinders[1];

//starting track
int disk_arm_start=50; 	

//Function prototypes
int getCylinders();
void FCFS();

////////////////////////////////GET TRACK NUMBERS//////////////////////////
int getCylinders(){
 	
	int track_value=0;
	int i;
	cylinders[tracks];
	
	//get string of tracks from user input.
 	printf("Enter list of disk tracks/cylinders.");
	//fgets(buffer, BUF_SIZE, stdin);
	scanf("%c", &buffer);
	//seperate string to a list of tracks and convert to integer type.
	char *token = strtok(buffer," ");
        while(token != NULL){
	   printf("Reading track %s\n", token);
	   cylinders[tracks++]= atoi(token);
	   token = strtok(NULL, " ");
	}

	//CHECK track entries for out of bounds.
	//Anything over 100 not accepted.
	for(i=0; i<=tracks; i++){
	  track_value  = *(ptr1 + i);
	  if ( track_value > 100){
		  printf("ERROR: Enter tracks between 0 to 100 \n"); 
		  getCylinders();
          }
	}

   return; 
}

///////////////////////////////FIRST COME FIRST SERVE/////////////////////////
void FCFS(){

	int track_distance=0;
	int i, val1, val2;
	int ptr_value=0;
	//calculate first distance
	ptr_value = *ptr1;
	if(disk_arm_start < ptr_value)
	       track_distance = ptr_value - disk_arm_start;
        else track_distance = disk_arm_start - ptr_value;
	
	//calculate the rest
	for(i=0; i<=tracks; i++){
	   val1 = *(ptr1 + i);
	   val2 = *(ptr2 + i);
	   if(val1 < val2)
               track_distance += val2 - val1;
           else 
               track_distance += val1 - val2;
	}
	printf("Total Distance: %i\n", &track_distance);
	return;
}
/******************************************************************************/

/////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {

    char ch;
    
    if (argc > 1){ 
	sscanf(argv[2], "%c", &ch);
     	//sscanf(argv[3], "%s",&buffer);
	getCylinders();

     	if (ch == 'F') { 
          FCFS();
     	}
      	if (ch == 'S'){
	  SSTF();
     	}
 	if(ch == 'L'){
	  LOOK();
          
        }
    }
    else {
	  printf("Enter a Disk Simulation.\n");	
          fprintf(stderr, "usage: disk [FCFS|SSTF|LOOK] \n");
    }
   
    return EXIT_SUCCESS;
}
