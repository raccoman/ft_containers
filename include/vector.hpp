#ifndef FT_CONTAINERS_VECTOR_HPP
#define FT_CONTAINERS_VECTOR_HPP

#include <memory>
#include <ft_iterator.hpp>
#include <ft_algorithm.hpp>
#include <iostream>
#include <unistd.h>
#include <memory>
#include <type_traits>

namespace ft {

	template<typename T>
	class vector_iterator {

	public:
		typedef T iterator_type;
		typedef typename iterator_traits<iterator_type>::iterator_category iterator_category;
		typedef typename iterator_traits<iterator_type>::value_type value_type;
		typedef typename iterator_traits<iterator_type>::difference_type difference_type;
		typedef typename iterator_traits<iterator_type>::pointer pointer;
		typedef typename iterator_traits<iterator_type>::reference reference;

	protected:
		pointer ptr;

	public:
		vector_iterator() : ptr(nullptr) {}

		vector_iterator(pointer ptr) : ptr(ptr) {}

		vector_iterator(const vector_iterator &other) : ptr(other.ptr) {}

		template<typename X>
		vector_iterator(const vector_iterator<X> &other) : ptr(other.base()) {}

		~vector_iterator() {}

		vector_iterator &operator=(const vector_iterator &other) {
			this->ptr = other.ptr;
			return *this;
		}

		reference operator*() { return *this->ptr; }

		pointer operator->() { return this->ptr; }

		reference operator[](difference_type n) const { return reference(*(ptr + n)); }

		vector_iterator operator++(int) {
			vector_iterator tmp(*this);
			++this->ptr;
			return tmp;
		}

		vector_iterator &operator++() {
			++this->ptr;
			return *this;
		}

		vector_iterator operator--(int) {
			vector_iterator tmp(*this);
			--this->ptr;
			return tmp;
		}

		vector_iterator &operator--() {
			--this->ptr;
			return *this;
		}

		vector_iterator operator+=(difference_type n) {
			ptr += n;
			return vector_iterator(ptr);
		}

		vector_iterator operator+(difference_type n) const { return vector_iterator(ptr + n); }

		vector_iterator operator-=(difference_type n) {
			ptr -= n;
			return vector_iterator(ptr);
		}

		vector_iterator operator-(difference_type n) const { return vector_iterator(ptr - n); }

		difference_type operator-(const vector_iterator &other) const { return this->ptr - other.ptr; }

		pointer base() { return ptr; }

		pointer base() const { return ptr; }
	};

	template<class U, class X>
	vector_iterator<X> operator+(U lhs, const vector_iterator<X> &rhs) {
		return (rhs + lhs);
	}

	template<typename U>
	bool operator==(const vector_iterator<U> &lhs, const typename vector_iterator<U>::pointer &rhs) {
		return lhs.base() == rhs;
	}

	template<typename U, typename X>
	bool operator==(const vector_iterator<U> &lhs, const vector_iterator<X> &rhs) { return lhs.base() == rhs.base(); }

	template<typename U, typename X>
	bool operator!=(const vector_iterator<U> &lhs, const vector_iterator<X> &rhs) { return lhs.base() != rhs.base(); }

	template<typename U, typename X>
	bool operator<(const vector_iterator<U> &lhs, const vector_iterator<X> &rhs) { return lhs.base() < rhs.base(); }

	template<typename U, typename X>
	bool operator<=(const vector_iterator<U> &lhs, const vector_iterator<X> &rhs) { return lhs.base() <= rhs.base(); }

	template<typename U, typename X>
	bool operator>(const vector_iterator<U> &lhs, const vector_iterator<X> &rhs) { return lhs.base() > rhs.base(); }

	template<typename U, typename X>
	bool operator>=(const vector_iterator<U> &lhs, const vector_iterator<X> &rhs) { return lhs.base() >= rhs.base(); }

	template<typename T, class Allocator = std::allocator<T> >
	class vector {

