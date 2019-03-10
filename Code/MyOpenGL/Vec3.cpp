#include "Vec3.h"

template <typename T>
Vec3<T>::Vec3(T x, T y, T z):
m_x(x), m_y(y), m_z(z)
{
}

template <typename T>
Vec3<T>::~Vec3()
{
}

template <typename T>
T Vec3<T>::z()
{
	return m_z;
}

template <typename T>
T Vec3<T>::y()
{
	return m_y;
}

template <typename T>
T Vec3<T>::x()
{
	return m_x;
}
