#include <algorithm>
#include <vector>

#include "point2d.hpp"

template<typename T>
std::vector<point<T>> chull(std::vector<point<T>> pgon) {
	std::vector<point<T>> hull;
	int n = pgon.size(), k = 0;
	std::sort(pgon.begin(), pgon.end());
	for (int t = 0; t < 2; t++) {
		int m = 0;
		for (int i = 0; i < n; i++) {
			while (m >= 2 and not pgon[i].left(hull[k-2], hull[k-1])) {
				hull.pop_back(), m--, k--;
			}
			if (i < n - 1) {
				hull.push_back(pgon[i]);
				m++, k++;
			}
		}
		std::reverse(pgon.begin(), pgon.end());
	}
	return hull;
}
