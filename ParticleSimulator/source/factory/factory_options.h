#pragma once

#include <stdint.h>

#include <opencv2/core/types.hpp>

#include "distribution.h"
#include "../particle_type.h"

class FactoryOptions
{
	friend class Factory;

public:
	void nParticles(size_t count) { _nParticles = count; }
	size_t nParticles() const { return _nParticles; }

	void massNormal(float mean = 1.0f, float stddev = 1.0f);
	void massUniform(float min = -1.0f, float max = 1.0f);
	void massConstant(float mass = 1.0f);

	void positionNormal(float meanX, float stddevX, float meanY, float stddevY);
	void positionUniformCentered(const cv::Point2f & center, const cv::Size2f & size);
	void positionUniform(const cv::Point2f & origin, const cv::Size2f & size);
	void positionUniform(const cv::Rect2f & region);

	void velocityConstant(float velocityX, float velocityY);
	void velocityNormal(float meanX, float stddevX, float meanY, float stddevY);
	void velocityUniform(float minX, float maxX, float minY, float maxY);

	void typeDiscrete(const std::initializer_list<ParticleType> & types);

private:
	size_t _nParticles = 1000;
	std::unique_ptr<ContinuousDistribution> _massDist = std::make_unique<ConstantDistribution>(0.0f);
	std::unique_ptr<ContinuousDistribution> _xPosDist = std::make_unique<ConstantDistribution>(0.0f);
	std::unique_ptr<ContinuousDistribution> _yPosDist = std::make_unique<ConstantDistribution>(0.0f);
	std::unique_ptr<ContinuousDistribution> _xVelDist = std::make_unique<ConstantDistribution>(0.0f);
	std::unique_ptr<ContinuousDistribution> _yVelDist = std::make_unique<ConstantDistribution>(0.0f);
	std::unique_ptr<ParticleTypeDistribution> _typeDist = std::make_unique<ParticleTypeDistribution>();
}; // class FactoryOptions