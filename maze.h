/* You are pre-supplied with the functions below. Add your own 
   function prototypes to the end of this file. */

/* helper functions to allocate and deallocate dynamic 2D arrays */
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);

/* pre-supplied function to load a maze from a file */
char **load_maze(const char *filename, int &height, int &width);

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width);

/*Question 1 - function to find marker character in maze*/
bool find_marker(char ch, char **m, int height, int width, int &row, int &column);

/*Question 2 - function to determine is a given path though the maze is valid*/
bool valid_solution(const char path[], char **maze, int height, int width); 

/* Question 3  - function which finds a valid solution path through maze*/
const char* find_path(char **m, int height, int width, char start, char end);

/* helper function to recursively try paths, mark maze with solution, returns ptr to
   dynamic char[] with solution string.  returns NULL ptr if no solution found
   char last_step contains move made to get to current square
   int total_steps contains which iteration of recursive function we are */
char* try_path(char **m, int row, int column, char end, char last_step, int total_steps);
