#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <algorithm> 

using namespace std;

const int rows = 7;
const int columns = 7;

int episode = 1000; // The # of trials for computer to learn, usually the # is big
double y = 0.8; // Constant for learning rate

using Array2D = array<array<int, columns>, rows>;


// The headers for all functions
// void printArray(int array[rows][columns]);
// void initializeRewardArray(int Reward[rows][columns]);
int getRandomPossibleAction(int state, int Reward[rows][columns]);
void QLearningAlgorithm(int initialState[rows][columns], int QTable[rows][columns], double y, int episodes);
int getReward(int state[rows][columns]);
void initializeQTable(int QTable[rows][columns]);

int main(){

    // The rows represents the states and the columns represents the actions
    int QTable[rows][columns];

    vector<Array2D> states;

    // We want to reach F. Hence, F-> is the goal
    // We can add 'A' to each row & column to make the result more familiar to the table in the Readme file

    // Initialize the Q-table array and initially all it's values are -1
    void initializeQTable(QTable);



    // Seed for random number generator
    srand(time(0));

    // Apply Q-learning algorithm
    QLearningAlgorithm(Reward, QTable, y, episode);

    cout << "The Q table after " << episode << " episodes is : " << endl
         << endl;
    printArray(QTable);
    cout << endl;

    return 0;
}

// void printArray(int array[rows][columns]){

//     for (int i = 0; i < rows; i++){
//         for (int j = 0; j < columns; j++){
//             cout <<  array[i][j] << " ";
//         }
//         cout << endl;
//     }
// }

// void initializeRewardArray(int Reward[rows][columns]){

//     // Assigning the values to the Reward array as the picture in the ReadMe file
//     for (int i = 0; i < rows; i++){
//         for (int j = 0; j < columns; j++){
//             if (j == 5 && (i == 1 || i == 4 || i == 5)){

//                 // If the column is F (If we reach the goal) give it big value
//                 // In other words if there is a direct action from current state to the gaol state
//                 Reward[i][j] = 100;

//             }else if ((i == 0 && j == 4) || ((i == 1 || i == 2) && ( j == 3)) || (i == 5 && ( j == 1 || j == 4)) ){

//                 // If there is a possible action give it value 0
//                 Reward[i][j] = 0;

//             }else if (((i == 3) && (j == 1 || j == 2 || j == 4)) || ( i == 4 && ( j == 0 || j == 3))){

//                 // If there is a possible action give it value 0
//                 Reward[i][j] = 0;
//             }else{

//                 // If there is no possible action (movement) give it -1
//                 Reward[i][j] = -1;
//             }
//         }
//     }
// }

void QLearningAlgorithm(int initialState[rows][columns], int QTable[rows][columns], double y, int episodes){

    // Loop for all episodes
    while (episodes--){

        // Do while the goal is not reached, in this case while the state != 5
        while (true){

            // Select one random action from this state call it x, this action should be possible
            int x = getRandomPossibleAction(state, Reward);

            // Get the maximum Q from the x row using QTable
            int maximumQ = -1;
            for (int i = 0; i < columns; i++)
            {
                maximumQ = max(maximumQ, QTable[x][i]);
            }

            // Update the QTable according to this equation
            QTable[state][x] = Reward[state][x] + y * maximumQ;

            // Update the state to be the next state which has been chosen randomly
            state = x;
            if (x == 5)
            {
                break;
                ;
            }
        }
    }
}

int getRandomPossibleAction(int state, int Reward[rows][columns])
{

    // Select random action, this action should be possible
    while (true)
    {

        int randomAction = rand() % columns;

        // If it's possible action break
        if (Reward[state][randomAction] != -1)
        {
            return randomAction;
        }
    }
}

int getReward(int state[rows][columns])
{
    int reword = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (state[i][j] == 4)
            {
                reword++;
            }
        }
    }
    return reword;
}

void initializeQTable(int QTable[rows][columns])
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
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