//A rubix2d solver

#include "rubix2d.h"


void shuffle_cube(cube_type cube){

   int i, n = 10, side, direction;

   for(i=0;i<n;i++){
     side = rand()%6;
     direction = rand()%2;
     printf("rotate_cube(cube, %s, %s)\n",str_side[side],str_direction[direction]);
     rotate_cube(cube, side, direction);
   }  
}

void solve_cube(cube_type cube){

   int max_moves = 10   
   int start, move;
   int nopts[max_moves+2];
   int option[max_moves+2][max_moves+2];




}

int main(){

  cube_type cube;

  init_cube(cube);

  print_cube(cube);

  //ROTATION test
  /*rotate_cube(cube,0,CLOCKWISE);
  print_cube(cube);

  rotate_cube(cube,0,COUNTER_CLOCKWISE);
  print_cube(cube);
  
  cross3gen(cube,RUBIX_WHITE,RUBIX_RED,RUBIX_BLUE);
  print_cube(cube);

  cross3gen(cube,RUBIX_RED,RUBIX_YELLOW,RUBIX_BLUE);
  print_cube(cube);

  cross3gen(cube,RUBIX_RED,RUBIX_YELLOW,RUBIX_BLUE);
  cross3gen(cube,RUBIX_WHITE,RUBIX_RED,RUBIX_BLUE);
  print_cube(cube);

  cube2x2gen(cube,RUBIX_RED,RUBIX_YELLOW,RUBIX_BLUE);
  print_cube(cube);

  */

  shuffle_cube(cube);
  print_cube(cube);
  
  solve_cube(cube);
  print_cube(cube);

  return(0);
}

