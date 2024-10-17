#pragma once

#include "cloud_template_1.h"

class MassCloud : public CloudTemplate1<cv::Mat1f, MassCloud>
{
	using super_t = CloudTemplate1<cv::Mat1f, MassCloud>;

public:
	MassCloud(int nParticles = 0) :
		super_t(nParticles) {}
	MassCloud(const MassCloud &) = default;
	MassCloud(MassCloud &&) noexcept = default;
	virtual ~MassCloud() noexcept = default;
	MassCloud & operator=(const MassCloud &) = default;
	MassCloud & operator=(MassCloud &&) noexcept = default;
}; // class MassCloud
