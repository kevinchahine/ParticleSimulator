#include "particle_type.h"

using namespace std;

const ParticleType::value_t ParticleType::MASS = 0;
const ParticleType::value_t ParticleType::PROTON = 1;
const ParticleType::value_t ParticleType::ELECTRON = 2;
const ParticleType::value_t ParticleType::NEUTRON = 3;
const size_t ParticleType::COUNT = 4;

const array<string, 4> ParticleType::NAME {
	"mass",
	"proton",
	"electron",
	"neutron"
};

std::ostream & operator<<(ostream & os, const ParticleType & pt) {
	if (pt.value() < ParticleType::NAME.size()) {
		os << ParticleType::NAME[pt.value()];
	}
	else {
		os << pt.value();
	}

	return os;
}

