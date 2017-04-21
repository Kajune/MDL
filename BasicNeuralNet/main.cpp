#include "NeuralNet.hpp"
#include <iostream>

std::pair<Vector<2>, float> TrainingData[] = {
	std::make_pair(Vector<2>{1, 1}, 0.0f),
	std::make_pair(Vector<2>{1, 0}, 1.0f),
	std::make_pair(Vector<2>{0, 1}, 1.0f),
	std::make_pair(Vector<2>{0, 0}, 0.0f),
};

int main() {
	Perceptron<2, 3, SigmoidFunction, DerivedSigmoidFunction> p1(10);
	Perceptron<3, 1, SigmoidFunction, DerivedSigmoidFunction> p2(10);

	p1.SetRandom(-5, 5);
	p2.SetRandom(-5, 5);
	
	float errorcount;
	int count = 0;
	do {
		errorcount = 0.0f;
		for (const auto& it : TrainingData) {
			p1.Backward(p2.Backward(Vector<1>{ it.second } -p2.Forward(p1.Forward(it.first))));
			errorcount += powf(it.second - p2.Forward().at(0), 2.0f);
		}
		std::cout << "Error: " << errorcount << std::endl;
		count++;
	} while (errorcount > 0.0001f);

	std::cout << "Learn Complete" << std::endl;

	while (true) {
		float a, b, c;
		std::cin >> a >> b;
		p1.SetInput({ a, b });
		p2.SetInput(p1.Forward());
		std::cout << p2.Forward().at(0) << std::endl;
	}
}