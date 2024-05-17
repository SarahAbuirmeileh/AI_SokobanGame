// Team Members 👩🏻‍💻👩🏻‍💻
// - Sarah Abu irmeileh
// - Asia Shalaldah  

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <vector>
#include <array>
#include <set>

using namespace std;

// The dimension of our array, if you want to edit it you edit it just here, not in the entire code.
const  int rows = 7, columns = 7;

// Constants representing the game elements
int emptySpace = 0;
int wall = 1;
int box = 2;
int storageLocation = 3;
int boxInStorage = 4;
int person = 5;
int personInStorage = 6;

// Constants representing the Q-learning ALG
const int episode = 1000; // The # of trials for computer to learn, usually the # is big
const double y = 0.8; // Constant for learning rate
using Array2D = array<array<int, columns>, rows>;


// The header for all functions
void Init(int initialState [rows][columns]);
void printArray(int array[rows][columns]);
bool IsGoal(int state [rows][columns]);
void GenerateChildren(int state[rows][columns], int children [4][rows][columns]);
void addChild(int children[4][rows][columns], int child[rows][columns], int& index);
bool isDeadLock(int state[rows][columns] );

// The headers for all functions for Q-learning ALG
int getRandomPossibleAction(int state);
void QLearningAlgorithm(int initialState[rows][columns], int QTable[rows][columns], double y, int episodes);
int getReward(int state[rows][columns]);
void initializeQTable(int QTable[rows][columns]);

