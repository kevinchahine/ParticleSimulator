#pragma once

#include "cloud_template.h"

template<typename MATRIX_T, typename DERIVED_T>
class CloudTemplateSquare : public CloudTemplate<MATRIX_T, DERIVED_T>
{
public:
	using super_t = CloudTemplate<MATRIX_T, DERIVED_T>;

public:
	CloudTemplateSquare(int nParticles) :
		CloudTemplate<MATRIX_T, DERIVED_T>(nParticles, nParticles) {}
	CloudTemplateSquare(const CloudTemplateSquare &) = default;
	CloudTemplateSquare(CloudTemplateSquare &&) noexcept = default;
	virtual ~CloudTemplateSquare() noexcept = default;
	CloudTemplateSquare & operator=(const CloudTemplateSquare &) = default;
	CloudTemplateSquare & operator=(CloudTemplateSquare &&) noexcept = default;
}; // class CloudTemplateSquare
