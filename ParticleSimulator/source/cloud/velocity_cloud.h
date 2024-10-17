#pragma once

#include "cloud_template_2.h"

class VelocityCloud : public CloudTemplate2<cv::Mat1f, VelocityCloud>
{
	using super_t = CloudTemplate2<cv::Mat1f, VelocityCloud>;

public:
	VelocityCloud(int nParticles = 0) :
		super_t(nParticles) {}
	VelocityCloud(const VelocityCloud &) = default;
	VelocityCloud(VelocityCloud &&) noexcept = default;
	virtual ~VelocityCloud() noexcept = default;
	VelocityCloud & operator=(const VelocityCloud &) = default;
	VelocityCloud & operator=(VelocityCloud &&) noexcept = default;
}; // class VelocityCloud
