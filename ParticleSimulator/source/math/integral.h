#pragma once

#include <opencv2/opencv.hpp>

#include <iostream>

template<typename MATRIX_T>
MATRIX_T integralRAM(const MATRIX_T & y, float deltaX) {
	MATRIX_T integral;

	cv::multiply(y, deltaX, integral);

	return integral;
}

template<typename MATRIX_T>
MATRIX_T integralTrapezoidal(const MATRIX_T & y1, const MATRIX_T & y2, float deltaX) {
	MATRIX_T integral;

	cv::add(y1, y2, integral);
	deltaX /= 2.0f;

	cv::multiply(integral, deltaX, integral);

	return integral;
}

template<class MATRIX_T>
MATRIX_T integralSimpsons(
	const MATRIX_T & y1,
	const MATRIX_T & y2,
	const MATRIX_T & y3,
	float x
) {
	MATRIX_T integral;

	MATRIX_T y2Times4;
	cv::multiply(y2, 4.0f, y2Times4);

	cv::add(y1, y2Times4, integral);
	cv::add(integral, y3, integral);

	cv::multiply(integral, x / 3.0f, integral);

	return integral;
}