#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <opencv2/opencv.hpp>

#include "cloud.hpp"

/// @brief 2-dimensional matrix
/// 
/// dim 0:	particle dimension
/// dim 1:	attribute of particle
///		columns:
///			- 0 position x
class Cloud
{
public:
	Cloud() :
		Cloud(0) {}
	Cloud(int nParticles) :
		_charge(nParticles),
		_element(nParticles),
		_mass(nParticles),
		_position(nParticles),
		_velocity(nParticles) {}
	Cloud(const Cloud &) = default;
	Cloud(Cloud &&) noexcept = default;
	virtual ~Cloud() noexcept = default;
	Cloud & operator=(const Cloud &) = default;
	Cloud & operator=(Cloud &&) noexcept = default;

	Cloud clone() const;

	void resize(size_t size);

	size_t nParticles() const { return (size_t) _position.rows; }

	ChargeCloud & charge() { return _charge; }
	ElementCloud & element() { return _element; }
	MassCloud & mass() { return _mass; }
	PositionCloud & position() { return _position; }
	VelocityCloud & velocity() { return _velocity; }

	const ChargeCloud & charge() const { return _charge; }
	const ElementCloud & element() const { return _element; }
	const MassCloud & mass() const { return _mass; }
	const PositionCloud & position() const { return _position; }
	const VelocityCloud & velocity() const { return _velocity; }

private:
	ChargeCloud _charge;
	ElementCloud _element;
	MassCloud _mass;
	PositionCloud _position;
	VelocityCloud _velocity;

public:
	// ===========================================================================
	// ========================= class Particle ==================================
	// ===========================================================================
	class Particle
	{
	public:
		Particle() = default;
		Particle(Cloud & cloud, size_t particleIndex) :
			_cloudPtr(&cloud),
			_index(particleIndex) {}
		Particle(const Particle &) = default;
		Particle(Particle &&) noexcept = default;
		virtual ~Particle() noexcept = default;
		Particle & operator=(const Particle &) = default;
		Particle & operator=(Particle &&) noexcept = default;

		bool isNull() const { return _cloudPtr == nullptr; }
		bool isNonNull() const { return !isNull(); }

		cv::Point2f position() const;
		void position(const cv::Point2f & position);

		cv::Point2f velocity() const;
		void velocity(const cv::Point2f & velocity);

		float mass() const;
		void mass(float mass);

		Cloud & cloud() { return *_cloudPtr; }
		const Cloud & cloud() const { return *_cloudPtr; }

		size_t index() const { return _index; }
		size_t & index() { return _index; }

	private:
		// Address of the Cloud which the Particle references
		Cloud * _cloudPtr = nullptr;

		// Index (row index) of the Cloud which this Particle references
		// Remember: Each row of a matrix corresponds to a single Particle.
		size_t _index = 0;
	};

	// ===========================================================================
	// ========================= class Iterator ==================================
	// ===========================================================================

	class iterator
	{
	public:
		iterator(Cloud & cloud, size_t index);
		iterator(const iterator &) = default;
		iterator(iterator &&) noexcept = default;
		~iterator() noexcept = default;
		iterator & operator=(const iterator &) = default;
		iterator & operator=(iterator &&) noexcept = default;

		Particle & operator*();
		Particle & operator->();

		iterator & operator++();
		iterator operator++(int);

		bool operator==(const iterator & other) const;
		bool operator!=(const iterator & other) const;

	protected:
		Cloud::Particle _particle;
	}; // class iterator

	class const_iterator : protected iterator
	{
	public:
		const_iterator(const Cloud * cloud, size_t index);
		const_iterator(const const_iterator &) = default;
		const_iterator(const_iterator &&) noexcept = default;
		~const_iterator() noexcept = default;
		const_iterator & operator=(const const_iterator &) = default;
		const_iterator & operator=(const_iterator &&) noexcept = default;

		const Particle & operator*();
		const Particle & operator->();

		const_iterator & operator++();
		const_iterator operator++(int);

		bool operator==(const const_iterator & other) const;
		bool operator!=(const const_iterator & other) const;
	}; // class const_iterator

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;

	friend iterator;
	friend const_iterator;
}; // class Cloud
