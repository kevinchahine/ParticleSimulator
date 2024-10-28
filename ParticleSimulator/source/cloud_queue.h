#pragma once

#include "cloud/cloud.hpp"

#include "boost/circular_buffer.hpp"

///	@brief		Stores a queue of Clouds
///	@details	Clouds are inserted at the front and removed (or
///				overwritten at the back)
///				In other words element 0 is the most recent.
///				One element before is a Cloud before
///				
class CloudQueue : public boost::circular_buffer<Cloud>
{
public:
	CloudQueue() : boost::circular_buffer<Cloud>(2) {}
	CloudQueue(size_t capacity) : boost::circular_buffer<Cloud>(capacity) {}
	CloudQueue(const CloudQueue &) = default;
	CloudQueue(CloudQueue &&) noexcept = default;
	virtual ~CloudQueue() noexcept = default;
	CloudQueue & operator=(const CloudQueue &) = default;
	CloudQueue & operator=(CloudQueue &&) noexcept = default;

	Cloud & latest() { return this->back(); }
	const Cloud & latest() const { return this->back(); }
}; // class CloudQueue