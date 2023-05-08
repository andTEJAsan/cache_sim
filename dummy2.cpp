#include <iostream>

// Define the struct
struct Point {
public:
    // Constructor
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    // Public methods
    int getX() { return x; }
    int getY() { return y; }

private:
    // Private data members
    int x;
    int y;
};

int main() {
    // Create a Point instance using the constructor
    Point p1(1, 2);

    // Access the data members using public methods
    std::cout << "p1.x: " << p1.getX() << std::endl;
    std::cout << "p1.y: " << p1.getY() << std::endl;

    return 0;
}
