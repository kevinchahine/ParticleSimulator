#pragma once

#include "force_engine.h"
#include "stop_watch.h"
#include "math/integral.h"
#include "math/operations.h"

using namespace std;

void ForceEngine::initialize(const Cloud & cloud) {
	_cloud = cloud.clone();
	size_t nParticles = _cloud.nParticles();

	size_t c = _accelerations.capacity();
	for (size_t i = 0; i < c; i++) {
		_accelerations.push_front(AccelerationCloud(nParticles));
	}

	c = _velocities.capacity();
	for (size_t i = 0; i < c; i++) {
		_velocities.push_front(_cloud.velocity());
	}
}

void ForceEngine::initialize(Cloud && cloud) {
	_cloud = std::move(cloud);
}

void ForceEngine::update() {
	ForceCloud gravitationalForce = this->calcGravitationalForce(_cloud);
	//ForceCloud coulombsForce = this->calcCoulombForce(_cloud);

	ForceCloud force = gravitationalForce;
	//cv::add(gravitationalForce, coulombsForce, force);

	AccelerationCloud accel = this->calcAcceleration(force, _cloud.mass());
	this->updateAcceleration(accel);
	this->updateVelocity();
	this->updatePosition();
}

float ForceEngine::frameRate() const {
	return 1.0f / _frameDuration;
}

void ForceEngine::frameRate(float rate) {
	_frameDuration = 1.0f / rate;
}

float ForceEngine::frameDuration() const {
	return _frameDuration;
}

void ForceEngine::frameDuration(float frameDuration) {
	_frameDuration = frameDuration;
}

float ForceEngine::timeScalar() const {
	return _timeScalar;
}

void ForceEngine::timeScalar(float timeScalar) {
	_timeScalar = timeScalar;
}

