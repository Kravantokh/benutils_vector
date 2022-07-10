#ifndef __BENUTILS_MATRIX__
#define __BENUTILS_MATRIX__

#include <array>

#ifndef MATH_REAL_TYPE
#define MATH_REAL_TYPE double
#endif

namespace bu::math{

template<unsigned int N, typename T = MATH_REAL_TYPE>
class Vecn{
public:
	std::array<T, N> contents;

	Vecn<N, T>& operator=(const Vecn<N, T>&) = default;

	T& operator[](unsigned int n){
		return contents.at(n - 1);
	}

	Vecn operator+(const Vecn<N, T>& v){
		
	}



};

template<typename T = MATH_REAL_TYPE>
class Vec3 : public Vecn<3, T>{
public:
	Vec3& operator=(const Vecn<3,T>& v){
		this->contents = v.contents;
		return *this;
	}

	Vecn<3, T> cross(Vecn<3, T> v){
		Vecn<3,T> V;

		V[1] = this->contents.at(2 - 1) * v[3] - this->contents.at(3 - 1) * v[2];
		V[2] = this->contents.at(3 - 1) * v[1] - this->contents.at(2 - 1) * v[3];
		V[3] = this->contents.at(1 - 1) * v[2] - this->contents.at(2 - 1) * v[1];
		
		return V;
	}
};

typedef Vecn<2, MATH_REAL_TYPE> Vec2;
typedef Vecn<4, MATH_REAL_TYPE> Vec4;


}
#endif
