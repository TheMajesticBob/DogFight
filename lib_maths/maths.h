#pragma once

#include <SFML/System.hpp>
#include <cmath>
#include <iomanip>  // std::setprecision
#include <iostream> // std::cout, std::fixed
#include <sstream>
#include <vector>
#include <glm/common.hpp>

namespace sf 
{

typedef Vector2<size_t> Vector2ul;

template<typename T>
T rand_range(T low, T high)
{
	return low + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (high - low)));
}

// Returns the length of the vector
template <typename T> double length(const Vector2<T>& v) {
  return sqrt(v.x * v.x + v.y * v.y);
}

// Returns the length squared of the vector
template <typename T> double lengthSquared(const Vector2<T>& v) {
	return v.x * v.x + v.y * v.y;
}

// double clamp(const double& v, const double& minRange, const double& maxRange)
// {
// 	if (v >= minRange)
// 	{
// 		if (v <= maxRange)
// 		{
// 			return v;
// 		}
// 
// 		return maxRange;
// 	}
// 
// 	return minRange;
// }

template <typename T> const double& minimum_distance(Vector2<T>& v, Vector2<T>& w, Vector2<T>& p) {
	// Return minimum distance between line segment vw and point p
	const T l2 = lengthSquared(v - w);  // i.e. |w-v|^2 -  avoid a sqrt
	if (l2 == 0.0) return length(p - v);   // v == w case
	// Consider the line extending the segment, parameterized as v + t (w - v).
	// We find projection of point p onto the line. 
	// It falls where t = [(p-v) . (w-v)] / |w-v|^2
	// We clamp t from [0,1] to handle points outside the segment vw.
	float d = dot(p - v, w - v) / l2;
	float t = glm::clamp<float>(d, 0, 1);
	const Vector2<T> projection = v + t * (w - v);  // Projection falls on the segment
	return length(p - projection);
}

// Normalizes the vector
template <typename T> Vector2<T> normalize(const Vector2<T>& v) {
  Vector2<T> vector;
  double l = length(v);

  if (l != 0) {
    vector.x = v.x / l;
    vector.y = v.y / l;
  }

  return vector;
}

template <typename T>
Vector2<T> operator*(const Vector2<T>& left, const Vector2<T>& right) {
  Vector2<T> r = left;
  r.x *= right.x;
  r.y *= right.y;
  return r;
}

template<typename T>
const float dot(const Vector2<T>& a, const Vector2<T>& b)
{
	return a.x * b.y + a.y * b.x;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) {
  os << '(' << v.x << ',' << v.y << ')';
  return os;
}

template <typename T, typename U> Vector2<T> Vcast(const Vector2<U>& v) {
  return Vector2<T>(static_cast<T>(v.x), static_cast<T>(v.y));
};

static double deg2rad(double degrees) {
  return degrees * 4.0 * atan(1.0) / 180.0;
}

static double rad2deg(double radians) {
	return radians / (4.0 * atan(1.0)) * 180.0;
}

template <typename T>
Vector2<T> rotate(const Vector2<T>& v, const double degrees) {
  const double theta = deg2rad(degrees);

  const double cs = cos(theta);
  const double sn = sin(theta);

  return {(T)(v.x * cs - v.y * sn), (T)(v.x * sn + v.y * cs)};
}

} // namespace sf

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
  os << "vec[";
  for (const auto& a : v) {
    os << a << ',';
  }
  os << ']';
  return os;
}

template <typename T>
std::string toStrDecPt(const uint16_t& dp, const T& i) {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(dp) << i;
  return stream.str();
}

template<typename T>
T lerp(T a, T b, float t)
{
  t = fmax(0.0f, fmin(1.0f, t));
  return (1.0f - t) * a + t * b;
}