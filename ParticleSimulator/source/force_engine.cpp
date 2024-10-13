#pragma once

#include "force_engine.h"

#include "stop_watch.h"

using namespace std;

void ForceEngine::initializeCloud(const Cloud & cloud) {
	_cloud = cloud.clone();
}

void ForceEngine::initializeCloud(Cloud && cloud) {
	_cloud = std::move(cloud);
}

void ForceEngine::update() {
	ForceCloud force = this->calcGravitationalForce(_cloud);

	AccelerationCloud accel = this->calcAcceleration(force, _cloud.massCloud());

	VelocityCloud deltaVelocity = this->calcVelocityChange(accel);
	this->applyVelocity(deltaVelocity);

	PositionCloud deltaPosition = this->calcPositionChange(_cloud.velocityCloud());
	this->applyPosition(deltaPosition);
}

ForceCloud ForceEngine::calcGravitationalForce(Cloud & cloud) {
	const int N_PARTICLES = cloud.nParticles();

	// Magnitude of Gravitational Force:
	// |F| = -G * m1 * m2 / d^2
	//	d = |p1 - p2|
	//	G = gravitational constant
	//	m1 = mass of particle 1
	//	m2 = mass of particle 2
	//	p1 = deltaPosition of particle 1
	//	p2 = deltaPosition of particle 2
	//	
	//	Force Vector:
	//		F = |F| * (p2 - p1)

	// --- Numerator ---
	// numerator = -G * m1 * m2
	static cv::Mat1f massRight = cv::repeat(cloud.mass(), 1, N_PARTICLES);
	
	static cv::Mat1f massDown;
	cv::rotate(cloud.mass(), massDown, cv::ROTATE_90_COUNTERCLOCKWISE);
	massDown = cv::repeat(massDown, N_PARTICLES, 1);

	cv::Mat1f numerator;
	cv::multiply(massRight, massDown, numerator);
	numerator *= -5000.0;
	
	// --- Denominator ---
	cv::Mat1f xPos = cloud.position()(cv::Range(0, N_PARTICLES), cv::Range(0, 1));
	cv::Mat1f yPos = cloud.position()(cv::Range(0, N_PARTICLES), cv::Range(1, 2));

	cv::Mat1f xPosRight = cv::repeat(xPos, 1, N_PARTICLES);
	cv::Mat1f xPosDown;
	cv::rotate(xPos, xPosDown, cv::ROTATE_90_COUNTERCLOCKWISE);
	xPosDown = cv::repeat(xPosDown, N_PARTICLES, 1);

	cv::Mat1f yPosRight = cv::repeat(yPos, 1, N_PARTICLES);
	cv::Mat1f yPosDown;
	cv::rotate(yPos, yPosDown, cv::ROTATE_90_COUNTERCLOCKWISE);
	yPosDown = cv::repeat(yPosDown, N_PARTICLES, 1);

	// TODO: maybe its backwards
	cv::Mat1f xDiff = xPosRight - xPosDown;
	cv::Mat1f yDiff = yPosRight - yPosDown;

	// --- Square the Differences ---
	cv::Mat1f xDiffSquared;
	cv::Mat1f yDiffSquared;

	cv::multiply(xDiff, xDiff, xDiffSquared);
	cv::multiply(yDiff, yDiff, yDiffSquared);

	// d^2
	cv::Mat1f distMagnitudeSquared = xDiffSquared + yDiffSquared;

	cv::Mat1f distMagnitude;
	cv::sqrt(distMagnitudeSquared, distMagnitude);

	// Force Magnitude
	cv::Mat1f forceMagnitude;
	cv::divide(numerator, distMagnitude, forceMagnitude);

	cv::Mat1f eye = cv::Mat1f::eye(N_PARTICLES, N_PARTICLES);
	cv::threshold(eye, eye, 0.5, 1.0, cv::ThresholdTypes::THRESH_BINARY_INV);

	cv::multiply(forceMagnitude, eye, forceMagnitude);
	cv::patchNaNs(forceMagnitude, 0.0);

	// Force Direction
	cv::Mat1f unitX;
	cv::Mat1f unitY;
	
	cv::divide(xDiff, distMagnitude, unitX);
	cv::divide(yDiff, distMagnitude, unitY);

	cv::Mat1f forceX;
	cv::Mat1f forceY;

	cv::multiply(forceMagnitude, unitX, forceX);
	cv::multiply(forceMagnitude, unitY, forceY);
	
	cv::patchNaNs(forceX, 0.0);
	cv::patchNaNs(forceY, 0.0);

	// Total Force
	cv::reduce(forceX, forceX, 1, cv::ReduceTypes::REDUCE_SUM);
	cv::reduce(forceY, forceY, 1, cv::ReduceTypes::REDUCE_SUM);

	// Combine into one Matrix
	ForceCloud force(N_PARTICLES);

	forceX.copyTo(force(cv::Rect(0, 0, 1, N_PARTICLES)));
	forceY.copyTo(force(cv::Rect(1, 0, 1, N_PARTICLES)));

	return force;
}

ForceCloud ForceEngine::calcFrictionForce(Cloud & cloud) {
	return ForceCloud();
}

AccelerationCloud ForceEngine::calcAcceleration(
	const ForceCloud & force,
	MassCloud & mass
) const {
	AccelerationCloud accel;

	cv::Mat massRepeated = cv::repeat(mass, 1, 2);

	cv::divide(force, massRepeated, accel.acceleration());

	return accel;
}

VelocityCloud ForceEngine::calcVelocityChange(const AccelerationCloud & accel) {
	VelocityCloud velocity;
	
	velocity.velocity() = accel * _frameDuration;

	// TODO: Trapezoidal Approximation
	// TODO: Simpsons Rule

	return velocity;
}

void ForceEngine::applyVelocity(const VelocityCloud & deltaVelocity) {
	cv::add(_cloud.velocity(), deltaVelocity.velocity(), _cloud.velocity());
}

PositionCloud ForceEngine::calcPositionChange(const VelocityCloud & velocity) {
	PositionCloud deltaPosition;

	// TODO: Trapezoidal Approximation
	// TODO: Simpsons Rule
	deltaPosition.position() = velocity.velocity() * _frameDuration;

	return deltaPosition;
}

void ForceEngine::applyPosition(const PositionCloud & deltaPosition) {
	cv::add(_cloud.position(), deltaPosition.position(), _cloud.position());
}
