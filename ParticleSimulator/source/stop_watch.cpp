#include "stop_watch.h"

#include <string>
#include <optional>

using namespace std;
using namespace std::chrono;

namespace tick
{
	void StopWatch::reset(std::chrono::nanoseconds elapsedTime)
	{
		m_elapsed = elapsedTime;

		state = STATE::PAUSED;
	}

	void StopWatch::stop()
	{
		if (state == STATE::RESUMED) {
			m_elapsed += (high_resolution_clock::now() - lastResumed);

			state = STATE::PAUSED;
		}
	}

	void StopWatch::start()
	{
		if (state == STATE::PAUSED) {
			lastResumed = high_resolution_clock::now();

			state = STATE::RESUMED;
		}
	}

	std::chrono::nanoseconds StopWatch::elapsed() const
	{
		switch (state)
		{
		case STATE::PAUSED:			return m_elapsed;
		case STATE::RESUMED:		return m_elapsed + (chrono::high_resolution_clock::now() - lastResumed);
		}
	}

	std::ostream & operator<<(std::ostream & os, const StopWatch & sw)
	{
		nanoseconds elapsed = sw.elapsed();
		hours hrs = duration_cast<hours>(elapsed);
		minutes min = duration_cast<minutes>(elapsed) - hrs;
		seconds sec = duration_cast<seconds>(elapsed) - hrs - min;

		stringstream ss;

		ss << hrs.count() << ':';

		ss << min.count() << ':';
		
		if (sec.count() < 10)
			ss << '0';
		
		ss << sec.count();

		ss << setw(10) << (sw.is_resumed() ? "running" : "stopped");

		os << ss.str();

		return os;
	}
} // namespace tick

