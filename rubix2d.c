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

///*
  //in layer1
  rotate_cube(cube,ncs,CLOCKWISE);
  rotate_cube(cube,top,CLOCKWISE);
  rotate_cube(cube,ncs,!CLOCKWISE);
  rotate_cube(cube,top,CLOCKWISE);
  rotate_cube(cube,ncs,CLOCKWISE);
  rotate_cube(cube,top,CLOCKWISE);
  rotate_cube(cube,ncs,!CLOCKWISE);
//*/

}

//orientation functions to solve_manually function
int nc_side(int cell){  //given and nc cell, get layer 2 side
   switch(cell){
     case 1: return(4); 
     case 3: return(1); 
     case 5: return(3); 
     case 7: return(2); 

     case 0: return(1); 
     case 2: return(4); 
     case 6: return(2); 
     case 8: return(3); 
   }
}

int left_nc_side(int cell){ //given an ncs, get the side to its left
   switch(cell){
     case 1: return(3); 
     case 3: return(4); 
     case 5: return(2); 
     case 7: return(1); 

     case 0: return(4); 
     case 2: return(3); 
     case 6: return(1); 
     case 8: return(2); 
   }
}

int nc_180(int top_cell){ //rotate 180 degree, top or bottom cell
   switch(top_cell){
     case 1: return(7); 
     case 3: return(3); 
     case 5: return(5); 
     case 7: return(1); 

     case 0: return(6); 
     case 2: return(8); 
     case 6: return(0); 
     case 8: return(2); 

   }
}

int side_nc(int side){ //given a layer2 side, get the nc cell 
   switch(side){
     case 1: return(3); 
     case 2: return(7); 
     case 3: return(5); 
     case 4: return(1); 
   }
}

int side_corner(int side){ //given a layer2 side, get the nc cell 
   switch(side){
     case 1: return(0); 
     case 2: return(6); 
     case 3: return(8); 
     case 4: return(2); 
   }
}


//end of orientation functions to solve_manually function

