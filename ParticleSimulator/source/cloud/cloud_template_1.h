#pragma once

#include "cloud_template.h"

template<typename MATRIX_T, typename DERIVED_T>
class CloudTemplate1 : public CloudTemplate<MATRIX_T, DERIVED_T>
{
public:
	CloudTemplate1(int nParticles) :
		CloudTemplate<MATRIX_T, DERIVED_T>(nParticles, 1) {}
	CloudTemplate1(const CloudTemplate1 &) = default;
	CloudTemplate1(CloudTemplate1 &&) noexcept = default;
	virtual ~CloudTemplate1() noexcept = default;
	CloudTemplate1 & operator=(const CloudTemplate1 &) = default;
	CloudTemplate1 & operator=(CloudTemplate1 &&) noexcept = default;

	float at(size_t particleIndex) const {
		return this->at<MATRIX_T::value_type>(particleIndex, 0);
	}

	void at(size_t particleIndex, float point) {
		this->at<MATRIX_T::value_type>(particleIndex, 0) = point;
	}

	// *** Unhide inherited methods ***
	using CloudTemplate<MATRIX_T, DERIVED_T>::at;

	float centroid() const;
}; // class CloudTemplate1

template<typename MATRIX_T, typename DERIVED_T>
inline float CloudTemplate1<MATRIX_T, DERIVED_T>::centroid() const {
	cv::Mat1f average;
	cv::reduce(*this, average, 0, cv::REDUCE_AVG);

	return average.at<float>(0, 0);
}
