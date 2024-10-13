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

	static_cast<ChargeCloud &>(c) = this->ChargeCloud::clone();
	static_cast<ElementCloud &>(c) = this->ElementCloud::clone();
	static_cast<MassCloud &>(c) = this->MassCloud::clone();
	static_cast<PositionCloud &>(c) = this->PositionCloud::clone();
	static_cast<VelocityCloud &>(c) = this->VelocityCloud::clone();

	return c;
}

void Cloud::resize(size_t size) {
	static_cast<ChargeCloud &>(*this).resize(size);
	static_cast<ElementCloud &>(*this).resize(size);
	static_cast<MassCloud &>(*this).resize(size);
	static_cast<PositionCloud &>(*this).resize(size);
	static_cast<VelocityCloud &>(*this).resize(size);
}

// ============================================================================
// ========================== class Particle ==================================
// ============================================================================

cv::Point2f Cloud::Particle::position() const {
	const PositionCloud & pos = _cloudPtr->positionCloud();

	return pos.at(_index);}

void Cloud::Particle::position(const cv::Point2f & position) {
	PositionCloud & pos = _cloudPtr->positionCloud();
	pos.at(_index, position);}

cv::Point2f Cloud::Particle::velocity() const {
	const VelocityCloud & vel = _cloudPtr->velocityCloud();	return vel.at(_index);}

void Cloud::Particle::velocity(const cv::Point2f & velocity) {
	VelocityCloud & vel = _cloudPtr->velocityCloud();	vel.at(_index, velocity);}

float Cloud::Particle::mass() const {
	const MassCloud & mass = _cloudPtr->massCloud();	return mass.at(_index);}

void Cloud::Particle::mass(float mass) {
	MassCloud & massCloud = _cloudPtr->massCloud();	massCloud.at(_index, mass);}

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