void solve_manually(cube_type cube){

  int i, j;
  int c1, i1, j1, ncs1, lncs1;
  int c2, i2, j2, ncs2, lncs2;
  int c3, i3, j3;

  int s,ns,ps,nns, swtopc;
  int notc1_top, notc1_ncs1;
  
  int top = RUBIX_WHITE;
  int left = RUBIX_RED;
  int front = RUBIX_BLUE;

  int bottom = opposite(top);

  int state = 0;
  

  while(1){    
    switch(state){
      case 0: //SOLVE NON-CORNERS ON LAYER 1
              for(c1=1;c1<8;c1+=2){
                ncs1=nc_side(c1);
                lncs1=left_nc_side(c1);
                i1=c1/3;
                j1=c1%3;

                //NC c1 IN LAYER 1
                //if correct
                if(cube[top][i1][j1]==top && cube[ncs1][0][1]==ncs1){
                  //printf("nc c1 = %i fixed\n",c1);
                  //print_cube2(cube);
                  continue;
                }
 
                //other cases
                //search c1 from other top spots (c2's)
                for(c2=c1; c2<8; c2+=2){
                  ncs2=nc_side(c2);
                  lncs2=left_nc_side(c2);
                  i2=c2/3;
                  j2=c2%3;
                     
                  if((cube[top][i2][j2]==top  &&  cube[ncs2][0][1]==ncs1) ||
                     (cube[top][i2][j2]==ncs1 &&  cube[ncs2][0][1]==top) 
                    )
                    break; 
                }
                if(c2<8){//found in top spots!             
     
                  //printf("c1 = %i found in a top spot c2 = %i\n",c1,c2);
                  //print_cube2(cube);

                  //put c1 down, third layer. let c1 be searched in layer3 or as c3
                  rotate_cube(cube,lncs2,!CLOCKWISE);
                  rotate_cube(cube,opposite(lncs2),CLOCKWISE);
                  rotate_cube(cube,ncs2,CLOCKWISE);
                  rotate_cube(cube,ncs2,CLOCKWISE);
                  rotate_cube(cube,lncs2,CLOCKWISE);  
                  rotate_cube(cube,opposite(lncs2),!CLOCKWISE);

                  //printf("c2 = %i put down to layer 3\n",c2);                  
                  //print_cube2(cube);
                  break; // for for-loop;
                }
                  
                //NC c1 IN LAYER 2
                //search c1 in second layer, i.e c2
                for(s=1; s<5; s++){
                  ns = (s==4?1:s+1);
                  if((cube[s][1][2]==top  && cube[ns][1][0]==ncs1) ||
                     (cube[s][1][2]==ncs1 && cube[ns][1][0]==top) 
                    )
                    break;
                }//for(s=1; s<5; s++){

                if(s<5){//found c1!
                  //printf("found c1 = %i in side s%i ns%i, second layer!\n", c1, s, ns); 
                  //print_cube2(cube);

                  swtopc = cube[s][1][2]==top?ns:s;
                  c2=side_nc(swtopc);
                  i2=c2/3;
                  j2=c2%3;
                  ps = (s==1?4:s-1);
                  nns = (ns==4?1:ns+1);
                  notc1_top=cube[top][i1][j1];
                  notc1_ncs1=cube[ncs1][0][1];
                  while(!((cube[swtopc][0][1]== notc1_ncs1) &&
                         (cube[top][i2][j2] == notc1_top)))
                    rotate_cube(cube, top, CLOCKWISE);  

                  //printf("position piece not c1 = %i: (%i,%i) in side swtopc%i s%i ns%i, second layer!\n",c1,notc1_top, notc1_ncs1, swtopc, s, ns);   
                  //print_cube2(cube);
        
                  if(swtopc==s){
                    rotate_cube(cube, top, CLOCKWISE);  
                    rotate_cube(cube, ps, CLOCKWISE);  
                    rotate_cube(cube, top, !CLOCKWISE);  
                    rotate_cube(cube, opposite(top), CLOCKWISE);  
                    rotate_cube(cube, s, !CLOCKWISE);  
                  }
                  else{
                    rotate_cube(cube, top, !CLOCKWISE);  
                    rotate_cube(cube, nns, !CLOCKWISE);  
                    rotate_cube(cube, top, CLOCKWISE);  
                    rotate_cube(cube, opposite(top), !CLOCKWISE);  
                    rotate_cube(cube, ns, CLOCKWISE);  
                  }

                  //printf("fixed top only of c1 = %i second layer!\n", c1);   
                  //print_cube2(cube);

                  //rotate top to its position
                  while(!(cube[top][i1][j1]==top && cube[ncs1][0][1]==ncs1))
                    rotate_cube(cube, top, CLOCKWISE);  

                  //printf("fixed top and ncside c1 = %i second layer!\n", c1);   
                  //print_cube2(cube);
                  continue;
                }//if(s<5){//found in layer2!
                 
                //printf("c1 = %i found in layer3\n",c1);
                //print_cube2(cube);

                //NC c1 IN LAYER 3
                //search c1 in third layer
                c3 = nc_180(c1);
                i3 = c3/3;
                j3 = c3%3;
                while(!((cube[opposite(top)][i3][j3]==top && cube[ncs1][2][1]==ncs1) ||
                        (cube[opposite(top)][i3][j3]==ncs1 && cube[ncs1][2][1]==top)))
                  rotate_cube(cube, opposite(top), CLOCKWISE);
                if(cube[opposite(top)][i3][j3]==top && cube[ncs1][2][1]==ncs1){
                  rotate_cube(cube,lncs1,!CLOCKWISE);
                  rotate_cube(cube,opposite(lncs1),CLOCKWISE);
                  rotate_cube(cube,ncs1,CLOCKWISE);
                  rotate_cube(cube,ncs1,CLOCKWISE);
                  rotate_cube(cube,lncs1,CLOCKWISE);  
                  rotate_cube(cube,opposite(lncs1),!CLOCKWISE);                  
                }
                else{
                  rotate_cube(cube,ncs1,!CLOCKWISE);
                  rotate_cube(cube,top,!CLOCKWISE);
                  rotate_cube(cube,opposite(top),CLOCKWISE);
                  rotate_cube(cube,opposite(lncs1),CLOCKWISE);
                  rotate_cube(cube,top,CLOCKWISE);
                  rotate_cube(cube,opposite(top),CLOCKWISE);
                }
                //printf("prevc1 = %i c1 = %i\n",prevc1, c1);
                //print_cube2(cube);
                
              }//for(c1=1;c1<8;c1+=2)
              if(!(c1<8)) state = 1; 
              break; //case 0
  
      case 1: //SOLVE CORNERS ON LAYER 1
              for(c1=0;c1<10;c1+=2){
                printf("checking corner c1 = %i\n",c1);
                if(c1 == 4) continue;

                ncs1=nc_side(c1);
                lncs1=left_nc_side(c1);
                i1=c1/3;
                j1=c1%3;

                //CORNER c1 IN LAYER 1
                //if correct
                if(cube[top][i1][j1]==top && cube[ncs1][0][0]==ncs1 && cube[lncs1][0][2]==lncs1){
                  printf("corner c1 = %i fixed\n",c1);
                  print_cube2(cube);
                  continue;
                }

                //if found in top               
                for(c2=c1;c2<10;c2+=2){
                  if(c2 == 4) continue;

                  ncs2=nc_side(c2);
                  lncs2=left_nc_side(c2);
                  i2=c2/3;
                  j2=c2%3;

                  if((cube[top][i2][j2]==top   && cube[ncs2][0][0]==ncs1  && cube[lncs2][0][2]==lncs1) ||
                     (cube[top][i2][j2]==top   && cube[ncs2][0][0]==lncs1 && cube[lncs2][0][2]==ncs1 ) ||
                     (cube[top][i2][j2]==ncs1  && cube[ncs2][0][0]==top   && cube[lncs2][0][2]==lncs1) ||
                     (cube[top][i2][j2]==ncs1  && cube[ncs2][0][0]==lncs1 && cube[lncs2][0][2]==top  ) ||
                     (cube[top][i2][j2]==lncs1 && cube[ncs2][0][0]==top   && cube[lncs2][0][2]==ncs1 ) ||
                     (cube[top][i2][j2]==lncs1 && cube[ncs2][0][0]==ncs1  && cube[lncs2][0][2]==top  )
                    )break;
                }
                if(c2<10){//found corner c1 in layer 1
                  printf("corner c1 = %i found in a top spot c2 = %i\n",c1,c2);
                  print_cube2(cube);

                  //put corner in layer 3
                  rotate_cube(cube, ncs2, !CLOCKWISE);
                  rotate_cube(cube, opposite(top), !CLOCKWISE);                  
                  rotate_cube(cube, ncs2, CLOCKWISE);
                  
                  printf("corner c1 = %i placed in layer 3\n",c1,c2);
                  print_cube2(cube);
                  break;
                }//if(c2<8){//found corner c1 in layer 1

                printf("corner c1 = %i in layer 3\n",c1);
                print_cube2(cube);
             
                c3 = nc_180(c1);
                i3 = c3/3;
                j3 = c3%3;
                
                while(!((cube[opposite(top)][i3][j3]==top   && cube[ncs1][2][0]==ncs1  && cube[lncs1][2][2]==lncs1) ||
                        (cube[opposite(top)][i3][j3]==top   && cube[ncs1][2][0]==lncs1 && cube[lncs1][2][2]==ncs1 ) ||
                        (cube[opposite(top)][i3][j3]==ncs1  && cube[ncs1][2][0]==top   && cube[lncs1][2][2]==lncs1) ||
                        (cube[opposite(top)][i3][j3]==ncs1  && cube[ncs1][2][0]==lncs1 && cube[lncs1][2][2]==top  ) ||
                        (cube[opposite(top)][i3][j3]==lncs1 && cube[ncs1][2][0]==top   && cube[lncs1][2][2]==ncs1 ) ||
                        (cube[opposite(top)][i3][j3]==lncs1 && cube[ncs1][2][0]==ncs1  && cube[lncs1][2][2]==top  )
                       ))rotate_cube(cube, opposite(top), CLOCKWISE);
                
                printf("position corner c1 = %i directly below top position\n",c1);
                print_cube2(cube);
                
                //if top color in bottom
                if(cube[opposite(top)][i3][j3]==top){

                  printf("corner c1 = %i top color = %i in bottom\n",c1,top);
                  print_cube2(cube);

                  rotate_cube(cube, ncs1, !CLOCKWISE);   
                  rotate_cube(cube, opposite(top), !CLOCKWISE);
                  rotate_cube(cube, opposite(top), !CLOCKWISE);
                  rotate_cube(cube, ncs1, CLOCKWISE);
                  
                  printf("place corner c1 = %i to move top color from bottom\n",c1);
                  print_cube2(cube);

                  break;

                }
                
                //if top color in ncs1
                if(cube[ncs1][2][0]==top){
                  printf("corner c1 = %i top color = %i in ncs1\n",c1, top);
                  print_cube2(cube);
                  
                  rotate_cube(cube, ncs1, !CLOCKWISE);   
                  rotate_cube(cube, opposite(top), !CLOCKWISE);
                  rotate_cube(cube, ncs1, CLOCKWISE);
                 
                }
                else{
                  printf("corner c1 = %i top color = %i in ncs1\n",c1,top);
                  print_cube2(cube);

                  rotate_cube(cube, lncs1, CLOCKWISE);   
                  rotate_cube(cube, opposite(top), CLOCKWISE);
                  rotate_cube(cube, lncs1, !CLOCKWISE);
                }

                printf("after fixing corner c1 = %i\n",c1);
                print_cube2(cube);


              }//for(c1=0;c0<8;c1+=2){  
              if(!(c1<10)) state = 2; 
              //state = 2;
              break; //case 1
      case 2: //SOLVE LAYER 2
             
              for(c1=0;c1<10;c1+=2){
                if(c1==4) continue;

                ncs1=nc_side(c1);
                lncs1=left_nc_side(c1);
                i1=c1/3;
                j1=c1%3;

                //NC c1 IN LAYER 1
                //if correct
                if(cube[lncs1][1][2]==lncs1 && cube[ncs1][1][0]==ncs1){
                  printf("layer2 c1 = %i fixed\n",c1);
                  print_cube2(cube);
                  continue;
                }
                //other cases
                //search layer2 c1 from other layer2 spots
                for(c2=c1; c2<10; c2+=2){
                  if(c2==4) continue;

                  ncs2=nc_side(c2);
                  lncs2=left_nc_side(c2);
                  i2=c2/3;
                  j2=c2%3;
                     
                  if((cube[lncs2][1][2]==lncs1 && cube[ncs2][1][0]==ncs1) ||
                     (cube[lncs2][1][2]==ncs1  && cube[ncs2][1][0]==lncs1) 
                    )
                    break; 
                }
                if(c2<10){//found in layer2 c1 in layer2!             
     
                  printf("c1 = %i found in layer2 c2 = %i spot: lncs2 = %i, ncs2 =%i\n",c1,c2,lncs2,ncs2);
                  print_cube2(cube);

                  //put c1 in layer3
                  rotate_cube(cube,ncs2,!CLOCKWISE);
                  rotate_cube(cube,opposite(top),CLOCKWISE);
                  rotate_cube(cube,ncs2,CLOCKWISE);
                  rotate_cube(cube,opposite(top),CLOCKWISE);

                  rotate_cube(cube,lncs2,CLOCKWISE);
                  rotate_cube(cube,opposite(top),!CLOCKWISE);
                  rotate_cube(cube,lncs2,!CLOCKWISE);

                  printf("c2 = %i put down to layer 3\n",c2);                  
                  print_cube2(cube);
                  break; // for for-loop;
                }

                printf("layer2 c1 = %i in layer 3\n",c1);
                print_cube2(cube);
                           
                c3 = nc_180(side_nc(lncs1));
                i3 = c3/3;
                j3 = c3%3;
                

                while(!((cube[lncs1][2][1]==lncs1 &&  cube[opposite(top)][i3][j3]==ncs1) ||
                        (cube[lncs1][2][1]==ncs1  &&  cube[opposite(top)][i3][j3]==lncs1)))
                  rotate_cube(cube,opposite(top),CLOCKWISE);
               
                if (cube[lncs1][2][1]==lncs1){
                  rotate_cube(cube,opposite(top),!CLOCKWISE);
           
                  rotate_cube(cube,ncs1,!CLOCKWISE);
                  rotate_cube(cube,opposite(top),CLOCKWISE);
                  rotate_cube(cube,ncs1,CLOCKWISE);
                  rotate_cube(cube,opposite(top),CLOCKWISE);

                  rotate_cube(cube,lncs1,CLOCKWISE);
                  rotate_cube(cube,opposite(top),!CLOCKWISE);
                  rotate_cube(cube,lncs1,!CLOCKWISE);
                }
                else{ 
                  rotate_cube(cube,opposite(top),CLOCKWISE);
                  rotate_cube(cube,opposite(top),CLOCKWISE);

                  rotate_cube(cube,lncs1,CLOCKWISE);
                  rotate_cube(cube,opposite(top),CLOCKWISE);
                  rotate_cube(cube,lncs1,!CLOCKWISE);

                  rotate_cube(cube,opposite(top),!CLOCKWISE);
                  rotate_cube(cube,ncs1,!CLOCKWISE);
                  rotate_cube(cube,opposite(top),!CLOCKWISE);
                  rotate_cube(cube,ncs1,CLOCKWISE);
                } 

              }//for(c1...
              if(!(c1<10)) 
                state = 3;
              break;

      case 3: //SOLVE CROSS ON LAYER 3
              state = 4;
              break;

      case 4: //SOLVE CROSS SIDES 
              state = 5;
              break;

      case 5: //SOLVE CORNERS ON LAYER 3
              state = 6;
              break;

    }//switch(state)
    if (state == 3) break;
  }//while(1) 
}//solve_manually



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

