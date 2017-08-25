#include <iostream>
#include <memory>
#include <cpprelude/dynamic_array.h>
using namespace std;

class Shape
{
public:
	virtual float area() = 0;
};

class Rectangle: public Shape
{
public:
	float w, h;

	Rectangle(){}
	Rectangle(float w_, float h_)
		:w(w_), h(h_)
	{}

	float area() override
	{
		return w*h;
	}
};

class Circle: public Shape
{
public:
	float r;
	Circle(){}
	Circle(float r_) :r(r_){}

	float area() override
	{
		return r*r*3.14;
	}
};

int oop()
{
	std::cout << "Hello, World!" << std::endl;

	cpprelude::dynamic_array<unique_ptr<Shape>> shapes;
	shapes.reserve(10000000);
	for(size_t i = 0; i < 10000000; ++i)
	{
		if(rand()%2)
		{
			shapes.insert_back(make_unique<Rectangle>(rand()%100, rand()%100));
		}
		else
		{
			shapes.insert_back(make_unique<Circle>(rand()%100));
		}
	}

	cpprelude::dynamic_array<float> areas;
	areas.reserve(10000000);
	for(auto& shape: shapes)
	{
		areas.insert_back(shape->area());
	}

	std::cout << "length: " << areas.count() << std::endl;
	return 0;
}

int main()
{
	return oop();
}


// #include <iostream>
// #include <cpprelude/dynamic_array.h>
// using namespace cpprelude;
//
// int
// main(int argc, char** argv)
// {
// 	dynamic_array<usize> numbers;
//
// 	numbers.reserve(10);
//
// 	for(usize i = 0; i < 10; ++i)
// 		numbers.insert_back(i+1);
//
// 	numbers.insert_back({11, 12, 13, 14, 15, 16, 17, 18, 19, 20});
//
// 	for(const auto& number: numbers)
// 		std::cout << number << std::endl;
// 	return 0;
// }
