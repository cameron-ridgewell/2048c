#ifndef BOX_HXX
#define BOX_HXX

#include <stdio.h>
#include <cstdlib>
#include <algorithm>
#include <utility>

class Box
{
	public:
		Box(size_t value = 0);

		Box(const Box& src);

		Box& operator=(const Box& rhs);

		size_t merge();

		size_t value(void);

	public:
		size_t val_;

		void swap(Box& src);
};

#endif