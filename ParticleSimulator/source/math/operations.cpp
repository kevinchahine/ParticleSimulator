#include "operations.h"

void limitMin(cv::Mat1f & mat, float minValue) {
	cv::max(mat, cv::Scalar::all(minValue), mat);
}
