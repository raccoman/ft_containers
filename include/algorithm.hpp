#ifndef FT_CONTAINERS_ALGORITHM_HPP
#define FT_CONTAINERS_ALGORITHM_HPP

namespace ft {

	template<typename T, typename U, typename R>
	struct binary_function {
		typedef T 	first_argument_type;
		typedef U 	second_argument_type;
		typedef R 	result_type;
	};

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

	template<class T1, class T2>
	struct pair {

		typedef T1	first_type;
		typedef T2	second_type;

		T1	first;
		T2	second;

		pair() : first(), second() {};
		pair(const T1& x, const T2& y) : first(x), second(y) {};
		template<class U1, class U2>
		pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {};

		pair& operator=(const pair& other) {
			first = other.first;
			second = other.second;
			return *this;
		}

	};

	template<class T1, class T2>
	ft::pair<T1,T2> make_pair(T1 t, T2 u) {
		return pair<T1, T2>(t, u);
	}

	template<class T1, class T2>
	bool operator==(const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs) {
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	template<class T1, class T2>
	bool operator!=(const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs) {
		return !(lhs == rhs);
	}

	template<class T1, class T2>
	bool operator<(const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs) {
		return lhs.first != rhs.first ? lhs.first < rhs.first : lhs.second < rhs.second;
	}

	template<class T1, class T2>
	bool operator<=(const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs) {
		return !(rhs < lhs);
	}

	template<class T1, class T2>
	bool operator>(const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs) {
		return rhs < lhs;
	}

	template<class T1, class T2>
	bool operator>=(const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs) {
		return !(lhs < rhs);
	}

}

#endif
