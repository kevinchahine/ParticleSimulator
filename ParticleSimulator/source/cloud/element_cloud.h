#pragma once

#include "cloud_template_1.h"

class ElementCloud : public CloudTemplate1<cv::Mat1i, ElementCloud>
{
	using super_t = CloudTemplate1<cv::Mat1i, ElementCloud>;

public:
	ElementCloud(int nParticles = 0) :
		super_t(nParticles) {}
	ElementCloud(const ElementCloud &) = default;
	ElementCloud(ElementCloud &&) noexcept = default;
	virtual ~ElementCloud() noexcept = default;
	ElementCloud & operator=(const ElementCloud &) = default;
	ElementCloud & operator=(ElementCloud &&) noexcept = default;

	cv::Mat1i & element() { return static_cast<super_t &>(*this); };
	const cv::Mat1i & element() const { return static_cast<const super_t &>(*this); };
}; // class ElementCloud
