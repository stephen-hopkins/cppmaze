#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include "maze.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/*Question 1 - function to find marker character in maze*/
bool find_marker(char ch, char **m, int height, int width, int &row, int &column)
{
  /*go through 2 dimension array sequentially, looking for ch, returning coordinates of first ch*/

  for (row = 0; row < height; row ++) {
    for (column = 0; column < width; column ++) {
      if (m[row][column] == ch)
	return true; }}

  //if not there
  row = -1;
  column = -1;
  return false;
}

/*Question 2 - function to determine if a given path though the maze is valid*/
bool valid_solution(const char path[], char **m, int height, int width) 
{
  // make 'pathlength' equal to length of string 'path'
  int pathlength;
  for (pathlength = 0; path[pathlength] != '\0'; pathlength ++)
    {}
  
  // find starting position (entrance marker)
  int row = 0, column = 0;
  if (!find_marker ('>', m, height, width, row, column)) {
    cout << "Error finding entrance marker in maze";
    return false;
  }
  
  //loop to move sequentially through maze until end of path string
  for (int step = 0; step < pathlength ; step ++) {

    // move current position to next step
    
    switch (path[step]) {
    case 'W':
      column --;
      break;
    case 'E':
      column ++;
      break;
    case 'N':
      row --;
      break;
    case 'S':
      row ++;
      break;
    default:
      cerr << "string contains invalid character";
      return false;
    }

    // check current position is still in maze
    if (row < 0 || row > height || column < 0 || column > width) {
      return false;
    }
    
    // check current position is in corridor
    switch (m[row][column]) {
    case ' ': 
    case 'X':
      break;
    default:
      return false;
    }}
     
    // now on last position of path string, check on maze exit 'X'
  if (m[row][column] == 'X')
    return true;
  else
    return false;
}  

  

/* Question 3  - function which finds a valid solution path through maze 
   char start and end variables are symbols of start & end in maze not coordinates*/
const char* find_path(char **m, int height, int width, char start, char end)
{

  // will point to solution string
  char *solution_ptr;
 
  // find starting place (row & column passed by ref so will contain start place)
  int row = 0, column = 0;
  if (!find_marker(start, m, height, width, row, column)) {
    cout << "Start marker not in maze.";
    return "no solution";
  }

  /* try E, S, W, N directions checking if they are within maze first
     helper function try_path returns ptr to dynamic string with solution
     or returns NULL if didn't find solution*/
  
  if (column + 1 < width) {
      solution_ptr = try_path(m, row, column + 1, end, 'E', 1);
      if (solution_ptr != NULL) 
	return solution_ptr;
      else
	return "no solution";
  }
  
  if (row + 1 < height) {
    solution_ptr = try_path (m, row + 1, column, end, 'S', 1);
    if (solution_ptr != NULL) 
      return solution_ptr;
    else
      return "no solution";
  }
  
  if (column - 1 > 0) {
    solution_ptr = try_path (m, row, column - 1, end,'W', 1);
    if (solution_ptr != NULL) 
      return solution_ptr;
      else
	return "no solution";
  }

  if (row - 1 > 0) {
    solution_ptr = try_path (m, row - 1, column, end, 'N', 1);
    if (solution_ptr != NULL) 
      return solution_ptr;
    else
      return "no solution";
  }
  
  // shouldn't be able to get here
  return "error";

}

/* helper function to recursively try paths, mark maze with solution, returns ptr to
   dynamic char[] with solution string.  returns NULL ptr if no solution found
   char last_step contains move made to get to current square
   int total_steps contains which iteration of recursive function we are on */
char* try_path(char **m, int row, int column, char end, char last_step, int total_steps)
 {
   char *solution_ptr;

   /*check if at end of maze , and if so create new dynamic array, store last move in last 
     character of array, return ptr to array */

   if (m[row][column] == end) {
     try {
       solution_ptr = new char[total_steps]; }
     catch (bad_alloc) {
       cerr << "Error allocating memory" << endl; }
     solution_ptr[total_steps] = '\0';
     solution_ptr[total_steps-1] = last_step;
     return solution_ptr;
   }

   //as not at end, mark current square in maze as '#', route to solution 
   m[row][column] = '#';

   /*check if surrounding squares are allowable moves (end char or ' ', excludes where 
     function has been before),  call itself on allowable surrounding squares. if ptr returned 
     means found solution and recursive stack frames are collapsing.  add last move to solution 
     array and return pointer to array to parent stack*/

   if ((m[row-1][column] == end) || (m[row-1][column] == ' ')) {
     solution_ptr = try_path (m, row - 1, column, end, 'N', total_steps + 1);
     if (solution_ptr != NULL) {
       solution_ptr[total_steps-1] = last_step;
       return solution_ptr;
     }}
     
   if ((m[row][column+1] == end) || (m[row][column+1] == ' ')) { 
     solution_ptr = try_path (m, row, column + 1, end, 'E', total_steps + 1); 
     if (solution_ptr != NULL) {
       solution_ptr[total_steps-1] = last_step;
       return solution_ptr;
     }}
       
   if ((m[row+1][column] == end) || (m[row+1][column] == ' ' )) {
     solution_ptr = try_path (m, row+1, column, end, 'S', total_steps + 1);
     if (solution_ptr != NULL) {
       solution_ptr[total_steps-1] = last_step;
       return solution_ptr;
     }}
   
   if ((m[row][column-1] == end) || (m[row][column-1] == ' ')) {
     solution_ptr = try_path (m, row, column - 1, end, 'W', total_steps + 1);
     if (solution_ptr != NULL) {
       solution_ptr[total_steps-1] = last_step;
       return solution_ptr;
     }}
     
   /* if we have got here there are no allowable moves from here and not on path to solution
      set place in maze back to ' ' and return NULL ptr*/

   m[row][column] = ' ';
   solution_ptr = NULL;
   return solution_ptr;

 }

     
     


       
     
   
   
		
