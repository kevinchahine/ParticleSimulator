#pragma once

#include <iostream>
#include <opencv2/core/types.hpp>

#include "../cloud/cloud.h"
#include "distribution.h"
#include "../particle.h"

#include "factory_options.h"

class Factory
{
public:
	Particle generateParticle(const FactoryOptions & ops);

	Cloud generateCloud(const FactoryOptions & ops);

private:
	Particle makeParticle(const ParticleType & pType);

}; // class Factory
