#include <iostream>

using namespace std;

// The dimension of our array, if you want to edit it you edit it just here, not in the entire code.
const  int rows = 7, columns = 5;

// Constants representing the game elements
int emptySpace = 0;
int wall = 1;
int box = 2;
int storageLocation = 3;
int boxInStorage = 4;
int person = 5;

// The header for all functions
void Init(int initialState [rows][columns]);
void printArray(int array[rows][columns]);
bool IsGoal(int state [rows][columns]);


int main(){

    int initialState [rows][columns];
    Init(initialState);
    printArray(initialState);

    return 0;
}

// Function to initialize the game
void Init(int initialState [rows][columns]){
    // The array is sent by reference by default , so we sent the array to this function and it'll be edited as needed
    
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            if (i == 0 || j == 0 || (i == rows - 1) || (j == columns - 1) || ( (i == 3) &&  (j == 1 || j == 2 )) ){
                initialState[i][j] = wall;
            }else if (i == 1 && j == 1){
                initialState[i][j] = person;
            }else if (  j == 2 && (i == 4 || i == 2)){
                initialState[i][j] = box;
            }else if ((i == 4 && j == 1) || (i == 5 && j == 3)){
                initialState[i][j] = storageLocation;
            }else{
                initialState[i][j] = emptySpace;
            }
        }
    }
}

// Function to print 2D array
void printArray(int array[rows][columns]){

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            cout <<  array[i][j] << " ";
        }
        cout << endl;
    }
}

// Function to determine weather the state is a goal state or not
bool IsGoal(int state [rows][columns]){
    // Goal = no 2's 
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            if (state[i][j] == box)
                return false;
        }
    }
    return true;
}