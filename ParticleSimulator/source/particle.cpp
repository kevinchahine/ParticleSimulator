#include "particle.h"

using namespace std;

unique_ptr<Particle> Particle::clone() {
	unique_ptr<Particle> c = make_unique<Particle>();

	c->Particle::operator=(*this);

	return c;
}
