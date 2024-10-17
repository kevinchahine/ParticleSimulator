#pragma once

#include <iostream>

#include <opencv2/core/Mat.hpp>

#include <opencv2/core.hpp>

template<typename MATRIX_T, typename DERIVED_T>
class CloudTemplate : public MATRIX_T
{
public:
	CloudTemplate(int nParticles, int nAttributes) {
		//*this = MATRIX_T::zeros(nParticles, nAttributes);
		static_cast<MATRIX_T &>(*this) = MATRIX_T::zeros(nParticles, nAttributes);
	}
	CloudTemplate(const CloudTemplate &) = default;
	CloudTemplate(CloudTemplate &&) noexcept = default;
	virtual ~CloudTemplate() noexcept = default;
	CloudTemplate & operator=(const CloudTemplate &) = default;
	CloudTemplate & operator=(CloudTemplate &&) noexcept = default;

	void resize(size_t size) { static_cast<MATRIX_T &>(*this).resize(size); }

	/// @brief		Number of particles in this cloud
	/// @details	Same as the number of rows in the matrix
	/// @return		Number of particles
	int nParticles() const { return this->rows; }//_data.rows; }

	/// @brief		Number of attributes of the quantity that this Cloud
	///				represents.
	/// @details	Same as the number of cols in the matrix
	///				For example: 
	///					- force has 2 attributes (xForce and yForce)
	///					- position has 2 attributes (xPos and yPos)
	///					- element has 1 attribute (element type)
	/// @return		Number of attributes.
	int nAttributes() const { return this->cols; } // _data.cols; }

	DERIVED_T clone() const;

	MATRIX_T & mat() { return static_cast<MATRIX_T &>(*this); }
	const MATRIX_T & mat() const { return static_cast<const MATRIX_T &>(*this); }
}; // class AccelerationCloud

template<typename MATRIX_T, typename DERIVED_T>
DERIVED_T CloudTemplate<MATRIX_T, DERIVED_T>::clone() const {
	DERIVED_T c(0);

	//c._data = this->_data.clone();
	static_cast<MATRIX_T &>(c) = static_cast<const MATRIX_T &>(*this).clone();

	return c;
}
