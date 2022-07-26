#include "benutils/vector.hpp"
#include "benutils/matrix.hpp"
#include <stdio.h>
#include <iostream>

using bu::math::Vecn;
using bu::math::Vec2;
using bu::math::Vec3;
using bu::math::Vec4;
using bu::math::Matrix;

int main(int argc, char* argv[]){
	Vec2 a;
	a[1] = 1;
	a[2] = 0;
	Vec2 b = {2, 2};
	printf("Vectors: a = ( %f, %f ), b = ( %f, %f )\n", a[1], a[2], b[1], b[2]);
	printf("Of norms: %f, %f\n", a.norm(), b.norm());
	printf("And squared norms: %f, %f\n", a.squared_norm(), b.squared_norm());
	// Addition, subtraction, unary minus operators are defined as any normal mathematician would expect.
	b.normalize();
	printf("After normalization b is (%f, %f)\n", b[1], b[2]);
	Vec2 c = a + b;
	printf("Sum of a and b: ( %f, %f )\n", c[1], c[2]);
	c = a - b;
	printf("Difference of a and b (normalized): ( %f, %f )\n", c[1], c[2]);
	c = -b;
	printf("Opposite of b (-b): ( %f, %f )\n", c[1], c[2]);

	b[1] = 1;
	b[2] = 0;
	printf("Vector b = ( %f, %f )\n", b[1], b[2]);

	a = b.xx();
	printf("After setting a to b.xx a = %s", b.to_str().c_str() );
	a = b.xy();
	printf("After setting a to b.xy a = ( %f, %f )\n", a[1], a[2]);
	a = b.yx();
	printf("After setting a to b.yx a = ( %f, %f )\n", a[1], a[2]);
	a = b.yy();
	printf("After setting a to b.yy a = ( %f, %f )\n", a[1], a[2]);

	// The * operator is defined as the dot product.
	double d = a * b;
	printf("Dot product of a and b: %f\n", d);

	Vec3 k;
	k[1] = 1;
	k[2] = 2;
	k[3] = 3;
	printf("k = (%f, %f, %f,)\n", k[1], k[2], k[3]);
	Vec2 l;
	l[1] = 4;
	l[2] = 5;
	printf("l = (%f, %f)\n", l[1], l[2]);
	k = l;
	printf("After promoting l into k by casting k = (%f, %f, %f,)\n", k[1], k[2], k[3]);
	k = l.yyx();	
	printf("After setting k to l.yyx k = (%f, %f, %f,)\n", k[1], k[2], k[3]);

	// fill a 2x2 matrix with with elements. The first row with 1s and the second row by swizzling from vectors.
	Matrix<2, 2, double> m;
	m[1][1] = 1;
	m[1][2] = 1;
	m[2] = l.xx();
	printf("%f, %f \n%f, %f\n\n", m[1][1], m[1][2], m[2][1], m[2][2]);
	
	Matrix<2, 2, double> t = {1, 0, 0 };
	printf("%f, %f \n%f, %f\n\n", t[1][1], t[1][2], t[2][1], t[2][2]);
	Matrix<2, 2, double> u = {2, 2, 2, 2};
	printf("%f, %f \n%f, %f\n\n", u[1][1], u[1][2], u[2][1], u[2][2]);
	Matrix<2, 2, double> v = t*u;
	printf("%f, %f \n%f, %f\n\n", v[1][1], v[1][2], v[2][1], v[2][2]);


}
