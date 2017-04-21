#pragma once

#include "Vector.hpp"
#include <cmath>
#include <random>

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

template <typename T>
struct DerivedSigmoidFunction {
	T operator()(const T& rhs) {
		return static_cast<T>(rhs * (1.0 - rhs));
	}
};

template <size_t Input, size_t Output, 
	template <typename DataType> class OutputFunction,
	template <typename DataType> class DerivedOutputFunction,
	typename DataType = float>
class Perceptron {
	std::array<Vector<Input, DataType>, Output> m_weight;
	Vector<Output, DataType> m_threshold;
	DataType m_learningRate = 1.0;

	Vector<Input, DataType> m_input;
	mutable bool m_isChanged = true;

public:
	Perceptron() = default;
	Perceptron(const Perceptron&) = default;
	Perceptron(Perceptron&&) = default;
	Perceptron& operator=(const Perceptron&) = default;
	Perceptron& operator=(Perceptron&&) = default;

	Perceptron(DataType learningRate)
		: m_learningRate(learningRate) {
	}

	Perceptron(DataType learningRate,
		const std::array<Vector<Input, DataType>, Output>& weight, 
		const Vector<Output, DataType>& threshould)
		: m_learningRate(learningRate), m_weight(weight), m_threshold(threshould){
	}

	void SetRandom(DataType min, DataType max) {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<DataType> dist(min, max);
		for (size_t i = 0; i < Output; i++) {
			for (size_t j = 0; j < Input; j++) {
				m_weight.at(i).at(j) = dist(mt);
			}
			m_threshold.at(i) = dist(mt);
		}
	}

	void SetInput(const Vector<Input, DataType>& input) {
		m_input = input;
		m_isChanged = true;
	}

	Vector<Output, DataType> Forward(const Vector<Input, DataType>& input) {
		SetInput(input);
		return Forward();
	}

	Vector<Output, DataType> Forward() const {
		static Vector<Output, DataType> ret;
		if (m_isChanged) {
			OutputFunction<DataType> func;
			for (size_t i = 0; i < Output; i++) {
				ret.at(i) = func(DotProduct(m_input, m_weight.at(i)) - m_threshold.at(i));
			}
			m_isChanged = false;
		}
		return ret;
	}

	Vector<Input, DataType> Backward(const Vector<Output, DataType>& error) {
		Vector<Input, DataType> ret;
		DerivedOutputFunction<DataType> func;
		for (size_t i = 0; i < Output; i++) {
			auto d = error.at(i) * func(Forward().at(i));
			m_weight.at(i) += m_input * m_learningRate * d;
			m_threshold.at(i) -= m_learningRate * d;
			ret += m_weight.at(i) * d;
		}
		return ret;
	}
};
