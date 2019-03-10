#pragma once
template <typename T>
class Vec3
{
public:
	Vec3(T x,T y,T z);
	T x();
	T y();
	T z();
	~Vec3();
private:
	T m_x;
	T m_y;
	T m_z;
};

