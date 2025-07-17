// Creating and Managing a 2D array
#include <iostream>

// Function to dynamically allocate a 2D array
int** create2DArray(int rows, int cols) {
    int** arr = new int*[rows]; // Allocate array of row pointers
    for (int i = 0; i < rows; i++) {
        arr[i] = new int[cols]; // Allocate array for each row
    }
    return arr;
}

// Function to deallocate a 2D array
void delete2DArray(int** arr, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] arr[i]; // Deallocate each row
    }
    delete[] arr; // Deallocate row pointers
}

int main() {
    int rows;
    int cols = 4;

    std::cout << "Please input number of rows: ";
    std::cin >> rows;

    // Create a 2D array
    int** arr = create2DArray(rows, cols);

    // Initialize the 2D array
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            arr[i][j] = i * cols + j; // Fill with sample data
        }
    }

    // Print the 2D array
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Clean up memory
    delete2DArray(arr, rows);

    return 0;
}
