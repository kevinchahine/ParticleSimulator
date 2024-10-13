#include "factory_options.h"

using namespace std;

void FactoryOptions::massNormal(float mean, float stddev) {
	this->_massDist = make_unique<NormalDistribution>(mean, stddev);
}

void FactoryOptions::massUniform(float min, float max) {
	this->_massDist = make_unique<UniformDistribution>(min, max);
}

void FactoryOptions::massConstant(float mass) {
	this->_massDist = make_unique<ConstantDistribution>(mass);
}

void FactoryOptions::positionNormal(
	float meanX, 
	float stddevX, 
	float meanY, 
	float stddevY
) {
	this->_xPosDist = make_unique<NormalDistribution>(meanX, stddevX);
	this->_yPosDist = make_unique<NormalDistribution>(meanY, stddevY);
}

void FactoryOptions::positionUniformCentered(
	const cv::Point2f & center,
	const cv::Size2f & size
) {
	this->_xPosDist = make_unique<UniformDistribution>(center.x - size.width / 2, center.x + size.width / 2);
	this->_yPosDist = make_unique<UniformDistribution>(center.y - size.height / 2, center.y + size.height / 2);
}

void FactoryOptions::positionUniform(
	const cv::Point2f & origin, 
	const cv::Size2f & size
) {
	this->_xPosDist = make_unique<UniformDistribution>(origin.x, origin.x + size.width);
	this->_yPosDist = make_unique<UniformDistribution>(origin.y, origin.y + size.height);
}

void FactoryOptions::positionUniform(const cv::Rect2f & region) {
	cv::Point2f tl = region.tl();
	cv::Point2f br = region.br();

	this->_xPosDist = make_unique<UniformDistribution>(tl.x, br.x);
	this->_yPosDist = make_unique<UniformDistribution>(tl.y, br.y);
}

void FactoryOptions::velocityUniform(
	float minX, 
	float maxX, 
	float minY, 
	float maxY
) {
	this->_xVelDist = make_unique<UniformDistribution>(minX, maxX);
	this->_yVelDist = make_unique<UniformDistribution>(minY, maxY);
}

void FactoryOptions::velocityConstant(float velocityX, float velocityY) {
	this->_xVelDist = make_unique<ConstantDistribution>(velocityX);
	this->_yVelDist = make_unique<ConstantDistribution>(velocityY);
}

void FactoryOptions::velocityNormal(
	float meanX, 
	float stddevX, 
	float meanY, 
	float stddevY
) {
	this->_xVelDist = make_unique<NormalDistribution>(meanX, stddevX);
	this->_yVelDist = make_unique<NormalDistribution>(meanY, stddevY);
}
