#pragma once

#include "cloud_template_2.h"

class ForceCloud : public CloudTemplate2<cv::Mat1f, ForceCloud>
{
	using super_t = CloudTemplate2<cv::Mat1f, ForceCloud>;

public:
	ForceCloud(int nParticles = 0) :
		super_t(nParticles) {}
	ForceCloud(const ForceCloud &) = default;
	ForceCloud(ForceCloud &&) noexcept = default;
	virtual ~ForceCloud() noexcept = default;
	ForceCloud & operator=(const ForceCloud &) = default;
	ForceCloud & operator=(ForceCloud &&) noexcept = default;
}; // class ForceCloud
