#pragma once

#include "Vector.hpp"
#include <cmath>

template <typename T>
struct StepFunction {
	T operator()(const T& rhs) {
		return static_cast<T>(rhs >= 0 ? 1.0 : 0.0);
	}
};

template <typename T>
struct SigmoidFunction {
	T operator()(const T& rhs) {
		return static_cast<T>(1.0 / (1 + std::exp(-rhs)));
	}
};

template <size_t Input, size_t Output, template <typename DataType> class OutputFunction, typename DataType = float>
class Perceptron {
	std::array<Vector<Input, DataType>, Output> m_weight;
	Vector<Output, DataType> m_threshold;
public:
	Perceptron() = default;
	Perceptron(const Perceptron&) = default;
	Perceptron(Perceptron&&) = default;
	Perceptron& operator=(const Perceptron&) = default;
	Perceptron& operator=(Perceptron&&) = default;

	Perceptron(const std::array<Vector<Input, DataType>, Output>& weight, const Vector<Output, DataType>& threshould)
		: m_weight(weight), m_threshold(threshould){
	}

	Vector<Output, DataType> Forward(const Vector<Input, DataType>& input) const {
		Vector<Output, DataType> ret;
		OutputFunction<DataType> func;
		for (size_t i = 0; i < Output; i++) {
			ret.at(i) = func(DotProduct(input, m_weight.at(i)) - m_threshold.at(i));
		}
		return ret;
	}
};
