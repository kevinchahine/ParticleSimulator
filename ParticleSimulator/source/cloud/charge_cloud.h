#pragma once

#include "cloud_template_1.h"

class ChargeCloud : public CloudTemplate1<cv::Mat1f, ChargeCloud>
{
	using super_t = CloudTemplate1<cv::Mat1f, ChargeCloud>;

public:
	ChargeCloud(int nParticles = 0) :
		super_t(nParticles) {}
	ChargeCloud(const ChargeCloud &) = default;
	ChargeCloud(ChargeCloud &&) noexcept = default;
	virtual ~ChargeCloud() noexcept = default;
	ChargeCloud & operator=(const ChargeCloud &) = default;
	ChargeCloud & operator=(ChargeCloud &&) noexcept = default;
}; // class ChargeCloud
