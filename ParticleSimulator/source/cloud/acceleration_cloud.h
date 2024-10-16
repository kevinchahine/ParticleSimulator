#pragma once

#include "cloud_template_2.h"

class AccelerationCloud : public CloudTemplate2<cv::Mat1f, AccelerationCloud>
{
	using super_t = CloudTemplate2<cv::Mat1f, AccelerationCloud>;

public:
	AccelerationCloud(int nParticles = 0) :
		super_t(nParticles) {}
	AccelerationCloud(const AccelerationCloud &) = default;
	AccelerationCloud(AccelerationCloud &&) noexcept = default;
	virtual ~AccelerationCloud() noexcept = default;
	AccelerationCloud & operator=(const AccelerationCloud &) = default;
	AccelerationCloud & operator=(AccelerationCloud &&) noexcept = default;
}; // class AccelerationCloud
