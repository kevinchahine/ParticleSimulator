#include "cloud.h"

#pragma warning ( disable : 2398 )

using namespace std;

//Cloud::super_t::iterator Cloud::erase(Cloud::super_t::iterator & pos) {
//
//	if (this->empty()) {
//		return this->end();
//	}
//	else {
//		if (this->size() > 1) {
//			// swap with last element
//			unique_ptr<Particle> & posRef = *pos;
//			unique_ptr<Particle> & lastRef = this->back();
//
//			std::swap(posRef, lastRef);
//		}
//
//		super_t::iterator ret = ++pos;
//
//		this->pop_back();
//		
//		return ret;
//	}
//}

Cloud Cloud::clone() const {
	Cloud c;

	c._charge = this->_charge.clone();
	c._element = this->_element.clone();
	c._mass = this->_mass.clone();
	c._position = this->_position.clone();
	c._velocity = this->_velocity.clone();

	return c;
}

void Cloud::resize(size_t size) {
	_charge.resize(size);
	_element.resize(size);
	_mass.resize(size);
	_position.resize(size);
	_velocity.resize(size);
}

// ============================================================================
// ========================== class Particle ==================================
// ============================================================================

cv::Point2f Cloud::Particle::position() const {
	const PositionCloud & pos = _cloudPtr->position();

	return pos.at(_index);}

void Cloud::Particle::position(const cv::Point2f & position) {
	PositionCloud & pos = _cloudPtr->position();
	pos.at(_index, position);}

cv::Point2f Cloud::Particle::velocity() const {
	const VelocityCloud & vel = _cloudPtr->velocity();	return vel.at(_index);}

void Cloud::Particle::velocity(const cv::Point2f & velocity) {
	VelocityCloud & vel = _cloudPtr->velocity();	vel.at(_index, velocity);}

float Cloud::Particle::mass() const {
	const MassCloud & mass = _cloudPtr->mass();	return mass.at(_index);}

void Cloud::Particle::mass(float mass) {
	MassCloud & massCloud = _cloudPtr->mass();	massCloud.at(_index, mass);}

// ============================================================================
// ========================== class iterator ==================================
// ============================================================================

Cloud::iterator Cloud::begin() {
	return iterator{ *this, 0 };
}

Cloud::iterator Cloud::end() {
	return iterator{ *this, this->nParticles() };
}

Cloud::const_iterator Cloud::begin() const {
	return const_iterator{ this, 0 };
}

Cloud::const_iterator Cloud::end() const {
	return const_iterator{ this, this->nParticles() };
}

Cloud::const_iterator Cloud::cbegin() const {
	return const_iterator{ this, 0 };
}

Cloud::const_iterator Cloud::cend() const {
	return const_iterator{ this, this->nParticles() };
}

// -------------------------- ITERATOR ----------------------------------------

Cloud::iterator::iterator(Cloud & cloud, size_t index) :
	_particle(cloud, index) {}

Cloud::Particle & Cloud::iterator::operator*() {
	return _particle;
}

Cloud::Particle & Cloud::iterator::operator->() {
	return _particle;
}

Cloud::iterator & Cloud::iterator::operator++() {
	++(_particle.index());

	return *this;
}

Cloud::iterator Cloud::iterator::operator++(int) {
	iterator copy = *this;

	++(*this);

	return copy;
}

bool Cloud::iterator::operator==(const iterator & other) const {
	const Particle & thisP = this->_particle;
	const Particle & otherP = other._particle;

	return (
		(&(thisP.cloud()) == &(otherP.cloud())) &&
		(thisP.index() == otherP.index())
	);
}

bool Cloud::iterator::operator!=(const iterator & other) const {
	return !(*this == other);
}

// -------------------------- CONST ITERATOR ----------------------------------

Cloud::const_iterator::const_iterator(const Cloud * cloud, size_t index) :
	iterator(const_cast<Cloud &>(*cloud), index) {}

const Cloud::Particle & Cloud::const_iterator::operator*() {
	return _particle;
}

const Cloud::Particle & Cloud::const_iterator::operator->() {
	return _particle;
}

Cloud::const_iterator & Cloud::const_iterator::operator++() {
	++(_particle.index());

	return *this;
}

Cloud::const_iterator Cloud::const_iterator::operator++(int) {
	const_iterator copy = *this;

	++(*this);

	return copy;
}

bool Cloud::const_iterator::operator==(const const_iterator & other) const {
	return (
		static_cast<const Cloud::iterator &>(*this) ==
		static_cast<const Cloud::iterator &>(other)
	);
}

bool Cloud::const_iterator::operator!=(const const_iterator & other) const {
	return !(*this == other);
}
