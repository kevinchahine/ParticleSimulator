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

	cv::Mat1f & position() { return static_cast<super_t &>(*this); };
	const cv::Mat1f & position() const { return static_cast<const super_t &>(*this); };
}; // class PositionCloud
