// code.cpp
// Compile & run: g++-12 -std=c++23 code.cpp -o prog && ./prog
#include <iostream>
#include <vector>
#include <unordered_map>
#include <format>

int main(){

	float vertexData[] = {1.0f, 1.0f, 1.0f,
												0.0f, 0.0f, 0.0f,
												2.0f, 0.0f, 1.0f};

	std::vector vertexData2{1.0f, 1.0f, 1.0f,
													0.0f, 0.0f, 0.0f,
													2.0f, 0.0f, 1.0f};
	
	for(auto v : vertexData2){
		std::cout << v << " ";	
	}
	std::cout << std::endl;

	std::unordered_map<std::string, unsigned int> map_example;
	map_example["test"] = 7U;
	for(auto[key,value] : map_example){
		std::cout << std::format("{0}:{1}",key,value) << std::endl;
	}

	return 0;
}
