/****
  This library contains rubix datatypes and functions

  1. datatypes

     side_type - an nxn matrix of int corresponding to side of rubix cube
     cube_type - an array of 6 side_type

  2. essential functions

     void init_cube(cube_type cube);   
     USAGE e.g.
               cube_type mycube;
               init_cube(mycube);

     void rotate_cube(cube_type cube, int side, int direction);
     USAGE e.g.
               rotate_side(mycube, RUBIX_RED, CLOCKWISE);

  ajj--12 September 2016, 7:31PM @Apt.1B 

*****/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define N 3
#define SIDES 6

#define CLOCKWISE 0
#define COUNTER_CLOCKWISE 1

#define ANSI_COLOR_RED     "\x1b[41m"
#define ANSI_COLOR_GREEN   "\x1b[42m"
#define ANSI_COLOR_YELLOW  "\x1b[43m"
#define ANSI_COLOR_BLUE    "\x1b[44m"
#define ANSI_COLOR_MAGENTA "\x1b[45m"
#define ANSI_COLOR_ORANGE  "\x1b[45m"
//#define ANSI_COLOR_ORANGE  "\033[48:2:255:165:0m"
#define ANSI_COLOR_CYAN    "\x1b[46m"
#define ANSI_COLOR_WHITE   "\x1b[47m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define RUBIX_WHITE  0
#define RUBIX_RED    1
#define RUBIX_BLUE   2
#define RUBIX_ORANGE 3
#define RUBIX_GREEN  4
#define RUBIX_YELLOW 5

typedef int side_type[N][N];
typedef side_type cube_type[SIDES];

char *str_side[6]={"WHITE","RED","BLUE","ORANGE","GREEN","YELLOW"}; 
char *str_direction[2]={"CLOCKWISE","COUNTER_CLOCKWISE"};

//Function Prototypes
//rubix2d Essentials
int opposite(int side);
int rubix_color(int side);
char char_color(int side);
void init_side(side_type side, int color);
void init_cube(cube_type cube);
void swap(int *a, int *b);
void rotate_side(side_type cube_side, int direction);
void rotate_cube(cube_type cube, int side, int direction);

//recommended shuffles
void cube2x2gen(cube_type cube, int top, int left, int right);
void cross3gen(cube_type cube, int top, int left, int right);


//recommended print functions
void printf_colored(int side);
void print_cube(cube_type cube);


//other printing functions
void print_side1(side_type side);
void print_cube1(cube_type cube);
void print_side2(side_type side);
void print_cube2(cube_type cube);


//moved to rubix2d.c
//void shuffle_cube(cube_type cube);
//void solve_cube(cube_type cube);


int opposite(int side){

   switch(side){
      case 0: return 5;
      case 1: return 3;
      case 2: return 4;
      case 3: return 1;
      case 4: return 2;
      case 5: return 0;
   }
}

int rubix_color(int side){

   switch(side){
      case 0: return RUBIX_WHITE;
      case 1: return RUBIX_RED;
      case 2: return RUBIX_BLUE;
      case 3: return RUBIX_ORANGE;
      case 4: return RUBIX_GREEN;
      case 5: return RUBIX_YELLOW;
   }
}

char char_color(int side){

   return '*';

   switch(side){
      case 0: return 'W';
      case 1: return 'R';
      case 2: return 'B';
      case 3: return 'O';
      case 4: return 'G';
      case 5: return 'Y';
   }
}


