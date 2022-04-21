#ifndef FT_CONTAINERS_FT_ITERATOR_HPP
#define FT_CONTAINERS_FT_ITERATOR_HPP

#include <ft_algorithm.hpp>

namespace ft {

	struct input_iterator_tag {
	};
	struct output_iterator_tag {
	};
	struct forward_iterator_tag : public input_iterator_tag {
	};
	struct bidirectional_iterator_tag : public forward_iterator_tag {
	};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {
	};

	template<typename T>
	struct iterator_traits {
		typedef std::ptrdiff_t difference_type;
		typedef T value_type;
		typedef T *pointer;
		typedef T &reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template<typename T>
	struct iterator_traits<const T> {
		typedef std::ptrdiff_t difference_type;
		typedef T value_type;
		typedef const T *pointer;
		typedef const T &reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template<typename T>
	struct iterator_traits<T *> {
		typedef std::ptrdiff_t difference_type;
		typedef T value_type;
		typedef T *pointer;
		typedef T &reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template<typename T>
	struct iterator_traits<const T *> {
		typedef std::ptrdiff_t difference_type;
		typedef T value_type;
		typedef const T *pointer;
		typedef const T &reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template<typename T>
	class reverse_iterator : public T {

	protected:
		T current;

	public:
		using typename T::difference_type;
		using typename T::value_type;
		using typename T::pointer;
		using typename T::reference;
		using typename T::iterator_category;

		reverse_iterator() : current() {}

		reverse_iterator(const T &other) { current = other; }

		template<typename X>
		reverse_iterator(const reverse_iterator<X> &other) { *this = other; }

		template<typename X>
		reverse_iterator &operator=(const reverse_iterator<X> &other) {
			current = other.base();
			return *this;
		}

		reference operator*() {
			T tmp = current;
			return *--tmp;
		}

		pointer operator->() {
			T tmp = current;
			--tmp;
			return tmp.operator->();
		}

		reference operator*() const {
			T tmp = current;
			return *--tmp;
		}

		pointer operator->() const {
			T tmp = current;
			--tmp;
			return tmp.operator->();
		}

		reference operator[](difference_type n) const { return reference(*this->operator+(n)); };

		reverse_iterator &operator++() {
			--current;
			return *this;
		}

		reverse_iterator &operator--() {
			++current;
			return *this;
		}

		reverse_iterator operator++(int) {
			reverse_iterator tmp(*this);
			--current;
			return tmp;
		}

		reverse_iterator operator--(int) {
			reverse_iterator tmp(*this);
			++current;
			return tmp;
		}

		reverse_iterator operator+(difference_type n) const { return current - n; }

		reverse_iterator operator-(difference_type n) const { return current + n; }

		reverse_iterator &operator+=(difference_type n) {
			current -= n;
			return *this;
		}

		reverse_iterator &operator-=(difference_type n) {
			current += n;
			return *this;
		}

		T base() { return current; }

		T base() const { return current; }
	};

	template<class U, class X>
	reverse_iterator<X> operator+(U lhs, const reverse_iterator<X> &rhs) {
		return rhs + lhs;
	}

	template<class U, class X>
	typename reverse_iterator<U>::difference_type
	operator-(const reverse_iterator<U> &lhs, const reverse_iterator<X> &rhs) {
		return -(lhs.base() - rhs.base());
	}

	template<typename U, typename X>
	bool operator==(const reverse_iterator<U> &lhs, const reverse_iterator<X> &rhs) {
		return lhs.base() == rhs.base();
	}

	template<typename U, typename X>
	bool operator!=(const reverse_iterator<U> &lhs, const reverse_iterator<X> &rhs) {
		return lhs.base() != rhs.base();
	};

	template<typename U, typename X>
	bool operator<(const reverse_iterator<U> &lhs, const reverse_iterator<X> &rhs) { return lhs.base() > rhs.base(); }

	template<typename U, typename X>
	bool operator<=(const reverse_iterator<U> &lhs, const reverse_iterator<X> &rhs) { return lhs.base() >= rhs.base(); }

	template<typename U, typename X>
	bool operator>(const reverse_iterator<U> &lhs, const reverse_iterator<X> &rhs) { return lhs.base() < rhs.base(); }

	template<typename U, typename X>
	bool operator>=(const reverse_iterator<U> &lhs, const reverse_iterator<X> &rhs) { return lhs.base() <= rhs.base(); }


}

#endif
