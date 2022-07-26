#ifndef __BU_MATRIX_
#define __BU_MATRIX_
#include <array>
#include <initializer_list>
#include <stdexcept>
#include "benutils/vector.hpp"

namespace bu::math{

template<unsigned int R, unsigned int C, typename T>
class Matrix{
	public:
	std::array<std::array<T, C>, R> contents;

	// Default constructor
	Matrix() = default;

	Matrix(std::initializer_list<T> list){
		int i = 0;
		int j = 0;
		if(list.size() != R*C){
			std::string err = "Bad assignment! The brace-initializer list should contain exactly as many elements as the matrix has at ";
			throw(std::runtime_error(err) );
		}
		for(auto element: list){
			contents.at(j).at(i) = element;
			++i;
			if(i%C == 0){
				i = 0;
				++j;
			}
		}
	}
	
	// Element acces operator to be used for writing to elements or getting rows as individual reference vectors.
	Vecn<C, T>& operator[](unsigned int m){
		// Some type punning, because otherwise an array can not be cast into a vector reference. May not be the best practice, but it's simple and it works. What more could one want? 
		return *( (Vecn<C, T>*) &contents.at(m - 1) );
	}

	// Default assignment oeprator
	Matrix<R, C, T>& operator=(const Matrix<R, C, T>&) = default;
	
	bool operator==(const Matrix<R, C, T>& m) const{
		bool eq = true;
		for(unsigned int i = 1; i <= R && eq; ++i){
			for(unsigned int j = 1; j <= C && eq; ++j){
				if(contents[i - 1][j - 1] != m.contents[i -1][j - 1])
					eq = false;
			}
		}

		return eq;
	}

	bool operator!=(const Matrix<R, C, T>& m) const{
		bool eq = true;
		for(unsigned int i = 1; i <= R && eq; ++i){
			for(unsigned int j = 1; j <= C && eq; ++j){
				if(contents[i - 1][j - 1] != m.contents[i -1][j - 1])
					eq = false;
			}
		}

		return !eq;
	}
	
	// Summation operator
	Matrix<R, C, T> operator+(const Matrix<R, C, T>& m) const{
		Matrix<R, C, T> ret;
		for(unsigned int i = 1; i <= R; ++i){
			for(unsigned int j = 1; j <= C; ++j){
				ret.contents[i][j] = contents[i][j] + m.contents[i][j];
			}
		}
	}
	// Subtratcion operator
	Matrix<R, C, T> operator-(const Matrix<R, C, T>& m) const{
		Matrix<R, C, T> ret;
		for(unsigned int i = 1; i <= R; ++i){
			for(unsigned int j = 1; j <= C; ++j){
				ret.contents[i][j] = contents[i][j] - m.contents[i][j];
			}
		}
	}
	

	// Unary minus operator
	Matrix<R, C, T> operator-() const{
		Matrix<R, C, T> ret;
		for(unsigned int i = 1; i <= R; ++i){
			for(unsigned int j = 1; j <= C; ++j){
				ret.contents[i][j] = -contents[i][j];
			}
		}
	}
	
	// Multiplication operator
	template<unsigned int R2>
	Matrix<R, R2, T> operator*(const Matrix<C, R2, T>& m) const{
		Matrix<R, R2, T> ret;
		for(unsigned int i = 1; i <= R; ++i){
			for(unsigned int j = 1; j <= R2; ++j){
				T sum = 0;
				for(unsigned int k = 1; k <= C; ++k)
					sum += contents[i - 1][k - 1] * m.contents[k - 1][j - 1];
				ret[i][j] = sum;
			}
		}
		return ret;
	}
};


} // namespace bu::math

#endif