void init_side(side_type side, int color){
   int i,j;

   for(i=0;i<N;i++)
     for(j=0;j<N;j++)
       side[i][j]=color;
       //side[i][j]=i*N+j+1;
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

void print_cube1(cube_type cube){
   
   int i;

   for(i=0;i<SIDES;i++)
     print_side1(cube[i]); 
}


void printf_colored(int side){

   switch(rubix_color(side)){

     case RUBIX_YELLOW: printf(ANSI_COLOR_YELLOW "%2c " ANSI_COLOR_RESET,char_color(side));
                        break;
     case RUBIX_WHITE:  printf(ANSI_COLOR_WHITE  "%2c " ANSI_COLOR_RESET,char_color(side));
                        break;
     case RUBIX_RED:    printf(ANSI_COLOR_RED    "%2c " ANSI_COLOR_RESET,char_color(side));
                        break;
     case RUBIX_BLUE:    printf(ANSI_COLOR_BLUE  "%2c " ANSI_COLOR_RESET,char_color(side));
                        break;
     case RUBIX_ORANGE: printf(ANSI_COLOR_ORANGE "%2c " ANSI_COLOR_RESET,char_color(side));
                        break;
     case RUBIX_GREEN:  printf(ANSI_COLOR_GREEN  "%2c " ANSI_COLOR_RESET,char_color(side));
                        break;
   } 
} 

void print_side2(side_type side){
   int i,j;

   for(i=0;i<N;i++){
     for(j=0;j<N;j++)
       printf("   ");
     for(j=0;j<N;j++)
       //printf("%2c ",char_color[side[i][j]]);
       //printf("%2c ",side[i][j]);

       printf_colored(side[i][j]);

     //if(i==N-1) 
     //  printf("_");
     printf("\n");
   }
}


void print_cube2(cube_type cube){
   
   int i,j,side;

   print_side2(cube[0]); 

   for(i=0;i<N;i++){
     for(side=1;side<SIDES-1;side++)
       for(j=0;j<N;j++)
         //printf("%2c ",char_color[cube[side][i][j]]);
         //printf("%2c ",cube[side][i][j]);  
         printf_colored(cube[side][i][j]); 
     printf("\n");
   }
   print_side2(cube[5]); 
   printf("\n");

}



void swap(int *a, int *b){
   int temp;
  
   temp=*a;*a=*b;*b=temp;
}

void rotate_side(side_type cube_side, int direction){
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
        
        temp_side[ri][rj]=cube_side[i][j];
     }

     memcpy(cube_side,temp_side,sizeof(side_type));	   
}

void rotate_cube(cube_type cube, int side, int direction){
   int i,temp;
 
   rotate_side(cube[side], direction);

   switch(side){

      case 0:
              if(direction==CLOCKWISE){
                 for(i=0;i<N;i++){
                   temp=cube[1][0][i]; 
                   cube[1][0][i]=cube[2][0][i];
                   cube[2][0][i]=cube[3][0][i];
                   cube[3][0][i]=cube[4][0][i];
                   cube[4][0][i]=temp;
                 }
              }
	      else {
                 for(i=0;i<N;i++){
                   temp=cube[1][0][i]; 
                   cube[1][0][i]=cube[4][0][i];
                   cube[4][0][i]=cube[3][0][i];
                   cube[3][0][i]=cube[2][0][i];
                   cube[2][0][i]=temp;
                 }
              }	
                
              break;
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
                   temp=cube[2][i][0]; 
                   cube[2][i][0]=cube[5][i][0];
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

      case 4:
              if(direction==CLOCKWISE){
                 for(i=0;i<N;i++){
                   temp=cube[1][i][0]; 
                   cube[1][i][0]=cube[0][0][N-i-1];
                   cube[0][0][N-i-1]=cube[3][N-i-1][N-1];
                   cube[3][N-i-1][N-1]=cube[5][N-1][i];
                   cube[5][N-1][i]=temp;
                 }
              }
	      else {
                 for(i=0;i<N;i++){
                   temp=cube[1][i][0]; 
                   cube[1][i][0]=cube[5][N-1][i];
                   cube[5][N-1][i]=cube[3][N-i-1][N-1];
                   cube[3][N-i-1][N-1]=cube[0][0][N-i-1];
                   cube[0][0][N-i-1]=temp;
                 }
              }	
                
              break;

      case 5:
              if(direction==CLOCKWISE){
                 for(i=0;i<N;i++){
                   temp=cube[1][N-1][i]; 
                   cube[1][N-1][i]=cube[4][N-1][i];
                   cube[4][N-1][i]=cube[3][N-1][i];
                   cube[3][N-1][i]=cube[2][N-1][i];
                   cube[2][N-1][i]=temp;
                 }
              }
	      else {
                 for(i=0;i<N;i++){
                   temp=cube[1][N-1][i]; 
                   cube[1][N-1][i]=cube[2][N-1][i];
                   cube[2][N-1][i]=cube[3][N-1][i];
                   cube[3][N-1][i]=cube[4][N-1][i];
                   cube[4][N-1][i]=temp;
                 }
              }	
                
              break;

   }
   
}

