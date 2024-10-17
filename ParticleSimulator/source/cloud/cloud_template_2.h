#pragma once

#include "cloud_template.h"

template<typename MATRIX_T, typename DERIVED_T>
class CloudTemplate2 : public CloudTemplate<MATRIX_T, DERIVED_T>
{
public:
	using super_t = CloudTemplate<MATRIX_T, DERIVED_T>;

public:
	CloudTemplate2(int nParticles) :
		CloudTemplate<MATRIX_T, DERIVED_T>(nParticles, 2) {}
	CloudTemplate2(const CloudTemplate2 &) = default;
	CloudTemplate2(CloudTemplate2 &&) noexcept = default;
	virtual ~CloudTemplate2() noexcept = default;
	CloudTemplate2 & operator=(const CloudTemplate2 &) = default;
	CloudTemplate2 & operator=(CloudTemplate2 &&) noexcept = default;

	cv::Point2f at(size_t particleIndex) const {
		return cv::Point2f{
			this->at<MATRIX_T::value_type>(particleIndex, 0),
			this->at<MATRIX_T::value_type>(particleIndex, 1)
		};
	}

	void at(size_t particleIndex, const cv::Point2f & point) {
		this->at<MATRIX_T::value_type>(particleIndex, 0) = point.x;
		this->at<MATRIX_T::value_type>(particleIndex, 1) = point.y;
	}

	// *** Unhide inherited methods ***
	using CloudTemplate<MATRIX_T, DERIVED_T>::at;

	cv::Point2f centroid() const;

	cv::Mat sliceX();
	const cv::Mat sliceX() const;

	cv::Mat sliceY();
	const cv::Mat sliceY() const;
}; // class CloudTemplate2

template<typename MATRIX_T, typename DERIVED_T>
cv::Point2f CloudTemplate2<MATRIX_T, DERIVED_T>::centroid() const {
	cv::Mat1f average;
	cv::reduce(*this, average, 0, cv::REDUCE_AVG);

	return cv::Point2f{ average.at<float>(0, 0), average.at<float>(0, 1) };
}

template<typename MATRIX_T, typename DERIVED_T>
cv::Mat CloudTemplate2<MATRIX_T, DERIVED_T>::sliceX() {
	MATRIX_T & mat = (*this);

	cv::Range down(0, this->nParticles());
	cv::Range right(0, 1);

	return mat(down, right);
}

template<typename MATRIX_T, typename DERIVED_T>
const cv::Mat CloudTemplate2<MATRIX_T, DERIVED_T>::sliceX() const {
	return const_cast<CloudTemplate2<MATRIX_T, DERIVED_T> &>(*this).sliceX();
}

template<typename MATRIX_T, typename DERIVED_T>
cv::Mat CloudTemplate2<MATRIX_T, DERIVED_T>::sliceY() {
	MATRIX_T & mat = (*this);

	cv::Range down(0, this->nParticles());
	cv::Range right(1, 2);

	return mat(down, right);
}

template<typename MATRIX_T, typename DERIVED_T>
const cv::Mat CloudTemplate2<MATRIX_T, DERIVED_T>::sliceY() const {
	return const_cast<CloudTemplate2<MATRIX_T, DERIVED_T> &>(*this).sliceY();
}
