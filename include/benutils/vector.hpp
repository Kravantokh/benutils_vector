#ifndef __BENUTILS_MATRIX__
#define __BENUTILS_MATRIX__

#include <array>
#include <exception>
#include <string>
#include <initializer_list>

#ifndef THROW_ON_ZERO_NORM
#define THROW_ON_ZERO_NORM 0
#endif

#ifndef MATH_REAL_TYPE
#define MATH_REAL_TYPE double
#endif

#ifndef MATH_GENERAL_SQRT
#include <math.h>
#define MATH_GENERAL_SQRT(x) sqrt( (x) )
#endif


#define SWIZZLE_GEN_1(A, B)\
Vecn<1, T> A () {\
	Vecn<1, T> ret;\
	ret.contents[0] = this->contents[(B - 1)];\
	static_assert(N >= B, "Bad swizzling! You are trying to acces an inexistent member! (Index out of range.)");\
	return ret;\
}


#define SWIZZLE_GEN_2(A, B, C, D)\
Vecn<2, T> A##B () {\
	Vecn<2, T> ret;\
	ret.contents[0] = this->contents[(C)];\
	ret.contents[1] = this->contents[(D)];\
	static_assert(N >= C || N >= D, "Bad swizzling! You are trying to acces an inexistent member! (Index out of range.)");\
	return ret;\
}

#define SWIZZLE_GEN_3(A, B, C, D, E, F)\
Vecn<3, T> A##B##C () {\
	Vecn<3, T> ret;\
	ret.contents[0] = this->contents[(D)];\
	ret.contents[1] = this->contents[(E)];\
	ret.contents[2] = this->contents[(F)];\
	static_assert(N >= D || N >= E || N >= F, "Bad swizzling! You are trying to acces an inexistent member! (Index out of range.)");\
	return ret;\
}

#define SWIZZLE_GEN_4(A, B, C, D, E, F, G, H)\
Vecn<4, T> A##B##C##D () {\
	Vecn<4, T> ret;\
	ret.contents[0] = this->contents[(E)];\
	ret.contents[1] = this->contents[(F)];\
	ret.contents[2] = this->contents[(G)];\
	ret.contents[4] = this->contents[(H)];\
	static_assert(N >= E || N >= F || N >= G || N >= H, "Bad swizzling! You are trying to acces an inexistent member! (Index out of range.)");\
	return ret;\
}
	
namespace bu::math{
// forward declaration of the Vecn class. Due to its lengthy code it is best reserved for the last place in this file.
template<unsigned int N, typename T = MATH_REAL_TYPE> class Vecn;

// 3D vectors need cross product, thus they must a separate class, but they should also retain the properties and operations of Vecn, thus they inherit from it.
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

// Defininf Vec2 and Vec4 for ease of use
typedef Vecn<2, MATH_REAL_TYPE> Vec2;
typedef Vecn<4, MATH_REAL_TYPE> Vec4;

// The main Vecn class
template<unsigned int N, typename T>
class Vecn{
public:
	std::array<T, N> contents;
	
	Vecn() = default;
	
	Vecn(std::initializer_list<T> list){
		int i = 1;
		for (auto element: list){
			contents.at(i - 1) = element;
			++i;
		}
	}

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
	template<unsigned int M> explicit Vecn(Vecn<M, T> v, char(*)[M>N] = 0){
		for(unsigned int i = 1; i <= N; ++i)
			this->contents.at(i - 1) = v.contents.at(i - 1);
	}

	// Assignment operator
	Vecn<N, T>& operator=(const Vecn<N, T>&) = default;
	
	// Equality operator
	bool operator==(const Vecn<N,T>& v) const{
		bool eq = 1;
		for(int i = 1; i <= N && eq; ++i){
			if(this->contents[i-1] != v[i])
				eq = false;
		}

		return eq;
	}
	
	bool operator!=(const Vecn<N,T>& v) const{
		bool eq = 1;
		for(int i = 1; i <= N && eq; ++i){
			if(this->contents[i-1] != v[i])
				eq = false;
		}

		return !eq;
	}
	
