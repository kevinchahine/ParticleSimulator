#pragma once

#include <iostream>
#include <opencv2/core/types.hpp>

#include "particle_type.h"

class Particle
{
public:
	cv::Point2f & position() { return _position; }
	const cv::Point2f & position() const { return _position; }

	cv::Point2f & velocity() { return _velocity; }
	const cv::Point2f & velocity() const { return _velocity; }

	float & mass() { return _mass; }
	const float & mass() const { return _mass; }

	virtual std::unique_ptr<Particle> clone();
	
private:
	cv::Point2f _position;
	cv::Point2f _velocity;
	float _mass;
}; // class Particle
