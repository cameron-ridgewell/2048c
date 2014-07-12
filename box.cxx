#include "box.hxx"

	Box::Box(size_t value):
		val_(value)
	{}

	Box::Box(const Box& src):
		val_(src.val_)
	{}

	Box& Box::operator=(const Box& rhs)
	{
		if (this != &rhs)
		{
			Box temp(rhs);
			swap(temp);
		}
		return *this;
	}

	size_t Box::merge()
	{
		return val_*=2;
	}

	size_t Box::value(void)
	{
		return val_;
	}

	void Box::swap(Box& src)
	{
		std::swap(val_,src.val_);
	}
