//Veronica Gonzalez
//C Programming Refresher

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    
   int DX=120,AX,BX;
   printf("Enter a positive number for AX \n");
   scanf("%i",&AX);
   printf("Enter a positive number for BX \n");
   scanf("%i",&BX);
   int memory[] = {AX, BX};
   int *ptr=&memory[0]; //pointer to contain memory location value of AX
   AX=*ptr;
   printf("\nvalue of AX: %i\n",AX);
   BX=*(ptr+1); //set value for BX at memory location in array using pointer
   printf("\nvalue of BX: %i\n",BX);
   AX=AX-BX;
   printf("\nvalue of DX: %i\n",DX);
   printf("\nvalue of AX after subtracting BX: %i\n",AX);
  //add value of DX to AX until positive
  
   while(AX < 0){
    
      AX = AX + DX;
      printf("\nAdd DX to AX until AX is a positive value.");  
  }    
  *ptr=AX;
  
   printf("\nValue at memory location of AX: %i\n",*ptr);
  
   //print contents of array  
   printf("\nValues at AX and BX memory locations: %i, %i\n",memory[0],memory[1]);
   return EXIT_SUCCESS;
}
