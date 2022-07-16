#include <stdio.h>
#include <string>

// I am not willing to write 100s of combinations of letters manually. Who would have thought... This program was used to generate the macros required for the custom syntax and simply the need for generation and the way the custom syntax works violates C++ principles, good practices and common sense on multiple levels.

std::string v1[] = {"x", "y", "z", "w"};
std::string v2[] = {"r", "g", "b", "a"};

int main(){
	
	printf("\n// Generated functions for accesing single elements\n");
	for(int i = 0; i < 4; ++i){
				printf("SWIZZLE_GEN_1(%s, %d)\n", v1[i].c_str(), i);
				printf("SWIZZLE_GEN_1(%s, %d)\n", v2[i].c_str(), i);
	}
	printf("\n// Generated functions for 2 element swizzling\n");
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
				printf("SWIZZLE_GEN_2(%s, %s, %d, %d)\n", v1[i].c_str(), v1[j].c_str(), i, j);
				printf("SWIZZLE_GEN_2(%s, %s, %d, %d)\n", v2[i].c_str(), v2[j].c_str(), i, j);
		}
	}

	printf("\n// Generated functions for 3 element swizzling\n");
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
			for(int k = 0; k < 4; ++k){
				printf("SWIZZLE_GEN_3(%s, %s, %s, %d, %d, %d)\n", v1[i].c_str(), v1[j].c_str(), v1[k].c_str(), i, j, k);
				printf("SWIZZLE_GEN_3(%s, %s, %s, %d, %d, %d)\n", v2[i].c_str(), v2[j].c_str(), v2[k].c_str(), i, j, k);
			}
		}
	}
	printf("\n// Generated functions for 4 element swizzling\n");
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
			for(int k = 0; k < 4; ++k){
				for(int l = 0; l < 4; ++l){
				printf("SWIZZLE_GEN_4(%s, %s, %s, %s, %d, %d, %d, %d)\n", v1[i].c_str(), v1[j].c_str(), v1[k].c_str(), v1[l].c_str(), i, j, k, l);
				printf("SWIZZLE_GEN_4(%s, %s, %s, %s, %d, %d, %d, %d)\n", v2[i].c_str(), v2[j].c_str(), v2[k].c_str(), v1[l].c_str(), i, j, k, l);
				}
			}
		}
	}


}
