#include "HillClimbAlgorithm.h"

int main(int argc, char* argv[]) {
	
	if (argc < 4 || argc > 4) {

		std::cout << "Wrong arguments!\n";
		std::cout << "Input the path to image, triangle count, and mutation limit of the triangles!";
		return -1;
	}
	std::string path = argv[1];
	int triangle_limit = std::stoi(argv[2]);
	int mutationLimit = std::stoi(argv[3]);

	HillClimbAlgorithm climb(path, triangle_limit, mutationLimit);
	climb.start();

	return 0;
}