#include "NeuralNet.hpp"
#include <iostream>

int main() {
	Perceptron<2, 2, StepFunction> p1({ Vector<2>{-2, 3}, Vector<2>{-2, 1} }, { -1.0, 0.5 });
	Perceptron<2, 1, StepFunction> p2({ Vector<2>{-60, 94} }, { -1.0 });
	Vector<2> vec{ 0, 0 };
	auto ret = p2.Forward(p1.Forward(vec));

	std::cout << ret.at(0) << std::endl;
}