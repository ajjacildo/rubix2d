/***** 
   A rubix2d solver


   ajj--12 September 2016, 7:31PM @Apt.1B

*****/


#include "rubix2d.h"

#define MAX_MOVES 9

void shuffle_random(cube_type cube, int n){

   int i, side, direction;

   printf("Shuffle cube:\n");
   for(i=0;i<n;i++){
     side = rand()%6;
     direction = rand()%2;
     printf("rotate_cube(cube, %s, %s)\n",str_side[side],str_direction[direction]);
     rotate_cube(cube, side, direction);
   } 
   //printf("\n"); 
}

void shuffle_from_file_int(cube_type cube){
   int s, i, j;
   FILE *fp=NULL;

   fp=fopen("test.in","r");
   if(fp==NULL || feof(fp)) return;

   for(s=0;s<SIDES;s++)   
     for(i=0;i<N;i++)   
       for(j=0;j<N;j++)
         fscanf(fp,"%i",&cube[s][i][j]);
   fclose(fp);
}

int color_int(int c){
   switch(c){
     case 'W': return 0;
     case 'R': return 1;
     case 'B': return 2;
     case 'O': return 3;
     case 'G': return 4;
     case 'Y': return 5;

   }
}

void shuffle_from_file_char(cube_type cube){
   int s, i, j, count=0;
   int c;
   FILE *fp=NULL;

   fp=fopen("test.in","r");
   if(fp==NULL || feof(fp)) return;

   while(!feof(fp)){
     c=getc(fp);
     if(feof(fp)) break;
     if(c=='W' || c=='R' || c=='B' || c=='O' || c=='G' || c=='Y'){
       //printf("%c",c);
       s=count/(N*N); 
       i=(count%(N*N))/N;   
       j=count%N;
       printf("%i %i %i\n",s,i,j);
       cube[s][i][j]=color_int(c);
       count++; 
       if(count%N==0) printf("\n");
     }
   }
   fclose(fp);
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

void solve_backtrack(cube_type cube, int n){

   //int max_moves = MAX_MOVES;   
   int max_moves = n;   
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
          revdir = c%2? CLOCKWISE:!CLOCKWISE;
          rotate_cube(cube, c/2, revdir);
          //printf("backtrack move %i: ",move);
          //printf("rotate_cube(cube, %s, %s)\n",str_side[c/2],str_direction[revdir]);
          //print_cube(cube);  
       }
       nopts[move]--;
     }
   }
}

void shuffle_layer1(cube_type cube){
  int lncs = RUBIX_ORANGE;
  int top = RUBIX_WHITE;
  int ncs = RUBIX_GREEN;
/*   
  //opposite non-corner side
  rotate_cube(cube,lncs,!CLOCKWISE);
  rotate_cube(cube,opposite(lncs),CLOCKWISE);

  rotate_cube(cube,ncs,CLOCKWISE);
  rotate_cube(cube,ncs,CLOCKWISE);

  rotate_cube(cube,lncs,CLOCKWISE);  
  rotate_cube(cube,opposite(lncs),!CLOCKWISE);

  rotate_cube(cube,ncs,!CLOCKWISE);
  rotate_cube(cube,top,!CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);
  rotate_cube(cube,opposite(lncs),CLOCKWISE);
  rotate_cube(cube,top,CLOCKWISE);
  rotate_cube(cube,opposite(top),CLOCKWISE);
*/
  //in layer1
  rotate_cube(cube,ncs,CLOCKWISE);
  rotate_cube(cube,top,CLOCKWISE);
  rotate_cube(cube,ncs,!CLOCKWISE);
  rotate_cube(cube,top,CLOCKWISE);
  rotate_cube(cube,ncs,CLOCKWISE);
  rotate_cube(cube,top,CLOCKWISE);
  rotate_cube(cube,ncs,!CLOCKWISE);


}

int nc_side(int cell){
   switch(cell){
     case 1: return(4); 
     case 3: return(1); 
     case 5: return(3); 
     case 7: return(2); 
   }
}

int lnc_side(int cell){
   switch(cell){
     case 1: return(3); 
     case 3: return(4); 
     case 5: return(2); 
     case 7: return(1); 
   }
}

int nc_bottom(int top_cell){
   switch(top_cell){
     case 1: return(7); 
     case 3: return(3); 
     case 5: return(5); 
     case 7: return(1); 
   }
}


