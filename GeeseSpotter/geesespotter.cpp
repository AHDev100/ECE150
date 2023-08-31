#include "geesespotter_lib.h"

char *createBoard (std::size_t xdim, std::size_t ydim); 
void hideBoard(char *board, std::size_t xdim, std::size_t ydim);
void cleanBoard(char *board);
void printBoard(char *board, std::size_t xdim, std::size_t ydim);
int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
std::size_t yloc);
bool isGameWon(char *board, std::size_t xdim, std::size_t ydim);
int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
std::size_t yloc);
void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim);

char *createBoard (std::size_t xdim, std::size_t ydim) {
    char *board{new char[xdim*ydim]}; 
    for (int i{0}; i < (xdim*ydim); ++i) {
        board[i] = 0; //Initializing all the elements in the 'board' character array to 0 as required
    }
    return board; 
}

void hideBoard(char *board, std::size_t xdim, std::size_t ydim){
    for (int i{0}; i<(xdim*ydim); ++i){
        board[i] ^= hiddenBit(); //we see all the elements to '*' so that we can now hide them
    }
}

void cleanBoard(char *board){ 
    delete[] board; //use the delete [] operator to deallocate memory 
    board = nullptr; 
}

void printBoard(char *board, std::size_t xdim, std::size_t ydim){
      for (int i{0}; i<ydim; ++i){
        for (int j{0}; j<xdim; ++j) {
            std::size_t index = j + (i*xdim);
            if (((board[index]) & hiddenBit()) == hiddenBit()) { //checks if the value is hidden
                if (((board[index]) & markedBit()) == markedBit()){ //checks if the value is marked
                    std::cout << 'M'; //if hidden and marked, we print out 'M'
                }
                else {
                    std::cout << '*';
                }
            }
            else {
                if (((board[index]) & markedBit()) == markedBit()){
                    std::cout << 'M'; //If not hidden, but marked, we print out 'M'
                }
                else {
                    std::cout << int((board[index] & valueMask()));//We typecast to an int to print out the integer, and not the ASCII value
                }
            }
        }
        std::cout << std::endl; 
    }
}


int mark (char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
std::size_t yloc){
    std::size_t index{}; //Used to find index of the character array in which the point lies
    index = (xloc) + (yloc*xdim);
    if (((board[index])&hiddenBit()) != hiddenBit()) { //If this value is less than 32, then that means bit [5] in the data structure is turned off - the value is revealed
        return 2;
    }
    else {
        //We remove the bit at location [5] and check to see if the bit at [4] is a 1 or a 0 - then based on that, we can toggle the value //This is when bit [4] is a 1
        board[index] = ((board[index])^(markedBit())); //Toggle the marked bit 
        return 0; 
    }
}

bool isGameWon(char *board, std::size_t xdim, std::size_t ydim){
    std::size_t non_geese{0}; //This checks for the number of fields that don't have a goose are present
    std::size_t revealed_non_geese{0}; //This checks how many of those fields are present
    for (int i{0}; i < xdim*ydim; ++i){
        if ((board[i] & valueMask()) != 9){
            if (((board[i]) & hiddenBit()) != hiddenBit()){ //If the field is both revealed and not a goose
                ++non_geese; 
                ++revealed_non_geese; 
            }
            else {
                ++non_geese; //If the field is just not a goose - it is hidden
            }
        }
    }
    if (non_geese == revealed_non_geese){ //If the number of fields that aren't a goose is the same as the number of revealed non-geese, we win
        return true; 
    }
    else {
        return false; 
    }
}

void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim) {
    std::size_t count {0}; 
    for (int loc{0}; loc < xdim*ydim; ++loc){
        if ((board[loc]&valueMask()) == 9){ //We don't want to modify anything if the value of the field is 9 (a goose)
            continue;
        }
        std::size_t xloc = loc%xdim; //Formula to find x location of point on the board
        std::size_t yloc = loc/xdim; //Formula to find y location of point on the board
        for (int x{-1}; x<=1; ++x){ /*We loop from -1 to 1 in both directions, because all the neighbours are one grid spot away from the 0 - this nested loop gives us all such locations*/
            for (int y{-1}; y<=1; ++y){
                if (x==0 && y==0){ //We don't want to do anything here, because this is the original location!
                    continue;
                }
                std::size_t neigh_ind = (xloc + x) + ((yloc+y)*(xdim)); //Formula for the index of the neighbour on the board array
                if ((xloc + x)>=0 && (xloc+x)<=(xdim-1) && (yloc+y)>=0 && (yloc + y)<=(ydim-1)){ //These conditions check if the index is not invalid - (xlocation and ylocation of the point can't be negative or greater than or equal to the xdimension)
                    if ((board[neigh_ind]&valueMask()) == 9){ //If it's a goose, we add 1 to the count variable - which checks for the amount of geese neighbouring the 0
                        count++; 
                    }
                    else {
                        continue; //Otherwise, we don't do anything and keep looping
                    }
                } 
            }
        }
        board[loc] = count; //After the loop, we assign the field the value of count --> the # of neighbouring fields
        count = 0; //We have to reset the value of count before looping through the loc loop --> This is to ensure no field values are skewed
    }
}

int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
std::size_t yloc){
  int index = xloc + yloc*xdim; //Formula to find where the point with coordinates (xloc, yloc) lies in the board array
  if ((board[index])&markedBit()){ //If the value is marked
    return 1; 
  }  
  else if (!(board[index]&hiddenBit())) { //If the value is revealed
    return 2; 
  }
  else if ((board[index]&valueMask()) == 9) { //If the field contains a goose (value of 9)
    if (board[index] & hiddenBit()){ //If the goose is hidden, we want to reveal it, so we toggle the hiddenBit()
        board[index]^=hiddenBit(); 
    }
    return 9; 
  }
  else {
    if (board[index] & hiddenBit()){ //If the value is hidden, we want to toggle it - so that is now revealed
        board[index] ^= hiddenBit(); 
    }
    if ((board[index]&valueMask()) == 0){ //If the value is 0, we have to reveal all the neighbours
        for (int x{-1}; x<=1; ++x){ //Same algorithm as computeneighbours for finding the neighbouring values
            for (int y{-1}; y<=1; ++y){
                    if ((x==0)&&(y==0)){ //Again, this gives us the initial location - we don't modify anything as it has already been modified
                        continue;
                    }
                    int neigh_index = (xloc+x) + (yloc+y)*(xdim); //We apply the formula for translating from point (xloc, yloc) to an index on the board array
                    if ((xloc + x) <= (xdim-1) && (xloc + x) >= 0 && (yloc + y) >= 0 && (yloc + y) <= (ydim-1)){ //Condition to check if the coordinates of the neighbour are valid (don't exceed maximum of minimum values)
                        if (board[neigh_index]&hiddenBit()){
                            board[neigh_index] ^= hiddenBit(); //If the neighbour is hidden, only then do we reveal it - otherwise, leave it as is
                        } 
                    }
                }
            }
        }
    }    
    return 0; 
  }