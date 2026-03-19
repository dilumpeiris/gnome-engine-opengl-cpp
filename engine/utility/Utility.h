#pragma once
#include <glm/glm.hpp>
#include <iostream>

class Utility {
  public:
	static void printMat(glm::mat4 mat) {
		std::cout << "Matrix: " << std::endl;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				std::cout << mat[col][row]; // [column][row] — column-major!
				if (col < 3)
					std::cout << "\t";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
};