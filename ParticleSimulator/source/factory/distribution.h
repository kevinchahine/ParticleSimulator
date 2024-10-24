#pragma once

#include <random>

#include "source/particle_type.h"

class Distribution
{
public:
}; // class Distribution

class ContinuousDistribution : public Distribution
{
public:
	virtual float operator()(std::default_random_engine & eng) = 0;

	float generate(std::default_random_engine & eng) {
		return this->operator()(eng);
	}
}; // class ContinuousDistribution

class DiscreteDistribution : public Distribution
{
public:
	DiscreteDistribution() = default;

	template <typename InputIt>
	DiscreteDistribution(const InputIt & first, const InputIt & last) :
		dist(first, last) {}

	DiscreteDistribution(std::initializer_list<double> weights) :
		dist(weights) {}

	virtual int operator()(std::default_random_engine & eng) {
		return dist(eng);
	}

	int generate(std::default_random_engine & eng) {
		return this->operator()(eng);
	}

protected:
	std::discrete_distribution<int> dist;
}; // class DiscreteDistribution 

class ParticleTypeDistribution
{
public:
	ParticleTypeDistribution() = default;

	template <typename InputIt>
	ParticleTypeDistribution(const InputIt & first, const InputIt & last) :
		dist(first, last) {}

	ParticleTypeDistribution(std::initializer_list<double> weights) :
		dist(weights) {}
	
	ParticleTypeDistribution(std::initializer_list<ParticleType> particleTypes) {
		std::vector<int> weights;
		weights.resize(ParticleType::COUNT, 0);

		for (const ParticleType & pt : particleTypes) {
			weights.at(pt.value()) = 1;
		}

		dist = std::discrete_distribution<int>(weights.begin(), weights.end());
	}

	ParticleType operator()(std::default_random_engine & eng) {
		ParticleType::value_t id = dist(eng);

		return ParticleType(id);
	}

	ParticleType generate(std::default_random_engine & eng) {
		return this->operator()(eng);
	}

protected:
	std::discrete_distribution<int> dist;
}; // class ParticleTypeDistribution 

class ConstantDistribution : public ContinuousDistribution
{
public:
	ConstantDistribution(float value) :
		value(value) {}

	virtual float operator()(std::default_random_engine & eng) override {
		return value;
	}

private:
	float value = 0.0f;
}; // class NullDistribution 

class UniformDistribution : public ContinuousDistribution
{
public:
	UniformDistribution(float min = 0.0f, float max = 0.0f) :
		dist(min, max) {}

	virtual float operator()(std::default_random_engine & eng) override {
		return dist(eng);
	}

private:
	std::uniform_real_distribution<float> dist;
}; // class UniformDistribution

class NormalDistribution : public ContinuousDistribution
{
public:
	NormalDistribution(float mean = 0.0f, float stddev = 1.0f) :
		dist(mean, stddev) {}

	virtual float operator()(std::default_random_engine & eng) override {
		return dist(eng);
	}

private:
	std::normal_distribution<float> dist;
}; // class NormalDistribution
