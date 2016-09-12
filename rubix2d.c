/***** 
   A rubix2d solver


   ajj--12 September 2016, 7:31PM @Apt.1B

*****/


#include "rubix2d.h"

#define MAX_MOVES 10


void shuffle_cube(cube_type cube){

   int i, n = MAX_MOVES, side, direction;

   for(i=0;i<n;i++){
     side = rand()%6;
     direction = rand()%2;
     printf("rotate_cube(cube, %s, %s)\n",str_side[side],str_direction[direction]);
     rotate_cube(cube, side, direction);
   }  
}

int check_cube(cube_type cube){
   int i,j,s;

   for(s=0;s<SIDES;s++){   
     for(i=0;i<N;i++){   
       for(j=0;j<N;j++)
         if(cube[s][i][j]!=s) 
           break; 
       if(j<N) break;
     }
     if(i<N) break;
   }
   return(s==SIDES);
}

void solve_cube(cube_type cube){

   int max_moves = MAX_MOVES;   
   int start, move,i,j;
   int s,side,direction,revdir;
   int c, prevc;
   int nopts[max_moves+2];
   int option[max_moves+2][12];

   //initialize move
   start=move=0;
   nopts[start] = 1;

   while(nopts[start]){
     if(nopts[move]){    

        //execute chosen candidate in previous iteration
        if(move > 0){
          c = option[move][nopts[move]];
          rotate_cube(cube,c/2,c%2);
          //printf("move %i: ",move);
          //printf("rotate_cube(cube, %s, %s)\n",str_side[c/2],str_direction[c%2]);            
          //print_cube(cube);
        }

        //check if solution found
        if(check_cube(cube)){ //solution found!
           printf("Solution Found!\n");
           for(i=1;i<=move;i++){
             c = option[i][nopts[i]];
             //rotate_cube(cube,c/2,c%2);
             printf("move %i: ",i);
             printf("rotate_cube(cube, %s, %s)\n",str_side[c/2],str_direction[c%2]);            
           }
           print_cube(cube);
           break;
        }
        
        //solution is not yet found
        //initialize next move
        move++;
        nopts[move]=0;

        //check max allowed moves is reached
        if(move > max_moves){
          continue;
        }
        else if(move==1){ //first move;
          for(c=0;c<12;c++){
            nopts[move]++;
            option[move][nopts[move]]=c; 		
          }
        }
        else { //populate candidate for next moves
          for(c=0;c<12;c++){
            prevc=option[move-1][nopts[move-1]];
            if(!(c/2==prevc/2 && c%2!=prevc%2)){
              nopts[move]++;
              option[move][nopts[move]]=c;
            } 		
          }

        }
     } 
     else { //backtrack
       move--;
       if(move > 0){
          c = option[move][nopts[move]];
          revdir = c%2? CLOCKWISE:COUNTER_CLOCKWISE;
          rotate_cube(cube, c/2, revdir);
          //printf("backtrack move %i: ",move);
          //printf("rotate_cube(cube, %s, %s)\n",str_side[c/2],str_direction[revdir]);
          //print_cube(cube);  
       }
       nopts[move]--;
     }
   }
}

int main(){

  cube_type cube;

  init_cube(cube);
  print_cube(cube);

  shuffle_cube(cube);
  print_cube(cube);
  
  solve_cube(cube);
  //print_cube(cube);


  //ROTATION test
  //rotate_cube(cube,0,CLOCKWISE);
  //print_cube(cube);

  //rotate_cube(cube,0,COUNTER_CLOCKWISE);
  //print_cube(cube);
  
  //cross3gen(cube,RUBIX_WHITE,RUBIX_RED,RUBIX_BLUE);
  //print_cube(cube);

  //cross3gen(cube,RUBIX_RED,RUBIX_YELLOW,RUBIX_BLUE);
  //print_cube(cube);

  //cross3gen(cube,RUBIX_RED,RUBIX_YELLOW,RUBIX_BLUE);
  //cross3gen(cube,RUBIX_WHITE,RUBIX_RED,RUBIX_BLUE);
  //print_cube(cube);

  //cube2x2gen(cube,RUBIX_RED,RUBIX_YELLOW,RUBIX_BLUE);
  //print_cube(cube);

  
  return(0);
}