	public:
		typedef T value_type;
		typedef Allocator allocator_type;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef vector_iterator<value_type> iterator;
		typedef vector_iterator<const value_type> const_iterator;
		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef typename iterator_traits<iterator>::difference_type difference_type;
		typedef std::size_t size_type;

	protected:
		allocator_type _allocator;
		pointer _container;
		size_type _capacity;
		size_type _size;

	public:
		explicit vector(const allocator_type &alloc = allocator_type()) :
				_allocator(alloc),
				_container(nullptr),
				_capacity(0),
				_size(0) {}

		explicit vector(size_type n, const value_type &val = value_type(),
						const allocator_type &alloc = allocator_type()) :
				_allocator(alloc),
				_container(nullptr),
				_capacity(0),
				_size(0) {
			this->assign(n, val);
		}


		template<typename U>
		vector(U first, U last, const allocator_type &alloc = allocator_type(),
			   typename ft::enable_if<!ft::is_integral<U>::value, U>::type * = 0) : _allocator(alloc),
																					_container(nullptr), _capacity(0),
																					_size(0) {
			this->template assign(first, last);
		}

		vector(const vector &other) : _allocator(other._allocator), _container(nullptr), _capacity(0), _size(0) {
			this->template assign(other.begin(), other.end());
		}

		vector &operator=(const vector &other) {
			this->template assign(other.begin(), other.end());
			return *this;
		}

		virtual ~vector() {
			this->clear();
			this->deallocate();
		}

		void clear() {
			for (size_type i = 0; i < _size; i++)
				_allocator.destroy(&_container[i]);
			_size = 0;
		}

		template<typename U>
		void assign(U first, U last, typename ft::enable_if<!ft::is_integral<U>::value, U>::type * = 0) {

			this->clear();

			for (U tmp = first; tmp != last; tmp++)
				_size++;

			if (_size > _capacity) {
				this->deallocate();
				_capacity = _size;
				_container = _allocator.allocate(_capacity);
			}

			size_type i = 0;
			while (first != last) {
				_allocator.construct(_container + i, *first);
				first++;
				i++;
			}
		}

		void assign(size_type n, const value_type &val) {

			this->clear();
			_size = n;
			if (_size > _capacity) {
				this->deallocate();
				_capacity = _size;
				_container = _allocator.allocate(_capacity);
			}

			size_type i = 0;
			while (i < _size) {
				_allocator.construct(_container + i, val);
				i++;
			}

		}

		size_type max_size() const {
			return _allocator.max_size();
		}

		void reserve(size_type n) {

			if (n > max_size())
				throw std::length_error("The vector _capacity cannot be greater than _allocator max _size.");
			if (n <= _capacity) return;

			pointer tmp = _allocator.allocate(n);
			for (size_type i = 0; i < _size; ++i) {
				_allocator.construct(tmp + i, _container[i]);
				_allocator.destroy(&_container[i]);
			}
			this->deallocate();
			_capacity = n;
			_container = tmp;
		}

		iterator begin() {
			return iterator(_container);
		}

		const_iterator begin() const {
			return const_iterator(_container);
		}

		iterator end() {
			return iterator(_container + _size);
		}

		const_iterator end() const {
			return const_iterator(_container + _size);
		}

		reverse_iterator rbegin() {
			return reverse_iterator(_container + _size);
		}

		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(_container + _size);
		}

