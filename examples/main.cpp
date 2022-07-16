#include "benutils/vector.hpp"
#include <stdio.h>

using bu::math::Vecn;
using bu::math::Vec2;
using bu::math::Vec3;
using bu::math::Vec4;

int main(int argc, char* argv[]){
	Vec2 a;
	a[1] = 1;
	a[2] = 0;
	Vec2 b;
	b[1] = 2;
	b[2] = 2;
	printf("Vectors: a = ( %f, %f ), b = ( %f, %f )\n", a[1], a[2], b[1], b[2]);
	printf("Of norms: %f, %f\n", a.norm(), b.norm());
	printf("And squared norms: %f, %f\n", a.squared_norm(), b.squared_norm());
	// Addition, subtraction, unary minus operators are defined as any normal mathematician would expect.
	b.normalize();
	printf("After normalization b is (%f, %f)\n", b[1], b[2]);
	Vec2 c = a + b;
	printf("Sum of a and b: ( %f, %f )\n", c[1], c[2]);
	c = a - b;
	printf("Difference of a and b ( %f, %f )\n", c[1], c[2]);
	c = -b;
	printf("Opposite of b (-b): ( %f, %f )\n", c[1], c[2]);

	b[1] = 1;
	b[2] = 0;
	printf("Vector b = ( %f, %f )\n", b[1], b[2]);

	a = b.xx();
	printf("Vector a = ( %f, %f )\n", a[1], a[2]);
	a = b.xy();
	printf("Vector a = ( %f, %f )\n", a[1], a[2]);
	a = b.yx();
	printf("Vector a = ( %f, %f )\n", a[1], a[2]);
	a = b.yy();
	printf("Vector a = ( %f, %f )\n", a[1], a[2]);

	// The * operator is defined as the dot product.
	double d = a * b;
	printf("Dot product of a and b: %f\n", d);

	Vec3 k;
	k[1] = 1;
	k[2] = 2;
	k[3] = 3;
	printf("%f %f %f\n", k[1], k[2], k[3]);
	Vec2 l;
	l[1] = 4;
	l[2] = 5;
	printf("%f %f\n", l[1], l[2]);
	k = l;
	printf("%f %f %f\n", k[1], k[2], k[3]);
	k = l.xxy();	
	printf("%f %f %f\n", k[1], k[2], k[3]);

	
}
