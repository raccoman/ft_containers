#ifndef FT_CONTAINERS_ITERATOR_HPP
#define FT_CONTAINERS_ITERATOR_HPP

namespace ft {

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	template<typename T>
	struct iterator_traits {
		typedef std::ptrdiff_t					difference_type;
		typedef T								value_type;
		typedef T*								pointer;
		typedef T&								reference;
		typedef random_access_iterator_tag		iterator_category;
	};

	template<typename T>
	struct iterator_traits<const T> {
		typedef std::ptrdiff_t					difference_type;
		typedef T								value_type;
		typedef const T*						pointer;
		typedef const T&						reference;
		typedef random_access_iterator_tag		iterator_category;
	};

	template<typename T>
	struct iterator_traits<T*> {
		typedef std::ptrdiff_t				difference_type;
		typedef T							value_type;
		typedef T*							pointer;
		typedef T&							reference;
		typedef random_access_iterator_tag	iterator_category;
	};

	template<typename T>
	struct iterator_traits<const T*> {
		typedef std::ptrdiff_t					difference_type;
		typedef T							value_type;
		typedef const T*					pointer;
		typedef const T&					reference;
		typedef random_access_iterator_tag	iterator_category;
	};

	template<typename Category, typename T, typename Distance = ptrdiff_t, typename Pointer = T*, typename Reference = T&>
	struct iterator {
		typedef T         value_type;
		typedef Distance  difference_type;
		typedef Pointer   pointer;
		typedef Reference reference;
		typedef Category  iterator_category;
	};

	template<typename T>
	class reverse_iterator : public T {

	public:
		using typename T::value_type;
		using typename T::pointer;
		using typename T::reference;
		using typename T::difference_type;

		reverse_iterator() : T() {}
		reverse_iterator(const T& other) : T(other) {}
		reverse_iterator(const reverse_iterator& other) : T(other.ptr) {}

		reverse_iterator& operator=(const reverse_iterator& other) { this->ptr = other.ptr; return *this; }
		reference operator*() { return *this->ptr; }
		pointer operator->() { return this->ptr; }

		reverse_iterator operator++(int) { reverse_iterator tmp(*this); operator++(); return tmp; }
		T& operator++() { return this->T::operator--(); }
		reverse_iterator operator--(int) { reverse_iterator tmp(*this); operator--(); return tmp; }
		T& operator--() { return this->T::operator++(); }

	};


}

#endif
