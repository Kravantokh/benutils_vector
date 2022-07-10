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
	
	Vecn() = default;

	// Conversion constructor for promotion (converting from vector of smaller dimension to vector of greater dimension)
	// Yes, a hack had to be used to make sure this conversion is used only for promotion and not demotion. (For future me and anyone who has no idea what this is: the char(*)[n] part declares a pointer to an array of n members. If M<N, n becomes 1, otherwise n is 0, which is invalid syntax (an array of length 0 make no sense) and thus template specialization fails, leaving the other, demotion conversion constructor to take its place)
	template<unsigned int M> Vecn(Vecn<M, T> v, char(*)[M<N] = 0){
		for(unsigned int i = 1; i <= M; ++i)
			this->contents.at(i - 1) = v.contents.at(i - 1);
		for(unsigned int i = M + 1; i<= N; ++i)
			this->contents.at(i-1) = 0;
	}

	// Conversion constructor for demoting (converting from vector of larger dimension to vector of smaller dimension)
	// Same hack again for the opposite reason
	template<unsigned int M> Vecn(Vecn<M, T> v, char(*)[M>N] = 0){
		for(unsigned int i = 1; i <= N; ++i)
			this->contents.at(i - 1) = v.contents.at(i - 1);
	}
	// Assignment operator
	Vecn<N, T>& operator=(const Vecn<N, T>&) = default;
	
	// Equality operator
	bool operator==(const Vecn<N,T>& v) const{
		bool eq = 1;
		
		for(int i = 1; i <= N; ++i){
			if(this->contents.at(i-1) != v[i]){
				eq = false;
				break;
			}
		}

		return eq;
	}
	
	// Element acces operator
	T& operator[](unsigned int n){
		return contents.at(n - 1);
	}
	
	// Summation operator
	Vecn<N, T> operator+(const Vecn<N, T>& v) const{
		Vecn<N, T> ret;
		for(unsigned int i = 1; i <= N; ++i)
			ret[i] = this->contents.at(i-1) + v.contents.at(i-1);
		return ret;
	}

	//Subtraction operator
	Vecn<N, T> operator-(const Vecn<N, T>& v) const{
		Vecn<N, T> ret;
		for(unsigned int i = 1; i <= N; ++i)
			ret[i] = this->contents.at(i-1) - v.contents.at(i-1);
		return ret;
	}

	// Unary minus operator
	Vecn<N, T> operator-() const{
		Vecn<N, T> ret;
		for(unsigned int i = 1; i <= N; ++i)
			ret[i] = -this->contents.at(i-1);
		return ret;
	}

	// Dot product
	T operator*(const Vecn<N, T>& v) const{
		T ret = 0;
		for(unsigned int i = 1; i <= N; ++i)
			ret += this->contents.at(i-1) * v.contents.at(i-1);
		return ret;
	}
};

template<typename T = MATH_REAL_TYPE>
class Vec3 : public Vecn<3, T>{
public:
	// Assignment operator from specialized Vecn instead of pure Vec3
	Vec3& operator=(const Vecn<3,T>& v){
		this->contents = v.contents;
		return *this;
	}
	
	// Function for cross product.
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
