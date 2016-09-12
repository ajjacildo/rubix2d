#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define N 5
#define SIDES 6

#define CLOCKWISE 0
#define COUNTER_CLOCKWISE 1

typedef int side_type[N][N];
typedef side_type cube_type[SIDES];

void init_side(side_type side, int color){
   int i,j;

   for(i=0;i<N;i++)
     for(j=0;j<N;j++)
       //side[i][j]=color;
       side[i][j]=i*N+j+1;
}

void init_cube(cube_type cube){
   
   int i;

   for(i=0;i<SIDES;i++)
      init_side(cube[i],i); 

}

void print_side1(side_type side){
   int i,j;

   for(i=0;i<N;i++){
     for(j=0;j<N;j++)
       printf("%2i ",side[i][j]);
     //if(i==N-1) 
     //  printf("_");
     printf("\n");
   }
}

void print_side(side_type side){
   int i,j;

   for(i=0;i<N;i++){
     for(j=0;j<N;j++)
       printf("   ");
     for(j=0;j<N;j++)
       printf("%2i ",side[i][j]);
     //if(i==N-1) 
     //  printf("_");
     printf("\n");
   }
}

void print_cube1(cube_type cube){
   
   int i;

   for(i=1;i<SIDES;i++)
     print_side(cube[i]); 
}

void print_cube(cube_type cube){
   
   int i,j,side;

   print_side(cube[0]); 

   for(i=0;i<N;i++){
     for(side=1;side<SIDES-1;side++)
       for(j=0;j<N;j++)
         printf("%2i ",cube[side][i][j]); 
     printf("\n");
   }
   print_side(cube[5]); 
   printf("\n");

}

void swap(int *a, int *b){
   int temp;
  
   temp=*a;*a=*b;*b=temp;
}

void rotate_side(cube_type cube, int side, int direction){
   int i,j,ri,rj;
   side_type temp_side;

   //memcpy(temp_side,cube[side],sizeof(side_type));	   
 
   for(i=0;i<N;i++)
     for(j=0;j<N;j++){

        if(direction==CLOCKWISE){
           ri=j; rj=((-1)*i)+N-1;
        }
        else{
           ri=((-1)*j)+N-1; rj=i; 
        } 
        //printf("%i %i -> %i %i \n", i,j, ri, rj);
        
        temp_side[ri][rj]=cube[side][i][j];
     }

   memcpy(cube[side],temp_side,sizeof(side_type));	   
 

}

void rotate_cube(cube_type cube, int side, int direction){
   int i,temp; 
  
   rotate_side(cube, side, direction);

   switch(side){
      case 1:
              if(direction==CLOCKWISE){
                 for(i=0;i<N;i++){
                   temp=cube[2][i][0]; 
                   cube[2][i][0]=cube[0][i][0];
                   cube[0][i][0]=cube[4][N-1-i][N-1];
                   cube[4][N-1-i][N-1]=cube[5][i][0];
                   cube[5][i][0]=temp;
                 }
              }
	      else {
                 for(i=0;i<N;i++){
                   temp=cube[3][i][0]; 
                   cube[3][i][0]=cube[5][i][0];
                   cube[5][i][0]=cube[4][N-1-i][N-1];
                   cube[4][N-1-i][N-1]=cube[0][i][0];
                   cube[0][i][0]=temp;
                 }
              }	
              break;


      case 2:
              if(direction==CLOCKWISE){
                 for(i=0;i<N;i++){
                   temp=cube[3][i][0]; 
                   cube[3][i][0]=cube[0][N-1][i];
                   cube[0][N-1][i]=cube[1][N-1-i][N-1];
                   cube[1][N-1-i][N-1]=cube[5][0][N-1-i];
                   cube[5][0][N-1-i]=temp;
                 }
              }
	      else {
                 for(i=0;i<N;i++){
                   temp=cube[3][i][0]; 
                   cube[3][i][0]=cube[5][0][N-1-i];
                   cube[5][0][N-1-i]=cube[1][N-1-i][N-1];
                   cube[1][N-1-i][N-1]=cube[0][N-1][i];
                   cube[0][N-1][i]=temp;
                 }
              }	
              break;

      case 3:
              if(direction==CLOCKWISE){
                 for(i=0;i<N;i++){
                   temp=cube[4][i][0]; 
                   cube[4][i][0]=cube[0][N-1-i][N-1];
                   cube[0][N-1-i][N-1]=cube[2][N-1-i][N-1];
                   cube[2][N-1-i][N-1]=cube[5][N-1-i][N-1];
                   cube[5][N-1-i][N-1]=temp;
                 }
              }
	      else {
                 for(i=0;i<N;i++){
                   temp=cube[4][i][0]; 
                   cube[4][i][0]=cube[5][N-1-i][N-1];
                   cube[5][N-1-i][N-1]=cube[2][N-1-i][N-1];
                   cube[2][N-1-i][N-1]=cube[0][N-1-i][N-1];
                   cube[0][N-1-i][N-1]=temp;
                 }
              }	
                
              break;

   }
   
}

int main(){

  cube_type cube;

  init_cube(cube);

  print_cube(cube);

  rotate_cube(cube,1,CLOCKWISE);
  print_cube(cube);

  rotate_cube(cube,1,COUNTER_CLOCKWISE);
  print_cube(cube);

  return(0);
}

