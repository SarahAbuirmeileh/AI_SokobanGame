// Team Members 👩🏻‍💻👩🏻‍💻
// - Sarah Abu irmeileh
// - Asia Shalaldah  

#include <iostream>
#include <vector>
#include <algorithm>

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
int personInStorage = 6;

// The header for all functions
void Init(int initialState [rows][columns]);
void printArray(int array[rows][columns]);
bool IsGoal(int state [rows][columns]);
void GenerateChildren(int state[rows][columns], int children [4][rows][columns], int &index);
void addChild(int children[4][rows][columns], int child[rows][columns], int& index);
bool isDeadLock(int[][] state);

int main(){

    int initialState [rows][columns];
    Init(initialState);

    cout << "The initial game is :" << endl << endl;
    printArray(initialState);
    cout << endl << endl;

    int children [4][rows][columns], childrenNumber;
    GenerateChildren(initialState,children, childrenNumber );

    cout << "All children (possible states) are :" << endl << endl;
    for (int i = 0; i < childrenNumber; i++ ){
        printArray(children[i]);
        cout << endl;
    }

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
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            if (state[i][j] == box)
                return false;
        }
    }
    return true;
}

// Function to generate all the children or all valid state from the given one
void GenerateChildren(int state[rows][columns], int children [4][rows][columns], int &index) {

    // left, right, top, down
    // 0 = go to fill the empty space, replace 5 with 0 and 0 with 5 
    // 1 = do nothing
    // 2 = 0, 5, 2
    // 3 = go to fill the storage space, replace 5 with 0 and 3 with 5 ( will override the storage location)
    // 4 = 0, 5, 2

    // We can say that children is array of 4 elements, each element is a 2d array represent the state

    // To add to the children array 
    index = 0;


    // The commited code represents other way to solve the issue when move a person to a storage location

    /* Store the index for the  storage locations to make sure that when the person move to a storage location 
       then left it we won't lose the storage location from the state  

       Each location is represented as string from 2 char, the first one is the x-index and the second is y-index
       example: "24" means the location in the array (2,4)*/

    // vector<string>storageLocations;
    // for (int i = 0; i < rows; i++){
    //     for (int j = 0; j < columns; j++){
    //         if (state[i][j] == storageLocation){
    //             storageLocations.push_back(""+i+j);
    //         }
    //     }
    // }

    int child [rows][columns];

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            if (state[i][j] == person || state[i][j] == personInStorage){
                // Top
                // if top is empty or storage location 
                if (state[i - 1][j] == emptySpace || state[i - 1][j] == storageLocation){
                    
                    copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                    // If the the current person is setting on a storage location, when moving him keep the storage location       
                    child[i][j] = state[i][j] == person ? emptySpace : storageLocation;

                    /* If the place which we want to move to is storage location save it as personInStorage if it's empty 
                       store it as person */
                    child[i - 1][j] = child[i - 1][j] == emptySpace ? person : personInStorage;
                    addChild(children, child, index);

                }else if (state[i - 1][j] == box || state[i - 1][j] == boxInStorage ) {
                    // if top is a box or box in a storage location
                    // if the place at the top of the box is empty, it will be 2
                    if (state[i - 2][j] == emptySpace){
                        
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i - 1][j] = person;
                        child[i - 2][j] = box;
                        addChild(children, child, index);

                    }else if (state[i - 2][j] == storageLocation){
                        // if the place at the top of the box is a storage location, it will be 4 
            
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i - 1][j] = person;
                        child[i - 2][j] = boxInStorage;
                        addChild(children, child, index);
                    }
                }
                // Left
                // if left is empty or storage location 
                if (state[i][j - 1] == emptySpace || state[i][j - 1] == storageLocation){

                    copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                    child[i][j] = state[i][j] == person ? emptySpace : storageLocation;
                    child[i][j - 1] = child[i][j - 1] == emptySpace ? person : personInStorage;
                    addChild(children, child, index);
                }else if (state[i][j - 1] == box || state[i][j - 1] == boxInStorage){
                    // if left is a box or box in a storage location
                    // if the place at the left of the box is empty, it will be 2
                    if (state[i][j - 2] == emptySpace){

                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i][j - 1] = person;
                        child[i][j - 2] = box;
                        addChild(children, child, index);

                    }else if (state[i - 2][j] == boxInStorage){
                        // if the place at the left of the box is a storage location, it will be 4 
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i][j - 1] = person;
                        child[i][j - 2] = boxInStorage;
                        addChild(children, child, index);
                    }
                }
                // Right
                // if right is empty or storage location 
                if (state[i][j + 1] == emptySpace || state[i][j + 1] == storageLocation){

                    copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                    child[i][j] = state[i][j] == person ? emptySpace : storageLocation;
                    child[i][j + 1] = child[i][j + 1]  == emptySpace ? person : personInStorage;
                    addChild(children, child, index);

                } else if (state[i][j + 1] == box || state[i][j + 1] == boxInStorage){
                    // if right is a box or box in a storage location
                    // if the place at the right of the box is empty, it will be 2
                    if (state[i][j + 2] == emptySpace){
                        
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i][j + 1] = person;
                        child[i][j + 2] = box;
                        addChild(children, child, index);

                    }else if (state[i][j + 2] == 3){
                        // if the place at the left of the box is a storage location, it will be 4 
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i][j + 1] = person;
                        child[i][j + 2] = boxInStorage;
                        addChild(children, child, index);
                    }
                }
                // Bottom
                // if bottom is empty or storage location
                if (state[i + 1][j] == emptySpace || state[i + 1][j] == storageLocation){
                    
                    copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                    child[i][j] = state[i][j] == person ? emptySpace : storageLocation;
                    child[i + 1][j] = child[i + 1][j]  == emptySpace ? person : personInStorage;
                    addChild(children, child, index);

                }else if (state[i + 1][j] == box || state[i + 1][j] == boxInStorage){
                    // if bottom is a box or box in a storage location
                    // if the place at the bottom of the box is empty, it will be 2
                    if (state[i + 2][j] == emptySpace){
                        
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i + 1][j] = person;
                        child[i + 2][j] = box;
                        addChild(children, child, index);
                    }else if (state[i + 2][j] == boxInStorage){
                        // if the place at the bottom of the box is a storage location, it will be 4 
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i + 1][j] = person;
                        child[i + 1][j] = boxInStorage;
                        addChild(children, child, index);
                    }
                }
            }
        }
    }

    // If the person was in a storage location then moved from it we should keep the storage location unchanged
    // for (int i = 0; i < storageLocations.size(); i++){
    //     int x = storageLocations[i][0] - '0';
    //     int y = storageLocations[i][1] - '0';

    //     for (int childIndex = 0; childIndex < index; childIndex++) {
    //         if (children[childIndex][x][y] == emptySpace)
    //             children[childIndex][x][y] = storageLocation;
    //     } 
    // }
}

void addChild(int children[][rows][columns], int child[rows][columns], int& index) {
    copy(&child[0][0], &child[0][0] + rows * columns, &children[index++][0][0]);
}

bool isDeadLock(int[][] state){
    
    for(int i = 0; i < state.Length; i++){
        for( int j = 0; j < state[i].Length; j++){
            // if the square is a box
            if (state[i][j] == box){
                // if the top and right of the box are wall
                if (state[i - 1][j] == wall && state[i][j + 1] == wall )
                    return true;

                // if the top and left of the box are wall
                if (state[i - 1][j] == wall && state[i][j - 1] == wall )
                    return true;

                // if the bottom and right of the box are wall
                if (state[i + 1][j] == wall && state[i][j + 1] == wall  )
                    return true;

                // if the bottom and left of the box are wall
                if (state[i + 1][j] == wall  && state[i][j - 1] == wall )
                    return true;
            }
        }
    }
    return false;
}
