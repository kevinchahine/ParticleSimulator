#pragma once

#include <random>

class Distribution
{
public:
	virtual float operator()(std::default_random_engine & eng) = 0;

	virtual float generate(std::default_random_engine & eng) = 0;
}; // class Distribution

class ConstantDistribution : public Distribution
{
public:
	ConstantDistribution(float value) : 
		value(value) {}

	virtual float operator()(std::default_random_engine & eng) override {
		return value;
	}

	virtual float generate(std::default_random_engine & eng) override {
		return this->operator()(eng);
	}

private:
	float value = 0.0f;
}; // class NullDistribution 

class UniformDistribution : public Distribution
{
public:
	UniformDistribution(float min = 0.0f, float max = 0.0f) :
		dist(min, max) {}

	virtual float operator()(std::default_random_engine & eng) override {
		return dist(eng);
	}

	virtual float generate(std::default_random_engine & eng) override {
		return this->operator()(eng);
	}

private:
	std::uniform_real_distribution<float> dist;
}; // class UniformDistribution

class NormalDistribution : public Distribution
{
public:
	NormalDistribution(float mean = 0.0f, float stddev = 1.0f) :
		dist(mean, stddev) {}

	virtual float operator()(std::default_random_engine & eng) override {
		return dist(eng);
	}

	virtual float generate(std::default_random_engine & eng) override {
		return this->operator()(eng);
	}

private:
	std::normal_distribution<float> dist;
}; // class NormalDistribution