void print_cube(cube_type cube){
   int i,j;
   side_type temp_side;


   //side 0
   for(i=0;i<N;i++){
     for(j=0;j<N;j++)
       printf("   ");
     for(j=0;j<N;j++)
       printf_colored(cube[0][i][j]);
     printf("\n");
   }

   //side 1 and 2
   for(i=0;i<N;i++){
     for(j=0;j<N;j++)
       printf_colored(cube[1][i][j]); 
     for(j=0;j<N;j++)
       printf_colored(cube[2][i][j]); 
     printf("\n");
   }

   //side 5 and 3
   memcpy(temp_side,cube[3],sizeof(side_type));
   rotate_side(temp_side,CLOCKWISE);
   for(i=0;i<N;i++){
     for(j=0;j<N;j++)
       printf("   ");
     for(j=0;j<N;j++)
       printf_colored(cube[5][i][j]);
     for(j=0;j<N;j++)
       printf_colored(temp_side[i][j]);
     printf("\n");
   } 
   
   //side 4
   memcpy(temp_side,cube[4],sizeof(side_type));
   rotate_side(temp_side,CLOCKWISE);
   rotate_side(temp_side,CLOCKWISE);
   for(i=0;i<N;i++){
     for(j=0;j<N;j++)
       printf("   ");
     for(j=0;j<N;j++)
       printf_colored(temp_side[i][j]);
     printf("\n");
   } 
   printf("\n");
}

void cube2x2gen(cube_type cube, int top, int left, int right){

  if (N!=3) return;

  //layer 1
  rotate_cube(cube,opposite(top),COUNTER_CLOCKWISE);
  rotate_cube(cube,left,CLOCKWISE);
  rotate_cube(cube,top,CLOCKWISE);
  rotate_cube(cube,opposite(right),COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(right),COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(top),COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(right),CLOCKWISE);
  rotate_cube(cube,top,COUNTER_CLOCKWISE);
  rotate_cube(cube,left,CLOCKWISE);
  rotate_cube(cube,opposite(right),CLOCKWISE);
  rotate_cube(cube,left,COUNTER_CLOCKWISE);

  //layer2
  //layer2 move1
  rotate_cube(cube,right,COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);
  rotate_cube(cube,right,CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);
  rotate_cube(cube,left,CLOCKWISE);
  rotate_cube(cube,opposite(top),COUNTER_CLOCKWISE);
  rotate_cube(cube,left,COUNTER_CLOCKWISE);

  //layer2 move2
  rotate_cube(cube,opposite(top),COUNTER_CLOCKWISE);

  rotate_cube(cube,opposite(right),COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);
  rotate_cube(cube,opposite(right),CLOCKWISE);

  rotate_cube(cube,opposite(top),CLOCKWISE);
  rotate_cube(cube,opposite(left),CLOCKWISE);
  rotate_cube(cube,opposite(top),COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(left),COUNTER_CLOCKWISE);

  //layer2 move3
  rotate_cube(cube,opposite(top),CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);

  rotate_cube(cube,opposite(right),CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);
  rotate_cube(cube,opposite(right),COUNTER_CLOCKWISE);

  rotate_cube(cube,opposite(top),COUNTER_CLOCKWISE);
  rotate_cube(cube,left,COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(top),COUNTER_CLOCKWISE);
  rotate_cube(cube,left,CLOCKWISE);

  //layer3 
  //layer3 correct cross
  rotate_cube(cube,opposite(top),CLOCKWISE);

  rotate_cube(cube,left,CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);
  rotate_cube(cube,left,COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);

  rotate_cube(cube,left,CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);
  rotate_cube(cube,left,COUNTER_CLOCKWISE);

  rotate_cube(cube,opposite(top),CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);

  //layer3 corner1
  rotate_cube(cube,opposite(right),CLOCKWISE); 
  rotate_cube(cube,opposite(top),CLOCKWISE); 
  rotate_cube(cube,opposite(right),COUNTER_CLOCKWISE); 
  rotate_cube(cube,opposite(top),COUNTER_CLOCKWISE); 

  rotate_cube(cube,right,COUNTER_CLOCKWISE); 
  rotate_cube(cube,opposite(top),CLOCKWISE);
  rotate_cube(cube,opposite(right),CLOCKWISE); 
  rotate_cube(cube,opposite(top),COUNTER_CLOCKWISE);

  rotate_cube(cube,right,CLOCKWISE); 
  rotate_cube(cube,opposite(right),COUNTER_CLOCKWISE); 


  //layer3 corner2
  rotate_cube(cube,top,CLOCKWISE); 
  rotate_cube(cube,opposite(top),COUNTER_CLOCKWISE); 
  rotate_cube(cube,opposite(right),CLOCKWISE); 

  rotate_cube(cube,top,COUNTER_CLOCKWISE); 
  rotate_cube(cube,opposite(right),COUNTER_CLOCKWISE); 
  rotate_cube(cube,opposite(top),CLOCKWISE); 

  rotate_cube(cube,opposite(right),CLOCKWISE); 
  rotate_cube(cube,top,CLOCKWISE); 
  rotate_cube(cube,opposite(right),COUNTER_CLOCKWISE); 
  rotate_cube(cube,top,COUNTER_CLOCKWISE); 

  //layer3 corner3
  rotate_cube(cube,left,COUNTER_CLOCKWISE); 
  rotate_cube(cube,opposite(left),CLOCKWISE); 
  rotate_cube(cube,opposite(right),COUNTER_CLOCKWISE); 

  rotate_cube(cube,left,CLOCKWISE); 
  rotate_cube(cube,opposite(right),CLOCKWISE); 
  rotate_cube(cube,opposite(left),COUNTER_CLOCKWISE); 

  rotate_cube(cube,opposite(right),COUNTER_CLOCKWISE); 
  rotate_cube(cube,left,COUNTER_CLOCKWISE); 
  rotate_cube(cube,opposite(right),CLOCKWISE); 
  rotate_cube(cube,left,CLOCKWISE); 
  
}