int main(){

    int initialState [rows][columns];
    Init(initialState);

    int x, y;

    printArray(initialState);
    cout << endl;
    cout << "Enter The Person's Position: (x,y)" << endl;

    cin >> x >> y;
    initialState[x][y] = 5;

    cout << "The initial game is :" << endl << endl;
    printArray(initialState);
    cout << endl << endl;

    int children [4][rows][columns];
    GenerateChildren(initialState,children );

    cout << "All children (possible states) are :" << endl << endl;
    for (int i = 0; i < 4; i++ ){
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
void GenerateChildren(int state[rows][columns], int children [4][rows][columns]) {

    // If the the valid action (child) is top we will store it in the children array in index 0, if left it will be stored in index 1, if right
    // it will be stored in 2 and if bottom in index 3

    // left, right, top, down
    // 0 = go to fill the empty space, replace 5 with 0 and 0 with 5 
    // 1 = do nothing
    // 2 = 0, 5, 2
    // 3 = go to fill the storage space, replace 5 with 0 and 3 with 5 ( will override the storage location)
    // 4 = 0, 5, 2

    // We can say that children is array of 4 elements, each element is a 2d array represent the state

    // To add to the children array 
    index = 0;
    // TODO: Give fixed index for each location (ACTION) for the child


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
                    addChild(children, child, 0);

                }else if (state[i - 1][j] == box || state[i - 1][j] == boxInStorage ) {
                    // if top is a box or box in a storage location
                    // if the place at the top of the box is empty, it will be 2
                    if (state[i - 2][j] == emptySpace){
                        
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i - 1][j] = person;
                        child[i - 2][j] = box;
                        addChild(children, child, 0);

                    }else if (state[i - 2][j] == storageLocation){
                        // if the place at the top of the box is a storage location, it will be 4 
            
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i - 1][j] = person;
                        child[i - 2][j] = boxInStorage;
                        addChild(children, child, 0);
                    }
                }
                // Left
                // if left is empty or storage location 
                if (state[i][j - 1] == emptySpace || state[i][j - 1] == storageLocation){

                    copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                    child[i][j] = state[i][j] == person ? emptySpace : storageLocation;
                    child[i][j - 1] = child[i][j - 1] == emptySpace ? person : personInStorage;
                    addChild(children, child, 1);
                }else if (state[i][j - 1] == box || state[i][j - 1] == boxInStorage){
                    // if left is a box or box in a storage location
                    // if the place at the left of the box is empty, it will be 2
                    if (state[i][j - 2] == emptySpace){

                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i][j - 1] = person;
                        child[i][j - 2] = box;
                        addChild(children, child, 1);

                    }else if (state[i - 2][j] == boxInStorage){
                        // if the place at the left of the box is a storage location, it will be 4 
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i][j - 1] = person;
                        child[i][j - 2] = boxInStorage;
                        addChild(children, child, 1);
                    }
                }
                // Right
                // if right is empty or storage location 
                if (state[i][j + 1] == emptySpace || state[i][j + 1] == storageLocation){

                    copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                    child[i][j] = state[i][j] == person ? emptySpace : storageLocation;
                    child[i][j + 1] = child[i][j + 1]  == emptySpace ? person : personInStorage;
                    addChild(children, child, 2);

                } else if (state[i][j + 1] == box || state[i][j + 1] == boxInStorage){
                    // if right is a box or box in a storage location
                    // if the place at the right of the box is empty, it will be 2
                    if (state[i][j + 2] == emptySpace){
                        
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i][j + 1] = person;
                        child[i][j + 2] = box;
                        addChild(children, child, 2);

                    }else if (state[i][j + 2] == 3){
                        // if the place at the left of the box is a storage location, it will be 4 
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i][j + 1] = person;
                        child[i][j + 2] = boxInStorage;
                        addChild(children, child, 2);
                    }
                }
                // Bottom
                // if bottom is empty or storage location
                if (state[i + 1][j] == emptySpace || state[i + 1][j] == storageLocation){
                    
                    copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                    child[i][j] = state[i][j] == person ? emptySpace : storageLocation;
                    child[i + 1][j] = child[i + 1][j]  == emptySpace ? person : personInStorage;
                    addChild(children, child, 3);

                }else if (state[i + 1][j] == box || state[i + 1][j] == boxInStorage){
                    // if bottom is a box or box in a storage location
                    // if the place at the bottom of the box is empty, it will be 2
                    if (state[i + 2][j] == emptySpace){
                        
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i + 1][j] = person;
                        child[i + 2][j] = box;
                        addChild(children, child, 3);
                    }else if (state[i + 2][j] == boxInStorage){
                        // if the place at the bottom of the box is a storage location, it will be 4 
                        copy(&state[0][0], &state[0][0] + rows * columns, &child[0][0]);

                        child[i][j] = emptySpace;
                        child[i + 1][j] = person;
                        child[i + 1][j] = boxInStorage;
                        addChild(children, child, 3);
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

bool isDeadLock(int state[rows][columns]){
    
    for(int i = 0; i < rows; i++){
        for( int j = 0; j < columns; j++){
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

void QLearningAlgorithm(int initialState[rows][columns], int QTable[rows][columns], double y, int episodes){

    vector<Array2D> states;
    states.push_back(initialState);
    int i=0;

    int state[rows][columns] = initialState;

    int stateId;
    int reward;
    int nextState[rows][columns] ;
    // Loop for all episodes
    while (--episodes){

        // Do while the goal is not reached and not deadlock and i < 100000, in this case while the state != 5
        while (!IsGoal(state) && !isDeadLock(state) && i<100000){

            int children [4][rows][columns];
            GenerateChildren(state, children);

            // Select one random action from this state call it x, this action should be possible
            int x = getRandomPossibleAction(state); // x from 0-3

            // Since the deadlock fun doesn't handle all deadlocked states
            if (x == -1){
                cout << "There is no valid action from this state" << endl;
                return;
            }

            stateId = insertMatrix(states, state);

            // Consider going to the next state N(S,X) 
            // We get that from children[x]
            nextState = children[x];

            reward = getReward(nextState);

            // Get the maximum Q from the x row using QTable
            int maximumQ = -1;
            for (int i = 0; i < 4; i++){
                maximumQ = max(maximumQ, QTable[x][i]);
            }

            // Update the QTable according to this equation
            QTable[stateId][x] = reward + y * maximumQ;

            // Update the state to be the next state which has been chosen randomly
            state = nextState;
            if (x == 5)

            i++;
        }
    }
}

int getRandomPossibleAction(int children [4][rows][columns]){

    // Select random action, this action should be possible
    set <int> uniqueChild;
    while (uniqueChild.size != 4) {

        int randomAction = rand() % 4;

        // If the child array not zero array it's a valid child
        for (int i=0; i<rows; i++){
            for (int j=0; j<columns; j++){
                if (children[randomAction][i][j]!=0){
                    return randomAction;
                }
            }
        }

        uniqueChild.insert(randomAction);

    }
    return -1;
}

int getReward(int state[rows][columns]){
    int reword = 0;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++) {
            if (state[i][j] == 4) {
                reword++;
            }
        }
    }
    return reword;
}

void initializeQTable(int QTable[rows][columns]){
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < columns; ++j){
            QTable[i][j] = -1;
        }
    }
}

// Function to find the index of a matrix in the vector
int findMatrixIndex(const vector<Array2D>& matrixVec, const Matrix3x3& matrix) {
    auto it = find(matrixVec.begin(), matrixVec.end(), matrix);
    if (it != matrixVec.end()) {
        return distance(matrixVec.begin(), it);
    } else {
        return -1; 
    }
}

// Function to insert a matrix into the vector and return its index
int insertMatrix(vector<Array2D>& matrixVec, const Array2D& matrix) {
    int index = findMatrixIndex(matrixVec, matrix);
    if (index == -1) {
        matrixVec.push_back(matrix);
        return matrixVec.size() - 1; // Index of the inserted matrix
    } else {
        return index; // Matrix already exists in the vector
    }
}
