#include "position_cloud.h"

cv::Mat1f PositionCloud::calcDistance() const {
	cv::Mat1f dist = this->calcSquareDistance();

	cv::sqrt(dist, dist);

	return dist;
}

cv::Mat1f PositionCloud::calcSquareDistance() const {
	const int N_PARTICLES = this->nParticles();

	// --- 1.) Slice ---
	cv::Mat1f xPos = this->sliceX();
	cv::Mat1f yPos = this->sliceY();

	// --- 2.) Repeat X ---
	cv::Mat1f xPosRight = cv::repeat(xPos, 1, N_PARTICLES);
	cv::Mat1f xPosDown;
	cv::rotate(xPos, xPosDown, cv::RotateFlags::ROTATE_90_COUNTERCLOCKWISE);
	xPosDown = cv::repeat(xPosDown, N_PARTICLES, 1);

	// --- 3.) Repeat Y ---
	cv::Mat1f yPosRight = cv::repeat(yPos, 1, N_PARTICLES);
	cv::Mat1f yPosDown;
	cv::rotate(yPos, yPosDown, cv::RotateFlags::ROTATE_90_COUNTERCLOCKWISE);
	yPosDown = cv::repeat(yPosDown, N_PARTICLES, 1);

	// --- 4.) Distance ---
	cv::Mat1f xDist;
	cv::Mat1f yDist;

	cv::subtract(xPosRight, xPosDown, xDist);
	cv::subtract(yPosRight, yPosDown, yDist);

	// --- 5.) Square Distances ---
	cv::multiply(xDist, xDist, xDist);
	cv::multiply(yDist, yDist, yDist);

	// --- 6.) Sum ---
	cv::Mat1f squareDistance;

	cv::add(xDist, yDist, squareDistance);

	return squareDistance;
}
