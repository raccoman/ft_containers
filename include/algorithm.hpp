#ifndef FT_CONTAINERS_ALGORITHM_HPP
#define FT_CONTAINERS_ALGORITHM_HPP

namespace ft {

	template<class T>
	const T& min(const T& a, const T& b) {
		return (a < b ? a : b);
	}

	template<class T>
	void swap(T& a, T& b) {
		T tmp(a);
		a = b;
		b = tmp;
	}

	template<class T, class U>
	bool lexicographical_compare(T first1, T last1, U first2, U last2) {
		while (first1 != last1) {

			if (first2 == last2 || *first2 < *first1) return false;
			else if (*first1 < *first2) return true;

			++first1;
			++first2;

		}
		return first2 != last2;
	}

}

#endif
