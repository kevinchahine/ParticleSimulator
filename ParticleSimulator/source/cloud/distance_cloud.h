#pragma once

#include "cloud_template_square.h"

class DistanceCloud : public CloudTemplateSquare<cv::Mat1f, DistanceCloud>
{
	using super_t = CloudTemplateSquare<cv::Mat1f, DistanceCloud>;

public:
	DistanceCloud(int nParticles = 0) :
		super_t(nParticles) {}
	DistanceCloud(const DistanceCloud &) = default;
	DistanceCloud(DistanceCloud &&) noexcept = default;
	virtual ~DistanceCloud() noexcept = default;
	DistanceCloud & operator=(const DistanceCloud &) = default;
	DistanceCloud & operator=(DistanceCloud &&) noexcept = default;
}; // class DistanceCloud
