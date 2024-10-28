#pragma once

#include <opencv2/opencv.hpp>

#include <iostream>

template<typename MATRIX_T>
MATRIX_T integralRAM(const MATRIX_T & y, float x) {
	MATRIX_T integral;

	cv::multiply(y, x, integral);

	return integral;
}

template<typename MATRIX_T>
MATRIX_T integralTrapezoidal(const MATRIX_T & y1, const MATRIX_T & y2, float x) {
	MATRIX_T integral;

	cv::add(y1, y2, integral);
	x /= 2.0f;

	cv::multiply(integral, x, integral);

	return integral;
}