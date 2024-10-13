#include "factory.h"

#include "../globals.h"

using namespace std;

Particle Factory::generateParticle(const FactoryOptions & ops) {
	Particle part = this->makeParticle(ops._types.front());

	// Alias the global variable
	std::default_random_engine & dre = globals::dre;

	part.mass() = ops._massDist->generate(dre);
	part.position().x = ops._xPosDist->generate(dre);
	part.position().y = ops._yPosDist->generate(dre);
	part.velocity().x = ops._xVelDist->generate(dre);
	part.velocity().y = ops._yVelDist->generate(dre);

	return part;
}

Cloud Factory::generateCloud(const FactoryOptions & ops) {
	Cloud cloud;

	cloud.resize(ops.nParticles());
	
	for (size_t n = 0; n < ops.nParticles(); n++) {
		// --- 1.) Generate Random Particle ---
		Particle p = this->generateParticle(ops);
		
		// --- 2.) Put it in Cloud ---
		cloud.mass().at<float>(n, 0) = p.mass();

		cloud.position().at<float>(n, 0) = p.position().x;
		cloud.position().at<float>(n, 1) = p.position().y;

		cloud.velocity().at<float>(n, 0) = p.velocity().x;
		cloud.velocity().at<float>(n, 1) = p.velocity().y;
	}

	return cloud;
}

Particle Factory::makeParticle(const ParticleType & pType) {
	switch (pType) {
	case ParticleType::VOID:
	case ParticleType::MASS:
		return Particle();
	default:
		throw std::runtime_error("Unrecognized particle type");
	}
}
