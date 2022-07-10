#include "benutils/matrix.hpp"
#include <stdio.h>

using bu::math::Vecn;
using bu::math::Vec2;
using bu::math::Vec3;
using bu::math::Vec4;

int main(int argc, char* argv[]){
	Vec2 v;
	printf("( %f, %f )", v[1], v[2]);
	v[1] = 3;
	v[2] = 6.42;
	printf("( %f, %f )", v[1], v[2]);
	Vec3 v2;
	v2[1] = 1;
	v2[2] = 0;
	v2[3] = 0;
	Vec3 v3;
	v3[1] = 0;
	v3[2] = 1;
	v3[3] = 0;
	printf("( %f, %f, %f )", v2[1], v2[2], v2[3]);
	printf("( %f, %f, %f )", v3[1], v3[2], v3[3]);
	Vec3 v4;
	v4 = v2.cross(v3);

	printf("( %f, %f, %f )", v4[1], v4[2], v4[3]);
}
