#include <iostream>

struct MyStruct {
    int** my2DArray;
    int numRows;
    int numCols;

    MyStruct(int rows, int cols) {
        numRows = rows;
        numCols = cols;
        my2DArray = new int*[numRows];
        for (int i = 0; i < numRows; i++) {
            my2DArray[i] = new int[numCols];
        }
    }

    ~MyStruct() {
        for (int i = 0; i < numRows; i++) {
            delete[] my2DArray[i];
        }
        delete[] my2DArray;
    }
};

int main() {
    MyStruct myStruct(9, 4); // Initialize MyStruct with a 3x4 array

    // Access and modify the array
    for (int i = 0; i < myStruct.numRows; i++) {
        for (int j = 0; j < myStruct.numCols; j++) {
            myStruct.my2DArray[i][j] = i + j;
            std::cout << myStruct.my2DArray[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
