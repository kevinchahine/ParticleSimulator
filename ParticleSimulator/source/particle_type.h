#pragma once

#include <stdint.h>
#include <iostream>
#include <array>

class ParticleType
{
public:
	using value_t = uint16_t;

	static const value_t MASS;
	static const value_t PROTON;
	static const value_t ELECTRON;
	static const value_t NEUTRON;
	static const size_t COUNT;

	static const std::array<std::string, 4> NAME;

	ParticleType() = default;
	ParticleType(value_t value) : _value(value) {}
	ParticleType(const ParticleType &) = default;
	ParticleType(ParticleType &&) noexcept = default;
	virtual ~ParticleType() noexcept = default;
	ParticleType & operator=(value_t value) { this->_value = value; return *this; }
	ParticleType & operator=(const ParticleType &) = default;
	ParticleType & operator=(ParticleType &&) noexcept = default;

	bool operator==(const ParticleType & pt) const { return this->_value == pt._value; }
	bool operator!=(const ParticleType & pt) const { return this->_value != pt._value; }

	value_t & value() { return _value; }
	const value_t & value() const { return _value; }

	friend std::ostream & operator<<(std::ostream & os, const ParticleType & pt);

private:
	value_t _value = MASS;
}; // class ParticleType
