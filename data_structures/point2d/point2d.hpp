#pragma once

#include <tuple>

template<typename T>
struct point {
    T x, y;
    int id;
    bool operator==(point a) const { return x == a.x && y == a.y; }
    bool operator<(const point a) const { return std::tie(x, y) < std::tie(a.x, a.y); }
    bool operator>(point a) const { return std::tie(x, y) > std::tie(a.x, a.y); }
    point& operator+=(point a) { x += a.x, y += a.y; return *this; }
    point operator+(point a) const { point p = *this; p += a; return p; }
    point& operator-=(point a) { x -= a.x, y -= a.y; return *this; }
    point operator-(point a) const { point p = *this; p -= a; return p; }
    T operator*(point a) const { return x * a.x + y * a.y; }
    T operator^(point a) const { return x * a.y - y * a.x; }
    bool belongs(point a, point b) const { return ((a - *this) ^ (b - *this)) == 0; }
    bool inside(point a, point b) const { return (*this).belongs(a, b) && (a - *this) * (b - *this) <= 0; }
    bool left(point a, point b) const { return ((a - *this) ^ (b - *this)) > 0; }
};