		reverse_iterator rend() {
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const {
			return const_reverse_iterator(begin());
		}

		size_type size() const {
			return _size;
		}

		void resize(size_type n, value_type val = value_type()) {
			if (n <= _size) {
				for (size_type i = n; i < _size; ++i)
					_allocator.destroy(&_container[i]);
				_size = n;
			}

			this->reserve(n);

			if (n > _size) {
				for (size_type i = _size; i < n; ++i)
					_allocator.construct(_container + i, val);
				_size = n;
			}
		}

		size_type capacity() const {
			return _capacity;
		}

		bool empty() const {
			return _size == 0;
		}

		reference operator[](size_type n) {
			if (n >= _size) return *end();
			return _container[n];
		}

		const_reference operator[](size_type n) const {
			if (n >= _size) return *end();
			return _container[n];
		}

		reference at(size_type n) {
			if (n >= _size)
				throw std::out_of_range("Index cannot be greater than or equal to vector size.");
			return _container[n];
		}

		const_reference at(size_type n) const {
			if (n >= _size)
				throw std::out_of_range("Index cannot be greater than or equal to vector size.");
			return _container[n];
		}

		reference front() {
			return _container[0];
		}

		const_reference front() const {
			return _container[0];
		}

		reference back() {
			return _container[_size - 1];
		}

		const_reference back() const {
			return _container[_size - 1];
		}

		void push_back(const value_type &val) {
			this->reserve(_size + 1);
			_allocator.construct(_container + _size, val);
			_size += 1;
		}

		void pop_back() {
			this->resize(_size - 1);
		}

		iterator insert(iterator position, const value_type &val) {
			size_type i = position - begin();
			this->insert(position, 1, val);
			return iterator(_container + i);
		}

		void insert(iterator position, size_type n, const value_type &val) {
			vector<T> tmp(*this);

			size_type i = position - begin();

			this->reserve(_size + n);

			for (size_type j = i; j < i + n; ++j)
				_allocator.construct(_container + j, val);
			for (size_type j = i + n; j < _size + n; ++j)
				_allocator.construct(_container + j, tmp[j - n]);

			_size += n;
		}

		template<class U>
		void
		insert(iterator position, U first, U last, typename ft::enable_if<!ft::is_integral<U>::value, U>::type * = 0) {
			vector tmp(*this);

			size_type n = 0;
			for (U it = first; it != last; it++)
				n++;

			size_type i = position - begin();

			this->reserve(_size + n);

			_size += n;

			for (size_type j = i; j < i + n; ++j)
				_allocator.construct(_container + j, *first++);

			for (size_type j = i + n; j < _size; ++j)
				_allocator.construct(_container + j, tmp[j - n]);
		}

		iterator erase(iterator position) {

			size_type i = position - begin();

			for (size_type j = i; j < _size; ++j) {
				_allocator.destroy(&_container[j]);
				if (j + 1 < _size) _allocator.construct(_container + j, _container[j + 1]);
			}

			_size -= 1;
			return position;
		}

		iterator erase(iterator first, iterator last) {

			vector tmp(*this);

			size_type i = first - begin();
			size_type n = last - first;

			for (size_type j = i; j < _size; ++j)
				_allocator.destroy(&_container[j]);

			for (size_type j = i + n; j < _size; ++j)
				_allocator.construct(_container + (j - n), tmp[j]); // 1 2 3 4 5 6 7 8 9 10 11 12
			// 4 5 6 7 8 9 10 11 12

			_size -= n;
			return first;
		}

		void swap(vector &other) {
			ft::swap(_container, other._container);
			ft::swap(_capacity, other._capacity);
			ft::swap(_size, other._size);
		}

		allocator_type get_allocator() const {
			return _allocator;
		}

	private:

		void deallocate() {
			if (_capacity) _allocator.deallocate(_container, _capacity);
		}

	};

	template<class T, class Alloc>
	bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		if (lhs.size() != rhs.size()) return false;

		for (size_t i = 0; i < lhs.size(); i++)
			if (lhs[i] != rhs[i])
				return false;

		return true;
	}

	template<class T, class Alloc>
	bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return !(lhs == rhs);
	}

	template<class T, class Alloc>
	bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<class T, class Alloc>
	bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return !(rhs < lhs);
	}

	template<class T, class Alloc>
	bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return rhs < lhs;
	}

	template<class T, class Alloc>
	bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return !(lhs < rhs);
	}

	template<class T, class Alloc>
	void swap(vector<T, Alloc> &x, vector<T, Alloc> &y) {
		x.swap(y);
	}

}

#endif
