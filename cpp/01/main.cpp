#include <iostream> // input and output
#include <cstdlib>
#include <vector>
#include <string>

int main(){

    std::vector<int> intVector;
    intVector.push_back(1); // inserted at index '0'
    intVector.push_back(2);
    intVector.push_back(3);
    intVector.push_back(4);
    intVector.push_back(5);
    intVector.push_back(88);

    std::cout << "size: " << intVector.size() << std::endl;
    std::cout << "intVector[0]: " << intVector[0] << std::endl;
    intVector[0] = 77;
    std::cout << "intVector.at(0): " << intVector.at(0) << std::endl;

    for(int i=0; i < intVector.size(); i++){
        std::cout << intVector[i] << std::endl;
    }

    int* pointerToUnderlyingArray = intVector.data();
    std::cout << "pointer: " << pointerToUnderlyingArray << std::endl;
    std::cout << "pointer[0]: " << pointerToUnderlyingArray[0] << std::endl;

    std::cout << "Hello" << std::endl;

    return EXIT_SUCCESS;
}
