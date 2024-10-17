#pragma once

#include "cloud_template_2.h"

class PositionCloud : public CloudTemplate2<cv::Mat1f, PositionCloud>
{
	using super_t = CloudTemplate2<cv::Mat1f, PositionCloud>;

public:
	PositionCloud(int nParticles = 0) :
		super_t(nParticles) {}
	PositionCloud(const PositionCloud &) = default;
	PositionCloud(PositionCloud &&) noexcept = default;
	virtual ~PositionCloud() noexcept = default;
	PositionCloud & operator=(const PositionCloud &) = default;
	PositionCloud & operator=(PositionCloud &&) noexcept = default;
}; // class PositionCloud
