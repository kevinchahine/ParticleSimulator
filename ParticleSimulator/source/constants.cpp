#include "constants.h"

using namespace std;

namespace Constants
{
	const float gravitational = -5000;
	const float coulombs = 8'987'551'792.3;
	const float a = -0.77;
	const float b = -3.7;
	const float c = -3.9;

	namespace Mass
	{
		extern const float DEFAULT = 1.0f;
		const float PROTON = 1.0f;
		const float ELECTRON = 5.45e-4;
		const float NEUTRON = PROTON + ELECTRON;
	} // namespace Mass
}; // class Constants