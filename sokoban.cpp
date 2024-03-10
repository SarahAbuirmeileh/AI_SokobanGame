#include <iostream>

using namespace std;

// The dimension of our array, if you want to edit it you edit it just here, not in the entire code.
const  int rows = 7, columns = 5;

// The header for all functions
void Init(int initialState [rows][columns]);

int main(){

    // Constants representing the game elements
    int emptySpace = 0;
    int wall = 1;
    int box = 2;
    int storageLocation = 3;
    int boxInStorage = 4;
    int person = 5;

    int initialState [rows][columns];
    Init(initialState);

    return 0;
}

// Function to initialize the game
void Init(int initialState [rows][columns]){
    // The array is sent by reference by default , so we sent the array to this function and it'll be edited as needed
    
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            if (i == 0 || j == 0 || (i == rows - 1) || (j == columns - 1) || ( (i == 3) &&  (j == 1 || j == 2 )) ){
                initialState[i][j] = 1;
            }else if (i == 1 && j == 1){
                initialState[i][j] = 5;
            }else if (  j == 2 && (i == 4 || i == 2)){
                initialState[i][j] = 2;
            }else if ((i == 4 && j == 1) || (i == 5 && j == 3)){
                initialState[i][j] = 3;
            }else{
                initialState[i][j] = 0;
            }
        }
    }
}