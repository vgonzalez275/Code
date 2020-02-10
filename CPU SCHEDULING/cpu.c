//Veronica Gonzalez
//CSC 139, Programming Assignment #2
//CPU scheduling simulation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Q_SIZE 20

typedef struct {
  int current_size;
  int head, tail;
  int q[Q_SIZE];
}q_t;

q_t ready_q, arrival_q, burst_q,running_q;
//int proc_size = ready_q.tail;
/////////////////prototype functions//////////////////////////////////
int getdata(int pid, int arrival, int burst);
void FCFS();
void SRTF();
void RR(int q);
void turntime_response(int wait_time[], int proc_size);

///////////////////////////QUEING FUNCTIONS//////////////////////////////////////
// to zero-fill stack
void Bzero(int *p, int bytes) {
   while(bytes!=0) {
	*p=0;
	bytes--;
   }
}
/////////////////////////////check if full queue//////////////////////
int QisFull(q_t *p) { 
  if((p->tail)==Q_SIZE) {
	return 1;
  }
  else return 0;
}
////////////////////////////check if empty queue////////////////////////
int QisEmpty(q_t *p) { 
  if((p->tail)==0) {
	return 1;
  }
  else	return 0;
}
///////////////////////////////Add to queue///////////////////////////
void EnQ(int to_add, q_t *p) {
   if(QisFull(p)==1) {
      printf("Panic: queue is full, cannot EnQ!\n");
      return;
   }
   p->q[p->tail]=to_add;
   p->tail++;
 
}
////////////////////////////remove from queue///////////////////////////
int DeQ(q_t *p) { 
   int i, value;
   if(QisEmpty(p)) {
      return -1;    //if q[] is empty
   }
   value = p->q[0];
   for(i=0;i<Q_SIZE;i++) {
	if(i < p->tail-1) {
		p->q[i]=p->q[i+1];
	}
	else p->q[i]=-1;
   }
   p->tail--;
   return value;
}
//////////////////////FUNCTIONS/////////////////////////////////////////
int getdata(int pid, int arrival, int burst){
   FILE* file;
   file=fopen("input.1", "r");
   if(file==NULL)
   {
      printf("Error opening input file \n");
      exit(EXIT_FAILURE);
   }
   
   while (!feof(file)){
	fscanf(file,"%d %d %d",&pid, &arrival, &burst);
      //fprintf("%d %d %d\n", pid, arrival, burst);
	EnQ(pid,&ready_q);
	EnQ(arrival,&arrival_q);
	EnQ(burst,&burst_q);
   }
   
   fclose(file);
   return; 
}
/////////////////////////////////////////////////////////////////////
void turntime_response(int wait_time[], int proc_size){
   //number of processes
  int resp_time[proc_size]; //processes 
  int total_time[proc_size];
  int turnaround_time[proc_size];
  int total_wait_t=0;
  int proc;
  int total_turn_t=0;
  int total_resp_t=0;
  float avg_wait_t=0;
  float avg_turn_t=0;
  float avg_resp_t=0;
  //calculate turn arount time
  for(proc=0;proc<proc_size;proc++){
   turnaround_time[proc] = burst_q.q[proc]+wait_time[proc];
   resp_time[proc] = turnaround_time[proc]-wait_time[proc];   
   printf("<system time   %i> process    %i is running\n", proc, proc+1);
  }
  //calculate average wait and turn around time.
  for(proc=0;proc<proc_size;proc++){
     total_wait_t = total_wait_t + wait_time[proc];
     total_turn_t = total_turn_t + turnaround_time[proc];
     //complete_t = turnaround_time[i] + arrival_q.q[i];
     total_resp_t = total_resp_t + resp_time[proc];
   }
  avg_wait_t= (float)total_wait_t / (float)proc_size;
  avg_turn_t= (float) total_turn_t/(float)proc_size;
  avg_resp_t= (float) total_resp_t/(float)proc_size;
  printf("Average waiting time: %.2f \nAverage turnaround time: %.2f \nAverage response time: %.2f \n", avg_wait_t, avg_turn_t, avg_resp_t);

}
void FCFS(){
    //average  waitingtime (end of first run - arrival of second run). Sum of all process waiting time /# of processes.
  int proc_size = ready_q.tail;
  int wait_time[proc_size];
  wait_time[0]=0;
  running_q.q[0]=0;
  int proc;
  //calculate wait time
  for (proc=1; proc< proc_size; proc++){
      running_q.q[proc] = running_q.q[proc-1]+burst_q.q[proc-1];
      wait_time[proc]=running_q.q[proc]-arrival_q.q[proc];
      if (wait_time[proc] <0)
          wait_time[proc]=0;
  }
  
  turntime_response(wait_time,proc_size);
  return;
}
/////////////////////////////////////////////////////////////////////
void SRTF(){
  int proc_size = ready_q.tail;
  int wait_time[proc_size];
  int i=0, j=0; 
  int hold=0; 
  int total_t=0; 
  int currently=0;  
  //find srtf
  for(i=0;i<proc_size;i++){
	currently=i;
	for(j=i+1;j<proc_size;j++){
	  if(burst_q.q[j]<burst_q.q[currently])
            currently=j;
        }
	hold=burst_q.q[i];
        burst_q.q[i]=burst_q.q[currently];
        burst_q.q[currently]=hold;
	
	hold=ready_q.q[i];
        ready_q.q[i]=ready_q.q[currently];
        ready_q.q[currently]=hold;
  }
  //calculate wait time
  for(i=1;i<proc_size;i++){
     wait_time[i]=0;
     for(j=0;j<i;j++)
	wait_time[i]+=burst_q.q[j];
	total_t += wait_time[i];
  } 
  turntime_response(wait_time,proc_size);
  return;
}
////////////////////////////////////////////////////////////////////
void RR(int q){
//q is the time quantum
 int proc_size = ready_q.tail;
 int wait_time[proc_size];
 int new_b[proc_size];
 int i;
 int time=0;

 for(i=0;i<proc_size;i++)
     new_b[i] = burst_q.q[i];
 
 for(i=0; i<proc_size;i++){
    if(new_b[i] > q){
       time+=q;
       new_b[i] -= q;
    }
    else{
       time=time+new_b[i];
       wait_time[i] = time-burst_q.q[i];
       new_b[i]=0;
    }
 }

 turntime_response(wait_time,proc_size);
 return;
}
//////////////////////////MAIN FUNCTION/////////////////////////////
int main(int argc, char *argv[]) {

    int i;
    int  time_q;
    char c1,c2;
    //int proc_size = ready_q.tail;
    int p[ready_q.tail],a[ready_q.tail],b[ready_q.tail];
    
    //zero out queues
    Bzero((int *)&ready_q,sizeof(q_t));
    Bzero((int *)&arrival_q,sizeof(q_t)); 
    Bzero((int *)&burst_q,sizeof(q_t));
    
    if (argc > 1){ 
	sscanf(argv[2], "%c", &c1);
     	sscanf(argv[3], "%c",&c2);
          if (c1 == 'i') {
		getdata(*p,*a,*b);
	  }
     	  if (c2 == 'F') { 
             //getdata(*p,*a,*b);
	     FCFS();
     	  }
      	  if (c2 == 'R'){
		if(argc==5){
		  sscanf(argv[4], "%i",&time_q);
		  RR(time_q);
                }
		else 
		  printf("Enter a positive time quantum"); 
     	  }
 	  if(c2 == 'S'){
		SRTF();
          
          }
     }
    else {
	  printf("Enter Scheduling Algorithm. Only enter time quantum for RR.\n");	
          fprintf(stderr, "usage: proj2 input.1 [FCFS|RR|SRTF] [time quantum]\n");
	  //exit(1);
    }
    exit(0);
}