ForceCloud ForceEngine::calcGravitationalForce(Cloud & cloud) {
	const int N_PARTICLES = cloud.nParticles();

	// Identity matrix. Used a lot.
	const cv::Mat1f eye = cv::Mat1f::eye(N_PARTICLES, N_PARTICLES);
	cv::Mat1f eyeInverse;
	cv::multiply(eye, cv::Scalar::all(-1.0f), eyeInverse);
	cv::add(eyeInverse, cv::Scalar::all(1.0f), eyeInverse);

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
	cv::Mat1f massRight = cv::repeat(cloud.mass(), 1, N_PARTICLES);
	
	cv::Mat1f massDown;
	cv::rotate(cloud.mass(), massDown, cv::ROTATE_90_COUNTERCLOCKWISE);
	massDown = cv::repeat(massDown, N_PARTICLES, 1);

	cv::Mat1f numerator;
	cv::multiply(massRight, massDown, numerator);
	numerator *= -Constants::gravitational * _timeScalar;
	
	// --- Denominator ---
	cv::Mat1f xPos = cloud.position().sliceX();
	cv::Mat1f yPos = cloud.position().sliceY();
	
	cv::Mat1f xPosRight = cv::repeat(xPos, 1, N_PARTICLES);
	cv::Mat1f xPosDown;
	cv::rotate(xPos, xPosDown, cv::ROTATE_90_COUNTERCLOCKWISE);
	xPosDown = cv::repeat(xPosDown, N_PARTICLES, 1);
	
	cv::Mat1f yPosRight = cv::repeat(yPos, 1, N_PARTICLES);
	cv::Mat1f yPosDown;
	cv::rotate(yPos, yPosDown, cv::ROTATE_90_COUNTERCLOCKWISE);
	yPosDown = cv::repeat(yPosDown, N_PARTICLES, 1);
	
	cv::Mat1f xDiff;
	cv::Mat1f yDiff;
	
	cv::subtract(xPosRight, xPosDown, xDiff);
	cv::subtract(yPosRight, yPosDown, yDiff);

	// --- Square the Differences ---
	cv::Mat1f xDiffSquared;
	cv::Mat1f yDiffSquared;
	
	cv::multiply(xDiff, xDiff, xDiffSquared);
	cv::multiply(yDiff, yDiff, yDiffSquared);
	
	// d^2
	cv::Mat1f distMagnitudeSquared;
	cv::add(xDiffSquared, yDiffSquared, distMagnitudeSquared);
	
	cv::Mat1f distMagnitude;
	cv::sqrt(distMagnitudeSquared, distMagnitude);
	
	// --- Limit Small Distances ---
	// Limit the smallest distance to prevent resulting extreamly large 
	// Forces when particles are very close together.
	//float minDistance = 1.0f;
	//limitMin(distMagnitude, minDistance);
	//cv::multiply(distMagnitude, eyeInverse, distMagnitude);

	// Force Magnitude
	cv::Mat1f forceMagnitude;
	cv::divide(numerator, distMagnitude, forceMagnitude);

	// Make matrix with 0s on main diagonal, 1s everywhere else.
	cv::threshold(eye, eye, 0.5, 1.0, cv::ThresholdTypes::THRESH_BINARY_INV);

	cv::multiply(forceMagnitude, eye, forceMagnitude);// Set diagonals to zero
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

ForceCloud ForceEngine::calcCoulombForce(Cloud & cloud) {
	const int N_PARTICLES = cloud.nParticles();

	// Magnitude of Coulomb's Force:
	// |F| = k * q1 * q2 / d^2
	//	d = |p1 - p2|
	//	k = Coulomb's constant
	//	q1 = Charge of particle 1
	//	q2 = Charge of particle 2
	//	p1 = deltaPosition of particle 1
	//	p2 = deltaPosition of particle 2
	//	
	//	Force Vector of Coulomb's Force:
	//		F = |F| * (p2 - p1)

	// --- Numerator ---
	// numerator = k * q1 * q2
	cv::Mat1f chargeRight = cv::repeat(cloud.charge(), 1, N_PARTICLES);

	cv::Mat1f chargeDown;
	cv::rotate(cloud.charge(), chargeDown, cv::ROTATE_90_COUNTERCLOCKWISE);
	chargeDown = cv::repeat(chargeDown, N_PARTICLES, 1);

	cv::Mat1f numerator;
	cv::multiply(chargeRight, chargeDown, numerator);
	numerator *= Constants::coulombs;

	// --- Denominator ---
	cv::Mat1f xPos = cloud.position().sliceX();
	cv::Mat1f yPos = cloud.position().sliceY();

	cv::Mat1f xPosRight = cv::repeat(xPos, 1, N_PARTICLES);
	cv::Mat1f xPosDown;
	cv::rotate(xPos, xPosDown, cv::ROTATE_90_COUNTERCLOCKWISE);
	xPosDown = cv::repeat(xPosDown, N_PARTICLES, 1);

	cv::Mat1f yPosRight = cv::repeat(yPos, 1, N_PARTICLES);
	cv::Mat1f yPosDown;
	cv::rotate(yPos, yPosDown, cv::ROTATE_90_COUNTERCLOCKWISE);
	yPosDown = cv::repeat(yPosDown, N_PARTICLES, 1);

	// TODO: maybe its backwards
	cv::Mat1f xDiff;
	cv::Mat1f yDiff;

	cv::subtract(xPosRight, xPosDown, xDiff);
	cv::subtract(yPosRight, yPosDown, yDiff);

	// --- Square the Differences ---
	cv::Mat1f xDiffSquared;
	cv::Mat1f yDiffSquared;

	cv::multiply(xDiff, xDiff, xDiffSquared);
	cv::multiply(yDiff, yDiff, yDiffSquared);

	// d^2
	cv::Mat1f distMagnitudeSquared;
	cv::add(xDiffSquared, yDiffSquared, distMagnitudeSquared);

	cv::Mat1f distMagnitude;
	cv::sqrt(distMagnitudeSquared, distMagnitude);

	// TODO: try to replace above code with this call. Try.
	//cv::Mat1f distMagnitude = cloud.position().calcDistance();

	// Force Magnitude
	cv::Mat1f forceMagnitude;
	cv::divide(numerator, distMagnitude, forceMagnitude);

	// Remove elements subject to division by 0
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

ForceCloud ForceEngine::calcSpringForce(Cloud & cloud) {
	const int N_PARTICLES = cloud.nParticles();

	cv::Mat1f distance = cloud.position().calcDistance();

	cv::Mat1f elasticForce = distance * -5.0f;

	ForceCloud force;

	return force;
}

AccelerationCloud ForceEngine::calcAcceleration(
	const ForceCloud & force,
	MassCloud & mass
) const {
	AccelerationCloud accel;

	cv::Mat massRepeated = cv::repeat(mass, 1, 2);

	cv::divide(force, massRepeated, accel);

	return accel;
}

void ForceEngine::updateAcceleration(AccelerationCloud & accel) {
	_accelerations.push_front(accel);
}

void ForceEngine::updateVelocity() {
	// --- Approximate Change in Velocity ---
	// --- for this iteration ---
	AccelerationCloud & latestAccel = _accelerations.front();

	// Get the latest velocity
	// (Store in a separate object in order to increase reference count of cv::Mat)
	const VelocityCloud & currVelocity = _cloud.velocity();

	VelocityCloud deltaVelocity;

	// Approximate integral of acceleration to get velocity
	// Use Rectangular Approximation Methods (RAM)
	float multiplier = _frameDuration;//**** * _timeScalar;
	//deltaVelocity.mat() = integralRAM(latestAccel.mat(), multiplier);
	//deltaVelocity.mat() = integralTrapezoidal(
	//	_accelerations.at(0).mat(),
	//	_accelerations.at(1).mat(),
	//	multiplier
	//);
	deltaVelocity.mat() = integralSimpsons(
		_accelerations.at(0).mat(),
		_accelerations.at(1).mat(),
		_accelerations.at(2).mat(),
		multiplier
	);

	// Use Trapezoidal Approximation
	// TODO: Trapezoidal Approximation
	// TODO: Simpsons Rule

	VelocityCloud nextVelocity;

	cv::add(deltaVelocity.mat(), currVelocity.mat(), nextVelocity);

	// Current Velocity becomes the previous one
	_velocities.push_front(currVelocity);

	// Next Velocity become the current one
	_cloud.velocity() = nextVelocity;
}

void ForceEngine::updatePosition() {
	// --- Approximate Change in Position ---
	// --- for this iteration ---
	VelocityCloud & latestVelocity = _velocities.front();

	PositionCloud deltaPosition;

	// Approximate integral of velocity to get position
	// Use Rectangular Approximation Methods (RAM)
	float multiplier = _frameDuration; //***** * _timeScalar;
	//deltaPosition.mat() = integralRAM(latestVelocity.mat(), multiplier);
	//deltaPosition.mat() = integralTrapezoidal(
	//	latestVelocity.mat(),
	//	_velocities.at(0).mat(),
	//	multiplier
	//);
	deltaPosition.mat() = integralSimpsons(
		latestVelocity.mat(),
		_velocities.at(0).mat(),
		_velocities.at(1).mat(),
		multiplier
	);

	// TODO: can we remove `.mat()`
	cv::add(_cloud.position(), deltaPosition, _cloud.position());
}
