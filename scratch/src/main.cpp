#include <iostream>
#include <cpprelude/memory.h>

struct vec3
{
	float x,y,z;
};

int
main(int argc, char** argv)
{
	std::cout << "Hello, World!" << std::endl;
	cpprelude::handle<vec3> v = cpprelude::alloc<vec3>();
	v->x = 1;
	v->y = 2;
	v->z = 3;

	cpprelude::handle<vec3> v2 = cpprelude::tmp::move(v);
	v2->x = 4;
	v2->y = 5;
	v2->z = 6;

	vec3& v3 = *v2;
	v3.x = 7;
	v3.y = 8;
	v3.z = 9;

	{
		auto weak_k = v2.to_weak();
		weak_k->x = 10;
		weak_k->y = 11;
		weak_k->z = 12;
	}

	return 0;
}