	// Element acces operator
	T& operator[](unsigned int n) {
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


	// Norm
	T norm(){
		T sum = 0;
		for(unsigned int i = 1; i<= N; ++i)
			sum += contents.at(i - 1) * contents.at(i - 1);
		return MATH_GENERAL_SQRT(sum);
	}
	
	// Squared norm (to save performance on the square root)
	T squared_norm(){
		T sum = 0;
		for(unsigned int i = 1; i<= N; ++i)
			sum += contents.at(i - 1) * contents.at(i - 1);
		return sum;
	}

	void normalize(){
		T norm = this->norm();
		if(norm == 0)
		#if THROW_ON_ZERO_NORM
			throw(std::logic_error("Tried to normalize a vector of length 0!"));
		#else
			return;
		#endif
		else{
			for(unsigned int i = 1; i <= N;++i)
				contents.at(i - 1) /= norm;
		}
		return;
	}
	
	std::string to_str(){
		std::string ret = "(";
		ret.reserve(3*N);	
		for(unsigned int i = 1; i < N; ++i)
			ret += std::to_string(contents.at(i - 1)) + ", ";
		
		ret += std::to_string(contents.at(N - 1)) + ")"; 
		
		return ret;
	}


// Generated functions for accesing single elements
SWIZZLE_GEN_1(x, 0)
SWIZZLE_GEN_1(r, 0)
SWIZZLE_GEN_1(y, 1)
SWIZZLE_GEN_1(g, 1)
SWIZZLE_GEN_1(z, 2)
SWIZZLE_GEN_1(b, 2)
SWIZZLE_GEN_1(w, 3)
SWIZZLE_GEN_1(a, 3)

// Generated functions for 2 element swizzling
SWIZZLE_GEN_2(x, x, 0, 0)
SWIZZLE_GEN_2(r, r, 0, 0)
SWIZZLE_GEN_2(x, y, 0, 1)
SWIZZLE_GEN_2(r, g, 0, 1)
SWIZZLE_GEN_2(x, z, 0, 2)
SWIZZLE_GEN_2(r, b, 0, 2)
SWIZZLE_GEN_2(x, w, 0, 3)
SWIZZLE_GEN_2(r, a, 0, 3)
SWIZZLE_GEN_2(y, x, 1, 0)
SWIZZLE_GEN_2(g, r, 1, 0)
SWIZZLE_GEN_2(y, y, 1, 1)
SWIZZLE_GEN_2(g, g, 1, 1)
SWIZZLE_GEN_2(y, z, 1, 2)
SWIZZLE_GEN_2(g, b, 1, 2)
SWIZZLE_GEN_2(y, w, 1, 3)
SWIZZLE_GEN_2(g, a, 1, 3)
SWIZZLE_GEN_2(z, x, 2, 0)
SWIZZLE_GEN_2(b, r, 2, 0)
SWIZZLE_GEN_2(z, y, 2, 1)
SWIZZLE_GEN_2(b, g, 2, 1)
SWIZZLE_GEN_2(z, z, 2, 2)
SWIZZLE_GEN_2(b, b, 2, 2)
SWIZZLE_GEN_2(z, w, 2, 3)
SWIZZLE_GEN_2(b, a, 2, 3)
SWIZZLE_GEN_2(w, x, 3, 0)
SWIZZLE_GEN_2(a, r, 3, 0)
SWIZZLE_GEN_2(w, y, 3, 1)
SWIZZLE_GEN_2(a, g, 3, 1)
SWIZZLE_GEN_2(w, z, 3, 2)
SWIZZLE_GEN_2(a, b, 3, 2)
SWIZZLE_GEN_2(w, w, 3, 3)
SWIZZLE_GEN_2(a, a, 3, 3)

// Generated functions for 3 element swizzling
SWIZZLE_GEN_3(x, x, x, 0, 0, 0)
SWIZZLE_GEN_3(r, r, r, 0, 0, 0)
SWIZZLE_GEN_3(x, x, y, 0, 0, 1)
SWIZZLE_GEN_3(r, r, g, 0, 0, 1)
SWIZZLE_GEN_3(x, x, z, 0, 0, 2)
SWIZZLE_GEN_3(r, r, b, 0, 0, 2)
SWIZZLE_GEN_3(x, x, w, 0, 0, 3)
SWIZZLE_GEN_3(r, r, a, 0, 0, 3)
SWIZZLE_GEN_3(x, y, x, 0, 1, 0)
SWIZZLE_GEN_3(r, g, r, 0, 1, 0)
SWIZZLE_GEN_3(x, y, y, 0, 1, 1)
SWIZZLE_GEN_3(r, g, g, 0, 1, 1)
SWIZZLE_GEN_3(x, y, z, 0, 1, 2)
SWIZZLE_GEN_3(r, g, b, 0, 1, 2)
SWIZZLE_GEN_3(x, y, w, 0, 1, 3)
SWIZZLE_GEN_3(r, g, a, 0, 1, 3)
SWIZZLE_GEN_3(x, z, x, 0, 2, 0)
SWIZZLE_GEN_3(r, b, r, 0, 2, 0)
SWIZZLE_GEN_3(x, z, y, 0, 2, 1)
SWIZZLE_GEN_3(r, b, g, 0, 2, 1)
SWIZZLE_GEN_3(x, z, z, 0, 2, 2)
SWIZZLE_GEN_3(r, b, b, 0, 2, 2)
SWIZZLE_GEN_3(x, z, w, 0, 2, 3)
SWIZZLE_GEN_3(r, b, a, 0, 2, 3)
SWIZZLE_GEN_3(x, w, x, 0, 3, 0)
SWIZZLE_GEN_3(r, a, r, 0, 3, 0)
SWIZZLE_GEN_3(x, w, y, 0, 3, 1)
SWIZZLE_GEN_3(r, a, g, 0, 3, 1)
SWIZZLE_GEN_3(x, w, z, 0, 3, 2)
SWIZZLE_GEN_3(r, a, b, 0, 3, 2)
SWIZZLE_GEN_3(x, w, w, 0, 3, 3)
SWIZZLE_GEN_3(r, a, a, 0, 3, 3)
SWIZZLE_GEN_3(y, x, x, 1, 0, 0)
SWIZZLE_GEN_3(g, r, r, 1, 0, 0)
SWIZZLE_GEN_3(y, x, y, 1, 0, 1)
SWIZZLE_GEN_3(g, r, g, 1, 0, 1)
SWIZZLE_GEN_3(y, x, z, 1, 0, 2)
SWIZZLE_GEN_3(g, r, b, 1, 0, 2)
SWIZZLE_GEN_3(y, x, w, 1, 0, 3)
SWIZZLE_GEN_3(g, r, a, 1, 0, 3)
SWIZZLE_GEN_3(y, y, x, 1, 1, 0)
SWIZZLE_GEN_3(g, g, r, 1, 1, 0)
SWIZZLE_GEN_3(y, y, y, 1, 1, 1)
SWIZZLE_GEN_3(g, g, g, 1, 1, 1)
SWIZZLE_GEN_3(y, y, z, 1, 1, 2)
SWIZZLE_GEN_3(g, g, b, 1, 1, 2)
SWIZZLE_GEN_3(y, y, w, 1, 1, 3)
SWIZZLE_GEN_3(g, g, a, 1, 1, 3)
SWIZZLE_GEN_3(y, z, x, 1, 2, 0)
SWIZZLE_GEN_3(g, b, r, 1, 2, 0)
SWIZZLE_GEN_3(y, z, y, 1, 2, 1)
SWIZZLE_GEN_3(g, b, g, 1, 2, 1)
SWIZZLE_GEN_3(y, z, z, 1, 2, 2)
SWIZZLE_GEN_3(g, b, b, 1, 2, 2)
SWIZZLE_GEN_3(y, z, w, 1, 2, 3)
SWIZZLE_GEN_3(g, b, a, 1, 2, 3)
SWIZZLE_GEN_3(y, w, x, 1, 3, 0)
SWIZZLE_GEN_3(g, a, r, 1, 3, 0)
SWIZZLE_GEN_3(y, w, y, 1, 3, 1)
SWIZZLE_GEN_3(g, a, g, 1, 3, 1)
SWIZZLE_GEN_3(y, w, z, 1, 3, 2)
SWIZZLE_GEN_3(g, a, b, 1, 3, 2)
SWIZZLE_GEN_3(y, w, w, 1, 3, 3)
SWIZZLE_GEN_3(g, a, a, 1, 3, 3)
SWIZZLE_GEN_3(z, x, x, 2, 0, 0)
SWIZZLE_GEN_3(b, r, r, 2, 0, 0)
SWIZZLE_GEN_3(z, x, y, 2, 0, 1)
SWIZZLE_GEN_3(b, r, g, 2, 0, 1)
SWIZZLE_GEN_3(z, x, z, 2, 0, 2)
SWIZZLE_GEN_3(b, r, b, 2, 0, 2)
SWIZZLE_GEN_3(z, x, w, 2, 0, 3)
SWIZZLE_GEN_3(b, r, a, 2, 0, 3)
SWIZZLE_GEN_3(z, y, x, 2, 1, 0)
SWIZZLE_GEN_3(b, g, r, 2, 1, 0)
SWIZZLE_GEN_3(z, y, y, 2, 1, 1)
SWIZZLE_GEN_3(b, g, g, 2, 1, 1)
SWIZZLE_GEN_3(z, y, z, 2, 1, 2)
SWIZZLE_GEN_3(b, g, b, 2, 1, 2)
SWIZZLE_GEN_3(z, y, w, 2, 1, 3)
SWIZZLE_GEN_3(b, g, a, 2, 1, 3)
SWIZZLE_GEN_3(z, z, x, 2, 2, 0)
SWIZZLE_GEN_3(b, b, r, 2, 2, 0)
SWIZZLE_GEN_3(z, z, y, 2, 2, 1)
SWIZZLE_GEN_3(b, b, g, 2, 2, 1)
SWIZZLE_GEN_3(z, z, z, 2, 2, 2)
SWIZZLE_GEN_3(b, b, b, 2, 2, 2)
SWIZZLE_GEN_3(z, z, w, 2, 2, 3)
SWIZZLE_GEN_3(b, b, a, 2, 2, 3)
SWIZZLE_GEN_3(z, w, x, 2, 3, 0)
SWIZZLE_GEN_3(b, a, r, 2, 3, 0)
SWIZZLE_GEN_3(z, w, y, 2, 3, 1)
SWIZZLE_GEN_3(b, a, g, 2, 3, 1)
SWIZZLE_GEN_3(z, w, z, 2, 3, 2)
SWIZZLE_GEN_3(b, a, b, 2, 3, 2)
SWIZZLE_GEN_3(z, w, w, 2, 3, 3)
SWIZZLE_GEN_3(b, a, a, 2, 3, 3)
SWIZZLE_GEN_3(w, x, x, 3, 0, 0)
SWIZZLE_GEN_3(a, r, r, 3, 0, 0)
SWIZZLE_GEN_3(w, x, y, 3, 0, 1)
SWIZZLE_GEN_3(a, r, g, 3, 0, 1)
SWIZZLE_GEN_3(w, x, z, 3, 0, 2)
SWIZZLE_GEN_3(a, r, b, 3, 0, 2)
SWIZZLE_GEN_3(w, x, w, 3, 0, 3)
SWIZZLE_GEN_3(a, r, a, 3, 0, 3)
SWIZZLE_GEN_3(w, y, x, 3, 1, 0)
SWIZZLE_GEN_3(a, g, r, 3, 1, 0)
SWIZZLE_GEN_3(w, y, y, 3, 1, 1)
SWIZZLE_GEN_3(a, g, g, 3, 1, 1)
SWIZZLE_GEN_3(w, y, z, 3, 1, 2)
SWIZZLE_GEN_3(a, g, b, 3, 1, 2)
SWIZZLE_GEN_3(w, y, w, 3, 1, 3)
SWIZZLE_GEN_3(a, g, a, 3, 1, 3)
SWIZZLE_GEN_3(w, z, x, 3, 2, 0)
SWIZZLE_GEN_3(a, b, r, 3, 2, 0)
SWIZZLE_GEN_3(w, z, y, 3, 2, 1)
SWIZZLE_GEN_3(a, b, g, 3, 2, 1)
SWIZZLE_GEN_3(w, z, z, 3, 2, 2)
SWIZZLE_GEN_3(a, b, b, 3, 2, 2)
SWIZZLE_GEN_3(w, z, w, 3, 2, 3)
SWIZZLE_GEN_3(a, b, a, 3, 2, 3)
SWIZZLE_GEN_3(w, w, x, 3, 3, 0)
SWIZZLE_GEN_3(a, a, r, 3, 3, 0)
SWIZZLE_GEN_3(w, w, y, 3, 3, 1)
SWIZZLE_GEN_3(a, a, g, 3, 3, 1)
SWIZZLE_GEN_3(w, w, z, 3, 3, 2)
SWIZZLE_GEN_3(a, a, b, 3, 3, 2)
SWIZZLE_GEN_3(w, w, w, 3, 3, 3)
SWIZZLE_GEN_3(a, a, a, 3, 3, 3)

// Generated functions for 4 element swizzling
SWIZZLE_GEN_4(x, x, x, x, 0, 0, 0, 0)
SWIZZLE_GEN_4(r, r, r, x, 0, 0, 0, 0)
SWIZZLE_GEN_4(x, x, x, y, 0, 0, 0, 1)
SWIZZLE_GEN_4(r, r, r, y, 0, 0, 0, 1)
SWIZZLE_GEN_4(x, x, x, z, 0, 0, 0, 2)
SWIZZLE_GEN_4(r, r, r, z, 0, 0, 0, 2)
SWIZZLE_GEN_4(x, x, x, w, 0, 0, 0, 3)
SWIZZLE_GEN_4(r, r, r, w, 0, 0, 0, 3)
SWIZZLE_GEN_4(x, x, y, x, 0, 0, 1, 0)
SWIZZLE_GEN_4(r, r, g, x, 0, 0, 1, 0)
SWIZZLE_GEN_4(x, x, y, y, 0, 0, 1, 1)
SWIZZLE_GEN_4(r, r, g, y, 0, 0, 1, 1)
SWIZZLE_GEN_4(x, x, y, z, 0, 0, 1, 2)
SWIZZLE_GEN_4(r, r, g, z, 0, 0, 1, 2)
SWIZZLE_GEN_4(x, x, y, w, 0, 0, 1, 3)
SWIZZLE_GEN_4(r, r, g, w, 0, 0, 1, 3)
SWIZZLE_GEN_4(x, x, z, x, 0, 0, 2, 0)
SWIZZLE_GEN_4(r, r, b, x, 0, 0, 2, 0)
SWIZZLE_GEN_4(x, x, z, y, 0, 0, 2, 1)
SWIZZLE_GEN_4(r, r, b, y, 0, 0, 2, 1)
SWIZZLE_GEN_4(x, x, z, z, 0, 0, 2, 2)
SWIZZLE_GEN_4(r, r, b, z, 0, 0, 2, 2)
SWIZZLE_GEN_4(x, x, z, w, 0, 0, 2, 3)
SWIZZLE_GEN_4(r, r, b, w, 0, 0, 2, 3)
SWIZZLE_GEN_4(x, x, w, x, 0, 0, 3, 0)
SWIZZLE_GEN_4(r, r, a, x, 0, 0, 3, 0)
SWIZZLE_GEN_4(x, x, w, y, 0, 0, 3, 1)
SWIZZLE_GEN_4(r, r, a, y, 0, 0, 3, 1)
SWIZZLE_GEN_4(x, x, w, z, 0, 0, 3, 2)
SWIZZLE_GEN_4(r, r, a, z, 0, 0, 3, 2)
SWIZZLE_GEN_4(x, x, w, w, 0, 0, 3, 3)
SWIZZLE_GEN_4(r, r, a, w, 0, 0, 3, 3)
SWIZZLE_GEN_4(x, y, x, x, 0, 1, 0, 0)
SWIZZLE_GEN_4(r, g, r, x, 0, 1, 0, 0)
SWIZZLE_GEN_4(x, y, x, y, 0, 1, 0, 1)
SWIZZLE_GEN_4(r, g, r, y, 0, 1, 0, 1)
SWIZZLE_GEN_4(x, y, x, z, 0, 1, 0, 2)
SWIZZLE_GEN_4(r, g, r, z, 0, 1, 0, 2)
SWIZZLE_GEN_4(x, y, x, w, 0, 1, 0, 3)
SWIZZLE_GEN_4(r, g, r, w, 0, 1, 0, 3)
SWIZZLE_GEN_4(x, y, y, x, 0, 1, 1, 0)
SWIZZLE_GEN_4(r, g, g, x, 0, 1, 1, 0)
SWIZZLE_GEN_4(x, y, y, y, 0, 1, 1, 1)
SWIZZLE_GEN_4(r, g, g, y, 0, 1, 1, 1)
SWIZZLE_GEN_4(x, y, y, z, 0, 1, 1, 2)
SWIZZLE_GEN_4(r, g, g, z, 0, 1, 1, 2)
SWIZZLE_GEN_4(x, y, y, w, 0, 1, 1, 3)
SWIZZLE_GEN_4(r, g, g, w, 0, 1, 1, 3)
SWIZZLE_GEN_4(x, y, z, x, 0, 1, 2, 0)
SWIZZLE_GEN_4(r, g, b, x, 0, 1, 2, 0)
SWIZZLE_GEN_4(x, y, z, y, 0, 1, 2, 1)
SWIZZLE_GEN_4(r, g, b, y, 0, 1, 2, 1)
SWIZZLE_GEN_4(x, y, z, z, 0, 1, 2, 2)
SWIZZLE_GEN_4(r, g, b, z, 0, 1, 2, 2)
SWIZZLE_GEN_4(x, y, z, w, 0, 1, 2, 3)
SWIZZLE_GEN_4(r, g, b, w, 0, 1, 2, 3)
SWIZZLE_GEN_4(x, y, w, x, 0, 1, 3, 0)
SWIZZLE_GEN_4(r, g, a, x, 0, 1, 3, 0)
SWIZZLE_GEN_4(x, y, w, y, 0, 1, 3, 1)
SWIZZLE_GEN_4(r, g, a, y, 0, 1, 3, 1)
SWIZZLE_GEN_4(x, y, w, z, 0, 1, 3, 2)
SWIZZLE_GEN_4(r, g, a, z, 0, 1, 3, 2)
SWIZZLE_GEN_4(x, y, w, w, 0, 1, 3, 3)
SWIZZLE_GEN_4(r, g, a, w, 0, 1, 3, 3)
SWIZZLE_GEN_4(x, z, x, x, 0, 2, 0, 0)
SWIZZLE_GEN_4(r, b, r, x, 0, 2, 0, 0)
SWIZZLE_GEN_4(x, z, x, y, 0, 2, 0, 1)
SWIZZLE_GEN_4(r, b, r, y, 0, 2, 0, 1)
SWIZZLE_GEN_4(x, z, x, z, 0, 2, 0, 2)
SWIZZLE_GEN_4(r, b, r, z, 0, 2, 0, 2)
SWIZZLE_GEN_4(x, z, x, w, 0, 2, 0, 3)
SWIZZLE_GEN_4(r, b, r, w, 0, 2, 0, 3)
SWIZZLE_GEN_4(x, z, y, x, 0, 2, 1, 0)
SWIZZLE_GEN_4(r, b, g, x, 0, 2, 1, 0)
SWIZZLE_GEN_4(x, z, y, y, 0, 2, 1, 1)
SWIZZLE_GEN_4(r, b, g, y, 0, 2, 1, 1)
SWIZZLE_GEN_4(x, z, y, z, 0, 2, 1, 2)
SWIZZLE_GEN_4(r, b, g, z, 0, 2, 1, 2)
SWIZZLE_GEN_4(x, z, y, w, 0, 2, 1, 3)
SWIZZLE_GEN_4(r, b, g, w, 0, 2, 1, 3)
SWIZZLE_GEN_4(x, z, z, x, 0, 2, 2, 0)
SWIZZLE_GEN_4(r, b, b, x, 0, 2, 2, 0)
SWIZZLE_GEN_4(x, z, z, y, 0, 2, 2, 1)
SWIZZLE_GEN_4(r, b, b, y, 0, 2, 2, 1)
SWIZZLE_GEN_4(x, z, z, z, 0, 2, 2, 2)
SWIZZLE_GEN_4(r, b, b, z, 0, 2, 2, 2)
SWIZZLE_GEN_4(x, z, z, w, 0, 2, 2, 3)
SWIZZLE_GEN_4(r, b, b, w, 0, 2, 2, 3)
SWIZZLE_GEN_4(x, z, w, x, 0, 2, 3, 0)
SWIZZLE_GEN_4(r, b, a, x, 0, 2, 3, 0)
SWIZZLE_GEN_4(x, z, w, y, 0, 2, 3, 1)
SWIZZLE_GEN_4(r, b, a, y, 0, 2, 3, 1)
SWIZZLE_GEN_4(x, z, w, z, 0, 2, 3, 2)
SWIZZLE_GEN_4(r, b, a, z, 0, 2, 3, 2)
SWIZZLE_GEN_4(x, z, w, w, 0, 2, 3, 3)
SWIZZLE_GEN_4(r, b, a, w, 0, 2, 3, 3)
SWIZZLE_GEN_4(x, w, x, x, 0, 3, 0, 0)
SWIZZLE_GEN_4(r, a, r, x, 0, 3, 0, 0)
SWIZZLE_GEN_4(x, w, x, y, 0, 3, 0, 1)
SWIZZLE_GEN_4(r, a, r, y, 0, 3, 0, 1)
SWIZZLE_GEN_4(x, w, x, z, 0, 3, 0, 2)
SWIZZLE_GEN_4(r, a, r, z, 0, 3, 0, 2)
SWIZZLE_GEN_4(x, w, x, w, 0, 3, 0, 3)
SWIZZLE_GEN_4(r, a, r, w, 0, 3, 0, 3)
SWIZZLE_GEN_4(x, w, y, x, 0, 3, 1, 0)
SWIZZLE_GEN_4(r, a, g, x, 0, 3, 1, 0)
SWIZZLE_GEN_4(x, w, y, y, 0, 3, 1, 1)
SWIZZLE_GEN_4(r, a, g, y, 0, 3, 1, 1)
SWIZZLE_GEN_4(x, w, y, z, 0, 3, 1, 2)
SWIZZLE_GEN_4(r, a, g, z, 0, 3, 1, 2)
SWIZZLE_GEN_4(x, w, y, w, 0, 3, 1, 3)
SWIZZLE_GEN_4(r, a, g, w, 0, 3, 1, 3)
SWIZZLE_GEN_4(x, w, z, x, 0, 3, 2, 0)
SWIZZLE_GEN_4(r, a, b, x, 0, 3, 2, 0)
SWIZZLE_GEN_4(x, w, z, y, 0, 3, 2, 1)
SWIZZLE_GEN_4(r, a, b, y, 0, 3, 2, 1)
SWIZZLE_GEN_4(x, w, z, z, 0, 3, 2, 2)
SWIZZLE_GEN_4(r, a, b, z, 0, 3, 2, 2)
SWIZZLE_GEN_4(x, w, z, w, 0, 3, 2, 3)
SWIZZLE_GEN_4(r, a, b, w, 0, 3, 2, 3)
SWIZZLE_GEN_4(x, w, w, x, 0, 3, 3, 0)
SWIZZLE_GEN_4(r, a, a, x, 0, 3, 3, 0)
SWIZZLE_GEN_4(x, w, w, y, 0, 3, 3, 1)
SWIZZLE_GEN_4(r, a, a, y, 0, 3, 3, 1)
SWIZZLE_GEN_4(x, w, w, z, 0, 3, 3, 2)
SWIZZLE_GEN_4(r, a, a, z, 0, 3, 3, 2)
SWIZZLE_GEN_4(x, w, w, w, 0, 3, 3, 3)
SWIZZLE_GEN_4(r, a, a, w, 0, 3, 3, 3)
SWIZZLE_GEN_4(y, x, x, x, 1, 0, 0, 0)
SWIZZLE_GEN_4(g, r, r, x, 1, 0, 0, 0)
SWIZZLE_GEN_4(y, x, x, y, 1, 0, 0, 1)
SWIZZLE_GEN_4(g, r, r, y, 1, 0, 0, 1)
SWIZZLE_GEN_4(y, x, x, z, 1, 0, 0, 2)
SWIZZLE_GEN_4(g, r, r, z, 1, 0, 0, 2)
SWIZZLE_GEN_4(y, x, x, w, 1, 0, 0, 3)
SWIZZLE_GEN_4(g, r, r, w, 1, 0, 0, 3)
SWIZZLE_GEN_4(y, x, y, x, 1, 0, 1, 0)
SWIZZLE_GEN_4(g, r, g, x, 1, 0, 1, 0)
SWIZZLE_GEN_4(y, x, y, y, 1, 0, 1, 1)
SWIZZLE_GEN_4(g, r, g, y, 1, 0, 1, 1)
SWIZZLE_GEN_4(y, x, y, z, 1, 0, 1, 2)
SWIZZLE_GEN_4(g, r, g, z, 1, 0, 1, 2)
SWIZZLE_GEN_4(y, x, y, w, 1, 0, 1, 3)
SWIZZLE_GEN_4(g, r, g, w, 1, 0, 1, 3)
SWIZZLE_GEN_4(y, x, z, x, 1, 0, 2, 0)
SWIZZLE_GEN_4(g, r, b, x, 1, 0, 2, 0)
SWIZZLE_GEN_4(y, x, z, y, 1, 0, 2, 1)
SWIZZLE_GEN_4(g, r, b, y, 1, 0, 2, 1)
SWIZZLE_GEN_4(y, x, z, z, 1, 0, 2, 2)
SWIZZLE_GEN_4(g, r, b, z, 1, 0, 2, 2)
SWIZZLE_GEN_4(y, x, z, w, 1, 0, 2, 3)
SWIZZLE_GEN_4(g, r, b, w, 1, 0, 2, 3)
SWIZZLE_GEN_4(y, x, w, x, 1, 0, 3, 0)
SWIZZLE_GEN_4(g, r, a, x, 1, 0, 3, 0)
SWIZZLE_GEN_4(y, x, w, y, 1, 0, 3, 1)
SWIZZLE_GEN_4(g, r, a, y, 1, 0, 3, 1)
SWIZZLE_GEN_4(y, x, w, z, 1, 0, 3, 2)
SWIZZLE_GEN_4(g, r, a, z, 1, 0, 3, 2)
SWIZZLE_GEN_4(y, x, w, w, 1, 0, 3, 3)
SWIZZLE_GEN_4(g, r, a, w, 1, 0, 3, 3)
SWIZZLE_GEN_4(y, y, x, x, 1, 1, 0, 0)
SWIZZLE_GEN_4(g, g, r, x, 1, 1, 0, 0)
SWIZZLE_GEN_4(y, y, x, y, 1, 1, 0, 1)
SWIZZLE_GEN_4(g, g, r, y, 1, 1, 0, 1)
SWIZZLE_GEN_4(y, y, x, z, 1, 1, 0, 2)
SWIZZLE_GEN_4(g, g, r, z, 1, 1, 0, 2)
SWIZZLE_GEN_4(y, y, x, w, 1, 1, 0, 3)
SWIZZLE_GEN_4(g, g, r, w, 1, 1, 0, 3)
SWIZZLE_GEN_4(y, y, y, x, 1, 1, 1, 0)
SWIZZLE_GEN_4(g, g, g, x, 1, 1, 1, 0)
SWIZZLE_GEN_4(y, y, y, y, 1, 1, 1, 1)
SWIZZLE_GEN_4(g, g, g, y, 1, 1, 1, 1)
SWIZZLE_GEN_4(y, y, y, z, 1, 1, 1, 2)
SWIZZLE_GEN_4(g, g, g, z, 1, 1, 1, 2)
SWIZZLE_GEN_4(y, y, y, w, 1, 1, 1, 3)
SWIZZLE_GEN_4(g, g, g, w, 1, 1, 1, 3)
SWIZZLE_GEN_4(y, y, z, x, 1, 1, 2, 0)
SWIZZLE_GEN_4(g, g, b, x, 1, 1, 2, 0)
SWIZZLE_GEN_4(y, y, z, y, 1, 1, 2, 1)
SWIZZLE_GEN_4(g, g, b, y, 1, 1, 2, 1)
SWIZZLE_GEN_4(y, y, z, z, 1, 1, 2, 2)
SWIZZLE_GEN_4(g, g, b, z, 1, 1, 2, 2)
SWIZZLE_GEN_4(y, y, z, w, 1, 1, 2, 3)
SWIZZLE_GEN_4(g, g, b, w, 1, 1, 2, 3)
SWIZZLE_GEN_4(y, y, w, x, 1, 1, 3, 0)
SWIZZLE_GEN_4(g, g, a, x, 1, 1, 3, 0)
SWIZZLE_GEN_4(y, y, w, y, 1, 1, 3, 1)
SWIZZLE_GEN_4(g, g, a, y, 1, 1, 3, 1)
SWIZZLE_GEN_4(y, y, w, z, 1, 1, 3, 2)
SWIZZLE_GEN_4(g, g, a, z, 1, 1, 3, 2)
SWIZZLE_GEN_4(y, y, w, w, 1, 1, 3, 3)
SWIZZLE_GEN_4(g, g, a, w, 1, 1, 3, 3)
SWIZZLE_GEN_4(y, z, x, x, 1, 2, 0, 0)
SWIZZLE_GEN_4(g, b, r, x, 1, 2, 0, 0)
SWIZZLE_GEN_4(y, z, x, y, 1, 2, 0, 1)
SWIZZLE_GEN_4(g, b, r, y, 1, 2, 0, 1)
SWIZZLE_GEN_4(y, z, x, z, 1, 2, 0, 2)
SWIZZLE_GEN_4(g, b, r, z, 1, 2, 0, 2)
SWIZZLE_GEN_4(y, z, x, w, 1, 2, 0, 3)
SWIZZLE_GEN_4(g, b, r, w, 1, 2, 0, 3)
SWIZZLE_GEN_4(y, z, y, x, 1, 2, 1, 0)
SWIZZLE_GEN_4(g, b, g, x, 1, 2, 1, 0)
SWIZZLE_GEN_4(y, z, y, y, 1, 2, 1, 1)
SWIZZLE_GEN_4(g, b, g, y, 1, 2, 1, 1)
SWIZZLE_GEN_4(y, z, y, z, 1, 2, 1, 2)
SWIZZLE_GEN_4(g, b, g, z, 1, 2, 1, 2)
SWIZZLE_GEN_4(y, z, y, w, 1, 2, 1, 3)
SWIZZLE_GEN_4(g, b, g, w, 1, 2, 1, 3)
SWIZZLE_GEN_4(y, z, z, x, 1, 2, 2, 0)
SWIZZLE_GEN_4(g, b, b, x, 1, 2, 2, 0)
SWIZZLE_GEN_4(y, z, z, y, 1, 2, 2, 1)
SWIZZLE_GEN_4(g, b, b, y, 1, 2, 2, 1)
SWIZZLE_GEN_4(y, z, z, z, 1, 2, 2, 2)
SWIZZLE_GEN_4(g, b, b, z, 1, 2, 2, 2)
SWIZZLE_GEN_4(y, z, z, w, 1, 2, 2, 3)
SWIZZLE_GEN_4(g, b, b, w, 1, 2, 2, 3)
SWIZZLE_GEN_4(y, z, w, x, 1, 2, 3, 0)
SWIZZLE_GEN_4(g, b, a, x, 1, 2, 3, 0)
SWIZZLE_GEN_4(y, z, w, y, 1, 2, 3, 1)
SWIZZLE_GEN_4(g, b, a, y, 1, 2, 3, 1)
SWIZZLE_GEN_4(y, z, w, z, 1, 2, 3, 2)
SWIZZLE_GEN_4(g, b, a, z, 1, 2, 3, 2)
SWIZZLE_GEN_4(y, z, w, w, 1, 2, 3, 3)
SWIZZLE_GEN_4(g, b, a, w, 1, 2, 3, 3)
SWIZZLE_GEN_4(y, w, x, x, 1, 3, 0, 0)
SWIZZLE_GEN_4(g, a, r, x, 1, 3, 0, 0)
SWIZZLE_GEN_4(y, w, x, y, 1, 3, 0, 1)
SWIZZLE_GEN_4(g, a, r, y, 1, 3, 0, 1)
SWIZZLE_GEN_4(y, w, x, z, 1, 3, 0, 2)
SWIZZLE_GEN_4(g, a, r, z, 1, 3, 0, 2)
SWIZZLE_GEN_4(y, w, x, w, 1, 3, 0, 3)
SWIZZLE_GEN_4(g, a, r, w, 1, 3, 0, 3)
SWIZZLE_GEN_4(y, w, y, x, 1, 3, 1, 0)
SWIZZLE_GEN_4(g, a, g, x, 1, 3, 1, 0)
SWIZZLE_GEN_4(y, w, y, y, 1, 3, 1, 1)
SWIZZLE_GEN_4(g, a, g, y, 1, 3, 1, 1)
SWIZZLE_GEN_4(y, w, y, z, 1, 3, 1, 2)
SWIZZLE_GEN_4(g, a, g, z, 1, 3, 1, 2)
SWIZZLE_GEN_4(y, w, y, w, 1, 3, 1, 3)
SWIZZLE_GEN_4(g, a, g, w, 1, 3, 1, 3)
SWIZZLE_GEN_4(y, w, z, x, 1, 3, 2, 0)
SWIZZLE_GEN_4(g, a, b, x, 1, 3, 2, 0)
SWIZZLE_GEN_4(y, w, z, y, 1, 3, 2, 1)
SWIZZLE_GEN_4(g, a, b, y, 1, 3, 2, 1)
SWIZZLE_GEN_4(y, w, z, z, 1, 3, 2, 2)
SWIZZLE_GEN_4(g, a, b, z, 1, 3, 2, 2)
SWIZZLE_GEN_4(y, w, z, w, 1, 3, 2, 3)
SWIZZLE_GEN_4(g, a, b, w, 1, 3, 2, 3)
SWIZZLE_GEN_4(y, w, w, x, 1, 3, 3, 0)
SWIZZLE_GEN_4(g, a, a, x, 1, 3, 3, 0)
SWIZZLE_GEN_4(y, w, w, y, 1, 3, 3, 1)
SWIZZLE_GEN_4(g, a, a, y, 1, 3, 3, 1)
SWIZZLE_GEN_4(y, w, w, z, 1, 3, 3, 2)
SWIZZLE_GEN_4(g, a, a, z, 1, 3, 3, 2)
SWIZZLE_GEN_4(y, w, w, w, 1, 3, 3, 3)
SWIZZLE_GEN_4(g, a, a, w, 1, 3, 3, 3)
SWIZZLE_GEN_4(z, x, x, x, 2, 0, 0, 0)
SWIZZLE_GEN_4(b, r, r, x, 2, 0, 0, 0)
SWIZZLE_GEN_4(z, x, x, y, 2, 0, 0, 1)
SWIZZLE_GEN_4(b, r, r, y, 2, 0, 0, 1)
SWIZZLE_GEN_4(z, x, x, z, 2, 0, 0, 2)
SWIZZLE_GEN_4(b, r, r, z, 2, 0, 0, 2)
SWIZZLE_GEN_4(z, x, x, w, 2, 0, 0, 3)
SWIZZLE_GEN_4(b, r, r, w, 2, 0, 0, 3)
SWIZZLE_GEN_4(z, x, y, x, 2, 0, 1, 0)
SWIZZLE_GEN_4(b, r, g, x, 2, 0, 1, 0)
SWIZZLE_GEN_4(z, x, y, y, 2, 0, 1, 1)
SWIZZLE_GEN_4(b, r, g, y, 2, 0, 1, 1)
SWIZZLE_GEN_4(z, x, y, z, 2, 0, 1, 2)
SWIZZLE_GEN_4(b, r, g, z, 2, 0, 1, 2)
SWIZZLE_GEN_4(z, x, y, w, 2, 0, 1, 3)
SWIZZLE_GEN_4(b, r, g, w, 2, 0, 1, 3)
SWIZZLE_GEN_4(z, x, z, x, 2, 0, 2, 0)
SWIZZLE_GEN_4(b, r, b, x, 2, 0, 2, 0)
SWIZZLE_GEN_4(z, x, z, y, 2, 0, 2, 1)
SWIZZLE_GEN_4(b, r, b, y, 2, 0, 2, 1)
SWIZZLE_GEN_4(z, x, z, z, 2, 0, 2, 2)
SWIZZLE_GEN_4(b, r, b, z, 2, 0, 2, 2)
SWIZZLE_GEN_4(z, x, z, w, 2, 0, 2, 3)
SWIZZLE_GEN_4(b, r, b, w, 2, 0, 2, 3)
SWIZZLE_GEN_4(z, x, w, x, 2, 0, 3, 0)
SWIZZLE_GEN_4(b, r, a, x, 2, 0, 3, 0)
SWIZZLE_GEN_4(z, x, w, y, 2, 0, 3, 1)
SWIZZLE_GEN_4(b, r, a, y, 2, 0, 3, 1)
SWIZZLE_GEN_4(z, x, w, z, 2, 0, 3, 2)
SWIZZLE_GEN_4(b, r, a, z, 2, 0, 3, 2)
SWIZZLE_GEN_4(z, x, w, w, 2, 0, 3, 3)
SWIZZLE_GEN_4(b, r, a, w, 2, 0, 3, 3)
SWIZZLE_GEN_4(z, y, x, x, 2, 1, 0, 0)
SWIZZLE_GEN_4(b, g, r, x, 2, 1, 0, 0)
SWIZZLE_GEN_4(z, y, x, y, 2, 1, 0, 1)
SWIZZLE_GEN_4(b, g, r, y, 2, 1, 0, 1)
SWIZZLE_GEN_4(z, y, x, z, 2, 1, 0, 2)
SWIZZLE_GEN_4(b, g, r, z, 2, 1, 0, 2)
SWIZZLE_GEN_4(z, y, x, w, 2, 1, 0, 3)
SWIZZLE_GEN_4(b, g, r, w, 2, 1, 0, 3)
SWIZZLE_GEN_4(z, y, y, x, 2, 1, 1, 0)
SWIZZLE_GEN_4(b, g, g, x, 2, 1, 1, 0)
SWIZZLE_GEN_4(z, y, y, y, 2, 1, 1, 1)
SWIZZLE_GEN_4(b, g, g, y, 2, 1, 1, 1)
SWIZZLE_GEN_4(z, y, y, z, 2, 1, 1, 2)
SWIZZLE_GEN_4(b, g, g, z, 2, 1, 1, 2)
SWIZZLE_GEN_4(z, y, y, w, 2, 1, 1, 3)
SWIZZLE_GEN_4(b, g, g, w, 2, 1, 1, 3)
SWIZZLE_GEN_4(z, y, z, x, 2, 1, 2, 0)
SWIZZLE_GEN_4(b, g, b, x, 2, 1, 2, 0)
SWIZZLE_GEN_4(z, y, z, y, 2, 1, 2, 1)
SWIZZLE_GEN_4(b, g, b, y, 2, 1, 2, 1)
SWIZZLE_GEN_4(z, y, z, z, 2, 1, 2, 2)
SWIZZLE_GEN_4(b, g, b, z, 2, 1, 2, 2)
SWIZZLE_GEN_4(z, y, z, w, 2, 1, 2, 3)
SWIZZLE_GEN_4(b, g, b, w, 2, 1, 2, 3)
SWIZZLE_GEN_4(z, y, w, x, 2, 1, 3, 0)
SWIZZLE_GEN_4(b, g, a, x, 2, 1, 3, 0)
SWIZZLE_GEN_4(z, y, w, y, 2, 1, 3, 1)
SWIZZLE_GEN_4(b, g, a, y, 2, 1, 3, 1)
SWIZZLE_GEN_4(z, y, w, z, 2, 1, 3, 2)
SWIZZLE_GEN_4(b, g, a, z, 2, 1, 3, 2)
SWIZZLE_GEN_4(z, y, w, w, 2, 1, 3, 3)
SWIZZLE_GEN_4(b, g, a, w, 2, 1, 3, 3)
SWIZZLE_GEN_4(z, z, x, x, 2, 2, 0, 0)
SWIZZLE_GEN_4(b, b, r, x, 2, 2, 0, 0)
SWIZZLE_GEN_4(z, z, x, y, 2, 2, 0, 1)
SWIZZLE_GEN_4(b, b, r, y, 2, 2, 0, 1)
SWIZZLE_GEN_4(z, z, x, z, 2, 2, 0, 2)
SWIZZLE_GEN_4(b, b, r, z, 2, 2, 0, 2)
SWIZZLE_GEN_4(z, z, x, w, 2, 2, 0, 3)
SWIZZLE_GEN_4(b, b, r, w, 2, 2, 0, 3)
SWIZZLE_GEN_4(z, z, y, x, 2, 2, 1, 0)
SWIZZLE_GEN_4(b, b, g, x, 2, 2, 1, 0)
SWIZZLE_GEN_4(z, z, y, y, 2, 2, 1, 1)
SWIZZLE_GEN_4(b, b, g, y, 2, 2, 1, 1)
SWIZZLE_GEN_4(z, z, y, z, 2, 2, 1, 2)
SWIZZLE_GEN_4(b, b, g, z, 2, 2, 1, 2)
SWIZZLE_GEN_4(z, z, y, w, 2, 2, 1, 3)
SWIZZLE_GEN_4(b, b, g, w, 2, 2, 1, 3)
SWIZZLE_GEN_4(z, z, z, x, 2, 2, 2, 0)
SWIZZLE_GEN_4(b, b, b, x, 2, 2, 2, 0)
SWIZZLE_GEN_4(z, z, z, y, 2, 2, 2, 1)
SWIZZLE_GEN_4(b, b, b, y, 2, 2, 2, 1)
SWIZZLE_GEN_4(z, z, z, z, 2, 2, 2, 2)
SWIZZLE_GEN_4(b, b, b, z, 2, 2, 2, 2)
SWIZZLE_GEN_4(z, z, z, w, 2, 2, 2, 3)
SWIZZLE_GEN_4(b, b, b, w, 2, 2, 2, 3)
SWIZZLE_GEN_4(z, z, w, x, 2, 2, 3, 0)
SWIZZLE_GEN_4(b, b, a, x, 2, 2, 3, 0)
SWIZZLE_GEN_4(z, z, w, y, 2, 2, 3, 1)
SWIZZLE_GEN_4(b, b, a, y, 2, 2, 3, 1)
SWIZZLE_GEN_4(z, z, w, z, 2, 2, 3, 2)
SWIZZLE_GEN_4(b, b, a, z, 2, 2, 3, 2)
SWIZZLE_GEN_4(z, z, w, w, 2, 2, 3, 3)
SWIZZLE_GEN_4(b, b, a, w, 2, 2, 3, 3)
SWIZZLE_GEN_4(z, w, x, x, 2, 3, 0, 0)
SWIZZLE_GEN_4(b, a, r, x, 2, 3, 0, 0)
SWIZZLE_GEN_4(z, w, x, y, 2, 3, 0, 1)
SWIZZLE_GEN_4(b, a, r, y, 2, 3, 0, 1)
SWIZZLE_GEN_4(z, w, x, z, 2, 3, 0, 2)
SWIZZLE_GEN_4(b, a, r, z, 2, 3, 0, 2)
SWIZZLE_GEN_4(z, w, x, w, 2, 3, 0, 3)
SWIZZLE_GEN_4(b, a, r, w, 2, 3, 0, 3)
SWIZZLE_GEN_4(z, w, y, x, 2, 3, 1, 0)
SWIZZLE_GEN_4(b, a, g, x, 2, 3, 1, 0)
SWIZZLE_GEN_4(z, w, y, y, 2, 3, 1, 1)
SWIZZLE_GEN_4(b, a, g, y, 2, 3, 1, 1)
SWIZZLE_GEN_4(z, w, y, z, 2, 3, 1, 2)
SWIZZLE_GEN_4(b, a, g, z, 2, 3, 1, 2)
SWIZZLE_GEN_4(z, w, y, w, 2, 3, 1, 3)
SWIZZLE_GEN_4(b, a, g, w, 2, 3, 1, 3)
SWIZZLE_GEN_4(z, w, z, x, 2, 3, 2, 0)
SWIZZLE_GEN_4(b, a, b, x, 2, 3, 2, 0)
SWIZZLE_GEN_4(z, w, z, y, 2, 3, 2, 1)
SWIZZLE_GEN_4(b, a, b, y, 2, 3, 2, 1)
SWIZZLE_GEN_4(z, w, z, z, 2, 3, 2, 2)
SWIZZLE_GEN_4(b, a, b, z, 2, 3, 2, 2)
SWIZZLE_GEN_4(z, w, z, w, 2, 3, 2, 3)
SWIZZLE_GEN_4(b, a, b, w, 2, 3, 2, 3)
SWIZZLE_GEN_4(z, w, w, x, 2, 3, 3, 0)
SWIZZLE_GEN_4(b, a, a, x, 2, 3, 3, 0)
SWIZZLE_GEN_4(z, w, w, y, 2, 3, 3, 1)
SWIZZLE_GEN_4(b, a, a, y, 2, 3, 3, 1)
SWIZZLE_GEN_4(z, w, w, z, 2, 3, 3, 2)
SWIZZLE_GEN_4(b, a, a, z, 2, 3, 3, 2)
SWIZZLE_GEN_4(z, w, w, w, 2, 3, 3, 3)
SWIZZLE_GEN_4(b, a, a, w, 2, 3, 3, 3)
SWIZZLE_GEN_4(w, x, x, x, 3, 0, 0, 0)
SWIZZLE_GEN_4(a, r, r, x, 3, 0, 0, 0)
SWIZZLE_GEN_4(w, x, x, y, 3, 0, 0, 1)
SWIZZLE_GEN_4(a, r, r, y, 3, 0, 0, 1)
SWIZZLE_GEN_4(w, x, x, z, 3, 0, 0, 2)
SWIZZLE_GEN_4(a, r, r, z, 3, 0, 0, 2)
SWIZZLE_GEN_4(w, x, x, w, 3, 0, 0, 3)
SWIZZLE_GEN_4(a, r, r, w, 3, 0, 0, 3)
SWIZZLE_GEN_4(w, x, y, x, 3, 0, 1, 0)
SWIZZLE_GEN_4(a, r, g, x, 3, 0, 1, 0)
SWIZZLE_GEN_4(w, x, y, y, 3, 0, 1, 1)
SWIZZLE_GEN_4(a, r, g, y, 3, 0, 1, 1)
SWIZZLE_GEN_4(w, x, y, z, 3, 0, 1, 2)
SWIZZLE_GEN_4(a, r, g, z, 3, 0, 1, 2)
SWIZZLE_GEN_4(w, x, y, w, 3, 0, 1, 3)
SWIZZLE_GEN_4(a, r, g, w, 3, 0, 1, 3)
SWIZZLE_GEN_4(w, x, z, x, 3, 0, 2, 0)
SWIZZLE_GEN_4(a, r, b, x, 3, 0, 2, 0)
SWIZZLE_GEN_4(w, x, z, y, 3, 0, 2, 1)
SWIZZLE_GEN_4(a, r, b, y, 3, 0, 2, 1)
SWIZZLE_GEN_4(w, x, z, z, 3, 0, 2, 2)
SWIZZLE_GEN_4(a, r, b, z, 3, 0, 2, 2)
SWIZZLE_GEN_4(w, x, z, w, 3, 0, 2, 3)
SWIZZLE_GEN_4(a, r, b, w, 3, 0, 2, 3)
SWIZZLE_GEN_4(w, x, w, x, 3, 0, 3, 0)
SWIZZLE_GEN_4(a, r, a, x, 3, 0, 3, 0)
SWIZZLE_GEN_4(w, x, w, y, 3, 0, 3, 1)
SWIZZLE_GEN_4(a, r, a, y, 3, 0, 3, 1)
SWIZZLE_GEN_4(w, x, w, z, 3, 0, 3, 2)
SWIZZLE_GEN_4(a, r, a, z, 3, 0, 3, 2)
SWIZZLE_GEN_4(w, x, w, w, 3, 0, 3, 3)
SWIZZLE_GEN_4(a, r, a, w, 3, 0, 3, 3)
SWIZZLE_GEN_4(w, y, x, x, 3, 1, 0, 0)
SWIZZLE_GEN_4(a, g, r, x, 3, 1, 0, 0)
SWIZZLE_GEN_4(w, y, x, y, 3, 1, 0, 1)
SWIZZLE_GEN_4(a, g, r, y, 3, 1, 0, 1)
SWIZZLE_GEN_4(w, y, x, z, 3, 1, 0, 2)
SWIZZLE_GEN_4(a, g, r, z, 3, 1, 0, 2)
SWIZZLE_GEN_4(w, y, x, w, 3, 1, 0, 3)
SWIZZLE_GEN_4(a, g, r, w, 3, 1, 0, 3)
SWIZZLE_GEN_4(w, y, y, x, 3, 1, 1, 0)
SWIZZLE_GEN_4(a, g, g, x, 3, 1, 1, 0)
SWIZZLE_GEN_4(w, y, y, y, 3, 1, 1, 1)
SWIZZLE_GEN_4(a, g, g, y, 3, 1, 1, 1)
SWIZZLE_GEN_4(w, y, y, z, 3, 1, 1, 2)
SWIZZLE_GEN_4(a, g, g, z, 3, 1, 1, 2)
SWIZZLE_GEN_4(w, y, y, w, 3, 1, 1, 3)
SWIZZLE_GEN_4(a, g, g, w, 3, 1, 1, 3)
SWIZZLE_GEN_4(w, y, z, x, 3, 1, 2, 0)
SWIZZLE_GEN_4(a, g, b, x, 3, 1, 2, 0)
SWIZZLE_GEN_4(w, y, z, y, 3, 1, 2, 1)
SWIZZLE_GEN_4(a, g, b, y, 3, 1, 2, 1)
SWIZZLE_GEN_4(w, y, z, z, 3, 1, 2, 2)
SWIZZLE_GEN_4(a, g, b, z, 3, 1, 2, 2)
SWIZZLE_GEN_4(w, y, z, w, 3, 1, 2, 3)
SWIZZLE_GEN_4(a, g, b, w, 3, 1, 2, 3)
SWIZZLE_GEN_4(w, y, w, x, 3, 1, 3, 0)
SWIZZLE_GEN_4(a, g, a, x, 3, 1, 3, 0)
SWIZZLE_GEN_4(w, y, w, y, 3, 1, 3, 1)
SWIZZLE_GEN_4(a, g, a, y, 3, 1, 3, 1)
SWIZZLE_GEN_4(w, y, w, z, 3, 1, 3, 2)
SWIZZLE_GEN_4(a, g, a, z, 3, 1, 3, 2)
SWIZZLE_GEN_4(w, y, w, w, 3, 1, 3, 3)
SWIZZLE_GEN_4(a, g, a, w, 3, 1, 3, 3)
SWIZZLE_GEN_4(w, z, x, x, 3, 2, 0, 0)
SWIZZLE_GEN_4(a, b, r, x, 3, 2, 0, 0)
SWIZZLE_GEN_4(w, z, x, y, 3, 2, 0, 1)
SWIZZLE_GEN_4(a, b, r, y, 3, 2, 0, 1)
SWIZZLE_GEN_4(w, z, x, z, 3, 2, 0, 2)
SWIZZLE_GEN_4(a, b, r, z, 3, 2, 0, 2)
SWIZZLE_GEN_4(w, z, x, w, 3, 2, 0, 3)
SWIZZLE_GEN_4(a, b, r, w, 3, 2, 0, 3)
SWIZZLE_GEN_4(w, z, y, x, 3, 2, 1, 0)
SWIZZLE_GEN_4(a, b, g, x, 3, 2, 1, 0)
SWIZZLE_GEN_4(w, z, y, y, 3, 2, 1, 1)
SWIZZLE_GEN_4(a, b, g, y, 3, 2, 1, 1)
SWIZZLE_GEN_4(w, z, y, z, 3, 2, 1, 2)
SWIZZLE_GEN_4(a, b, g, z, 3, 2, 1, 2)
SWIZZLE_GEN_4(w, z, y, w, 3, 2, 1, 3)
SWIZZLE_GEN_4(a, b, g, w, 3, 2, 1, 3)
SWIZZLE_GEN_4(w, z, z, x, 3, 2, 2, 0)
SWIZZLE_GEN_4(a, b, b, x, 3, 2, 2, 0)
SWIZZLE_GEN_4(w, z, z, y, 3, 2, 2, 1)
SWIZZLE_GEN_4(a, b, b, y, 3, 2, 2, 1)
SWIZZLE_GEN_4(w, z, z, z, 3, 2, 2, 2)
SWIZZLE_GEN_4(a, b, b, z, 3, 2, 2, 2)
SWIZZLE_GEN_4(w, z, z, w, 3, 2, 2, 3)
SWIZZLE_GEN_4(a, b, b, w, 3, 2, 2, 3)
SWIZZLE_GEN_4(w, z, w, x, 3, 2, 3, 0)
SWIZZLE_GEN_4(a, b, a, x, 3, 2, 3, 0)
SWIZZLE_GEN_4(w, z, w, y, 3, 2, 3, 1)
SWIZZLE_GEN_4(a, b, a, y, 3, 2, 3, 1)
SWIZZLE_GEN_4(w, z, w, z, 3, 2, 3, 2)
SWIZZLE_GEN_4(a, b, a, z, 3, 2, 3, 2)
SWIZZLE_GEN_4(w, z, w, w, 3, 2, 3, 3)
SWIZZLE_GEN_4(a, b, a, w, 3, 2, 3, 3)
SWIZZLE_GEN_4(w, w, x, x, 3, 3, 0, 0)
SWIZZLE_GEN_4(a, a, r, x, 3, 3, 0, 0)
SWIZZLE_GEN_4(w, w, x, y, 3, 3, 0, 1)
SWIZZLE_GEN_4(a, a, r, y, 3, 3, 0, 1)
SWIZZLE_GEN_4(w, w, x, z, 3, 3, 0, 2)
SWIZZLE_GEN_4(a, a, r, z, 3, 3, 0, 2)
SWIZZLE_GEN_4(w, w, x, w, 3, 3, 0, 3)
SWIZZLE_GEN_4(a, a, r, w, 3, 3, 0, 3)
SWIZZLE_GEN_4(w, w, y, x, 3, 3, 1, 0)
SWIZZLE_GEN_4(a, a, g, x, 3, 3, 1, 0)
SWIZZLE_GEN_4(w, w, y, y, 3, 3, 1, 1)
SWIZZLE_GEN_4(a, a, g, y, 3, 3, 1, 1)
SWIZZLE_GEN_4(w, w, y, z, 3, 3, 1, 2)
SWIZZLE_GEN_4(a, a, g, z, 3, 3, 1, 2)
SWIZZLE_GEN_4(w, w, y, w, 3, 3, 1, 3)
SWIZZLE_GEN_4(a, a, g, w, 3, 3, 1, 3)
SWIZZLE_GEN_4(w, w, z, x, 3, 3, 2, 0)
SWIZZLE_GEN_4(a, a, b, x, 3, 3, 2, 0)
SWIZZLE_GEN_4(w, w, z, y, 3, 3, 2, 1)
SWIZZLE_GEN_4(a, a, b, y, 3, 3, 2, 1)
SWIZZLE_GEN_4(w, w, z, z, 3, 3, 2, 2)
SWIZZLE_GEN_4(a, a, b, z, 3, 3, 2, 2)
SWIZZLE_GEN_4(w, w, z, w, 3, 3, 2, 3)
SWIZZLE_GEN_4(a, a, b, w, 3, 3, 2, 3)
SWIZZLE_GEN_4(w, w, w, x, 3, 3, 3, 0)
SWIZZLE_GEN_4(a, a, a, x, 3, 3, 3, 0)
SWIZZLE_GEN_4(w, w, w, y, 3, 3, 3, 1)
SWIZZLE_GEN_4(a, a, a, y, 3, 3, 3, 1)
SWIZZLE_GEN_4(w, w, w, z, 3, 3, 3, 2)
SWIZZLE_GEN_4(a, a, a, z, 3, 3, 3, 2)
SWIZZLE_GEN_4(w, w, w, w, 3, 3, 3, 3)
SWIZZLE_GEN_4(a, a, a, w, 3, 3, 3, 3)

};


}
#endif