void cross3gen(cube_type cube, int top, int left, int right){

  if (N!=3) return;

  //first I
  rotate_cube(cube,left,COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(left),CLOCKWISE);

  rotate_cube(cube,opposite(right),CLOCKWISE);
  rotate_cube(cube,opposite(right),CLOCKWISE);

  rotate_cube(cube,left,CLOCKWISE);
  rotate_cube(cube,opposite(left),COUNTER_CLOCKWISE);

  rotate_cube(cube,left,CLOCKWISE);
  rotate_cube(cube,opposite(left),COUNTER_CLOCKWISE);

  rotate_cube(cube,right,CLOCKWISE);
  rotate_cube(cube,right,CLOCKWISE);

  rotate_cube(cube,left,COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(left),CLOCKWISE);

  //rotate one side
  rotate_cube(cube,left,COUNTER_CLOCKWISE);

  //rotate two sides 2x
  rotate_cube(cube,right,COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(right),CLOCKWISE);

  rotate_cube(cube,right,COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(right),CLOCKWISE);


  //second I
  rotate_cube(cube,top,COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);

  rotate_cube(cube,right,CLOCKWISE);
  rotate_cube(cube,right,CLOCKWISE);

  rotate_cube(cube,top,CLOCKWISE);
  rotate_cube(cube,opposite(top),COUNTER_CLOCKWISE);

  rotate_cube(cube,top,CLOCKWISE);
  rotate_cube(cube,opposite(top),COUNTER_CLOCKWISE);

  rotate_cube(cube,opposite(right),CLOCKWISE);
  rotate_cube(cube,opposite(right),CLOCKWISE);

  rotate_cube(cube,top,COUNTER_CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);

  //rotate one side (reverse)
  rotate_cube(cube,left,CLOCKWISE);

}

/*
// main moved to rubix2d.c
int main(){

  cube_type cube;

  init_cube(cube);

  print_cube(cube);

  //ROTATION test
  //rotate_cube(cube,0,CLOCKWISE);
  //print_cube(cube);

  //rotate_cube(cube,0,COUNTER_CLOCKWISE);
  //print_cube(cube);
  
  cross3gen(cube,RUBIX_WHITE,RUBIX_RED,RUBIX_BLUE);
  print_cube(cube);

  cross3gen(cube,RUBIX_RED,RUBIX_YELLOW,RUBIX_BLUE);
  print_cube(cube);

  cross3gen(cube,RUBIX_RED,RUBIX_YELLOW,RUBIX_BLUE);
  cross3gen(cube,RUBIX_WHITE,RUBIX_RED,RUBIX_BLUE);
  print_cube(cube);

  cube2x2gen(cube,RUBIX_RED,RUBIX_YELLOW,RUBIX_BLUE);
  print_cube(cube);
  
  return(0);
}


*/
