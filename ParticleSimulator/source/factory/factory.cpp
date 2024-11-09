#include "factory.h"

#include "../globals.h"
#include "../constants.h"

using namespace std;

Particle Factory::generateParticle(const FactoryOptions & ops) {
	// Alias the global variable
	std::default_random_engine & dre = globals::dre;

	ParticleType particleType = ops._typeDist->generate(dre);

	Particle part = this->makeParticle(particleType);

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

	//generateCharge(ops, cloud);

	return cloud;
}

void Factory::generateCharge(
	const FactoryOptions & ops, 
	Cloud & cloud
) {
	// TODO: Fix this its very buggy
	// TODO: find a way to control mass without messing it up
	
//	for (size_t n = 0; n < ops.nParticles(); n++) {
//		ParticleType pt = ops._typeDist->generate(globals::dre);
//
//		float charge;
//		float mass;
//
//		if (pt == ParticleType::MASS) {
//			// No charge 
//			charge = 0.0f;
//			mass = Constants::Mass::DEFAULT;
//		}
//		else if (pt == ParticleType::PROTON) {
//			charge = Constants::Mass::PROTON;
//			mass = Constants::Mass::PROTON;
//		}
//		else if (pt == ParticleType::ELECTRON) {
//			charge = Constants::Mass::ELECTRON;
//			mass = Constants::Mass::ELECTRON;
//		}
//		else if (pt == ParticleType::NEUTRON) {
//			charge = Constants::Mass::NEUTRON;
//			mass = Constants::Mass::NEUTRON;
//		}
//		else {
//			stringstream ss;
//			ss << "Unknown ParticleType: " << pt;
//			throw std::runtime_error(ss.str().c_str());
//		}
//
//		cloud.charge().at<float>(n, 0) = charge;
//		cloud.mass().at<float>(n, 0) = mass;
//	}
}

Particle Factory::makeParticle(const ParticleType & pType) {
	if (pType == ParticleType::MASS) {
		return Particle();
	}
	return Particle();
}
