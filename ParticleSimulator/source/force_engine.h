#pragma once

#include "cloud_queue.h"
#include "constants.h"

class ForceEngine
{
public:
	/// @brief		Initializes the particle cloud that is saved 
	///				in this ForceEngine.
	///			
	/// @details	All operations will be applied to the saved particle cloud.
	/// 
	/// @param		cloud A Particle Cloud
	void initialize(const Cloud & cloud);

	/// @see		ForceEngine::initialize
	void initialize(Cloud && cloud);

	void setFrameDuration(float duration) { _frameDuration = duration; }

	void update();

	const Cloud & cloud() const { return _cloud; }

	/// @brief		Returns physical frame rate of simulation.
	/// @details	Frequency at which iterations of the simulation are
	///				executed.
	/// @return		frame rate in (Hz)
	float frameRate() const;

	/// @brief		Sets physical frame rate of simulation.
	/// @details	Frequency at which iterations of the simulation are
	///				executed.
	/// @param		rate frame rate in (Hz)
	void frameRate(float rate);

	/// @brief		Returns physical frame duration of simulation.
	/// @details	Duration of each iteration of the simulation.
	/// @return		Frame duration in units of seconds (Sec)
	float frameDuration() const;

	/// @brief		Sets physical frame duration of simulation.
	/// @details	Duration of each iteration of the simulation.
	/// @param		frameDuration Frame duration in units of seconds (Sec)
	void frameDuration(float frameDuration);

	/// @brief		Speeds up or slows down simulation.
	/// @details	Rate at which the simulation is speed up.
	/// @return		rate
	float timeScalar() const;

	/// @brief		Speeds up or slows down simulation.
	/// @details	Rate at which the simulation is speed up.
	/// @param		timeScalar Range must be greater than 0.0:
	///					- ts < 0.0f			- invalid
	///					- ts == 0.0f		- freezes simulation (not practical)
	///					- 0.0f < ts < 1.0f	- slows simulation
	///					- ts == 1.0f		- normal speed (universal speed)
	///					- ts > 1.0f			- speeds up simulation
	///					- ts ~=	10'000'000	- Good for gravity
	///	@return		rate
	void timeScalar(float timeScalar);

private:
	/// @brief		Calculates the force of gravity applied to each particle
	///				in the cloud. 
	/// @param		cloud A Cloud which contains mass and positions
	/// @return		A ForceCloud storing the total gravitational force applied
	///				to each particle in the cloud.
	ForceCloud calcGravitationalForce(Cloud & cloud);

	/// @brief		Calculates the force between charged particles applied to each
	///				particle in the cloud.
	/// @param		cloud A Cloud which contains mass, position and charge
	/// @return		A ForceCloud storing the total Coulomb force applied 
	///				to each particle in the cloud.
	ForceCloud calcCoulombForce(Cloud & cloud);

	/// @brief		Calculates the Spring-Mass-Damper force applied to each
	///				particle in the cloud.
	/// @param		cloud A Cloud which contains mass and element type
	/// @return		A ForceCloud
	ForceCloud calcSpringMassDamperForce(Cloud & cloud);

	ForceCloud calcSpringForce(Cloud & cloud);

	ForceCloud calcDamperForce(Cloud & cloud);

	ForceCloud calcFrictionForce(Cloud & cloud);

	AccelerationCloud calcAcceleration(
		const ForceCloud & force,
		MassCloud & mass) const;

	void updateAcceleration(AccelerationCloud & accel);

	void updateVelocity();

	void updatePosition();

private:
	// Duration of each frame in seconds
	float _frameDuration = 1.0f / 60.0f;// duration of 60Hz

	// Duration of each frame in seconds in the simulated time.
	float _timeScalar = 1.0f;

	Cloud _cloud;

	// TODO: add a queue of Clouds (circular buffer) to support
	//		trapezoidal approximations and Simpsons rule
	boost::circular_buffer<AccelerationCloud> _accelerations = 
		boost::circular_buffer<AccelerationCloud>(3);

	boost::circular_buffer<VelocityCloud> _velocities = 
		boost::circular_buffer<VelocityCloud>(2);
}; // class ForceEngine