void solve_manually(cube_type cube){

  int i, j;
  int c1, i1, j1, ncs1,lncs1;
  int c2, i2, j2, ncs2,lncs2;
  int cb, ib, jb, ncsb,lncsb;
  
  int top = RUBIX_WHITE;
  int left = RUBIX_RED;
  int front = RUBIX_BLUE;

  int state = 0;

  while(1){    
    switch(state){
      case 0: //first layer non-corners
              for(c1=1;c1<8;c1+=2){
                ncs1=nc_side(c1);
                lncs1=lnc_side(c1);
                i1=c1/3;
                j1=c1%3;

                //if correct
                if(cube[top][i1][j1]==top &&  cube[ncs1][0][1]==ncs1) continue;

                //if inverted
                if(cube[top][i1][j1]==ncs1 &&  cube[ncs1][0][1]==top){
                   //put it down
                   rotate_cube(cube,lncs1,!CLOCKWISE);
                   rotate_cube(cube,opposite(lncs1),CLOCKWISE);
                   rotate_cube(cube,ncs1,CLOCKWISE);
                   rotate_cube(cube,ncs1,CLOCKWISE);
                   rotate_cube(cube,lncs1,CLOCKWISE);  
                   rotate_cube(cube,opposite(lncs1),!CLOCKWISE);
         
                   //fix from directly mid down
                   rotate_cube(cube,ncs1,!CLOCKWISE);
                   rotate_cube(cube,top,!CLOCKWISE);
                   rotate_cube(cube,opposite(top),CLOCKWISE);
                   rotate_cube(cube,opposite(lncs1),CLOCKWISE);
                   rotate_cube(cube,top,CLOCKWISE);
                   rotate_cube(cube,opposite(top),CLOCKWISE);
                   continue; 
                 } //if 
                  
                 //other cases
                 //search from other top spots
                 for(c2=c1+2; c2<8; c2+=2){

                    ncs2=nc_side(c2);
                    lncs2=lnc_side(c2);
                    i2=c2/3;
                    j2=c2%3;
                     
		    if((cube[top][i2][j2]==top  &&  cube[ncs2][0][1]==ncs1) ||
                       (cube[top][i2][j2]==ncs1 &&  cube[ncs2][0][1]==top) 
                      )
                      break; 
                 }
                 if(c2<8){//found in top spots!                  

                    cb=nc_bottom(c1);
                    ib=cb/3;
                    jb=cb%3;

                    if(cube[top][i2][j2]==top &&  cube[ncs2][0][1]==ncs1){
                      //put it down
                      rotate_cube(cube,lncs2,!CLOCKWISE);
                      rotate_cube(cube,opposite(lncs2),CLOCKWISE);
                      rotate_cube(cube,ncs2,CLOCKWISE);
                      rotate_cube(cube,ncs2,CLOCKWISE);
                      rotate_cube(cube,lncs2,CLOCKWISE);  
                      rotate_cube(cube,opposite(lncs2),!CLOCKWISE);                  
  
	              //find position
                      while(!(cube[opposite(top)][ib][jb]==top && cube[ncs1][2][1]==ncs1))
                         rotate_cube(cube, opposite(top), CLOCKWISE);

                      //directly below
		      rotate_cube(cube,lncs1,!CLOCKWISE);
                      rotate_cube(cube,opposite(lncs1),CLOCKWISE);
                      rotate_cube(cube,ncs1,CLOCKWISE);
                      rotate_cube(cube,ncs1,CLOCKWISE);
                      rotate_cube(cube,lncs1,CLOCKWISE);  
                      rotate_cube(cube,opposite(lncs1),!CLOCKWISE);                  
                    }
                    else{
                      //put it down
                      rotate_cube(cube,lncs2,!CLOCKWISE);
                      rotate_cube(cube,opposite(lncs2),CLOCKWISE);
                      rotate_cube(cube,ncs2,CLOCKWISE);
                      rotate_cube(cube,ncs2,CLOCKWISE);
                      rotate_cube(cube,lncs2,CLOCKWISE);  
                      rotate_cube(cube,opposite(lncs2),!CLOCKWISE);                  
  
		      //find position
                      while(!(cube[opposite(top)][ib][jb]==ncs1 && cube[ncs1][2][1]==top))
                         rotate_cube(cube, opposite(top), CLOCKWISE);

                      //fix from directly mid down
                      rotate_cube(cube,ncs1,!CLOCKWISE);
                      rotate_cube(cube,top,!CLOCKWISE);
                      rotate_cube(cube,opposite(top),CLOCKWISE);
                      rotate_cube(cube,opposite(lncs1),CLOCKWISE);
                      rotate_cube(cube,top,CLOCKWISE);
                      rotate_cube(cube,opposite(top),CLOCKWISE);
                    }

                    print_cube2(cube);
                    continue;
                 }//if(c2<8){//found in top spots!

                 //search in second layer





              }//for c in case 1
              if( c1==8)
                state = 1; 
              break;   
    }//switch(state)
    break;
  }//while(1) 
}//solve_cubem

int main(){

  cube_type cube;

  //printf("%i %i", !CLOCKWISE, CLOCKWISE);  exit(1);

  init_cube(cube);
  print_cube2(cube);


  shuffle_layer1(cube);
  print_cube2(cube);

  
  solve_manually(cube);
  print_cube2(cube);


  //SHUFFLE test
  //shuffle_from_file_char(cube);
  //print_cube(cube);
  //print_cube1(cube);


  //ROTATION test
  //rotate_cube(cube,0,CLOCKWISE);
  //print_cube(cube);

  //rotate_cube(cube,0,!CLOCKWISE);
  //print_cube(cube);


  //PATTERNS   
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

