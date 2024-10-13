#include "globals.h"

#include <time.h>

namespace globals
{
	// !!! Make sure this gets initialized before it is used  !!!
	// !!! Make sure other static and global variables do not !!!
	// !!! attempt to access this before it is fully valid    !!!
	std::default_random_engine dre(time(0));
} // namespace globals
