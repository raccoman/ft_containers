#ifndef FT_CONTAINERS_MAP_HPP
#define FT_CONTAINERS_MAP_HPP

#include <algorithm.hpp>
#include <rbtree.hpp>

namespace ft {

	template<typename T>
	class map_iterator {

	public:
		typedef T                                                     		iterator_type;
		typedef typename iterator_traits<iterator_type>::iterator_category	iterator_category;
		typedef typename iterator_traits<iterator_type>::value_type        	value_type;
		typedef typename iterator_traits<iterator_type>::difference_type   	difference_type;
		typedef typename iterator_traits<iterator_type>::pointer           	pointer;
		typedef typename iterator_traits<iterator_type>::reference         	reference;

	protected:
		pointer	ptr;

	public:
		map_iterator() : ptr(nullptr) {}
		map_iterator(pointer ptr) : ptr(ptr) {}
		map_iterator(const map_iterator& other) : ptr(other.ptr) {}
		virtual ~map_iterator() {}

		map_iterator& operator=(const map_iterator& other) { this->ptr = other.ptr; return *this; }
		reference operator*() { return *this->ptr; }
		pointer operator->() { return this->ptr; }
		map_iterator operator++(int) { map_iterator tmp(*this); ++this->ptr; return tmp; }
		map_iterator& operator++() { ++this->ptr; return *this; }
		map_iterator operator--(int) { map_iterator tmp(*this); --this->ptr; return tmp; }
		map_iterator& operator--() { --this->ptr; return *this; }
		bool operator==(const map_iterator& other) const { return this->ptr == other.ptr; }
		bool operator!=(const map_iterator& other) const { return this->ptr != other.ptr; }
	};

	template<
	        class Key,
			class T,
			class Compare = std::less<Key>,
			class Allocator = std::allocator<std::pair<const Key, T> >
	>
	class map {

	public:
		typedef Key										key_type;
		typedef T										mapped_type;
		typedef ft::pair<const Key, T>					value_type;
		typedef std::size_t								size_type;
		typedef std::ptrdiff_t							difference_type;
		typedef Compare									key_compare;
		typedef Allocator								allocator_type;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;
		typedef map_iterator<value_type>				iterator;
		typedef map_iterator<const value_type>			const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

	protected:


	};

}

#endif
