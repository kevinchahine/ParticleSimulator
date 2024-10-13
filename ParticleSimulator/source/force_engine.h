#pragma once

#include "cloud/cloud.hpp"

class ForceEngine
{
public:
	/// @brief		Initializes the particle cloud that is saved 
	///				in this ForceEngine.
	///			
	/// @details	All operations will be applied to the saved particle cloud.
	/// 
	/// @param		cloud A Particle Cloud
	void initializeCloud(const Cloud & cloud);

	/// @see		ForceEngine::initializeCloud
	void initializeCloud(Cloud && cloud);

	void setFrameDuration(float duration) { _frameDuration = duration; }

	void update();

	const Cloud & cloud() const { return _cloud; }

private:
	/// @brief		Calculates the force of gravity applied to each particle
	///				in the cloud. 
	/// @param		cloud A Cloud which contains mass and positions
	/// @return		A ForceCloud storing the total gravitational force applied
	///				to each particle in the cloud.
	ForceCloud calcGravitationalForce(Cloud & cloud);

	ForceCloud calcFrictionForce(Cloud & cloud);

	AccelerationCloud calcAcceleration(
		const ForceCloud & force,
		MassCloud & mass) const;

	VelocityCloud calcVelocityChange(const AccelerationCloud & accel);

	void applyVelocity(const VelocityCloud & deltaVelocity);

	PositionCloud calcPositionChange(const VelocityCloud & velocity);

	void applyPosition(const PositionCloud & deltaPosition);

private:
	// Duration of each frame in seconds
	float _frameDuration = 1 / 60.0f;// duration of 60Hz

	// TODO: add a queue of Clouds (circular buffer) to support
	//		trapezoidal approximations and Simpsons rule
	Cloud _cloud;
}; // class ForceEngine