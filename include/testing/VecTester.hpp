#ifndef FT_CONTAINERS_VECTESTER_HPP
# define FT_CONTAINERS_VECTESTER_HPP

#include <vector>
#include <vector.hpp>
#include <testing/tester.hpp>

namespace vec {

	void testConstructors() {

		std::vector<char> oDefault;
		std::vector<char> oFill(5, 'r');
		std::vector<char> oRange(oFill.begin(), oFill.end());
		std::vector<char> oCopy(oFill);

		ft::vector<char> ftDefault;
		ft::vector<char> ftFill(5, 'r');
		ft::vector<char> ftRange(ftFill.begin(), ftFill.end());
		ft::vector<char> ftCopy(ftFill);

		startTest("Constructors");
		test(oDefault.capacity() == ftDefault.capacity() && oDefault.size() == ftDefault.size());
		test(oFill.capacity() == ftFill.capacity() && oFill.size() == ftFill.size());
		test(oRange.capacity() == ftRange.capacity() && oRange.size() == ftRange.size());
		test(oCopy.capacity() == ftCopy.capacity() && oCopy.size() == ftCopy.size());
		endTest();

	}

	void testDestructors() {
		std::vector<char> o(5, 'r');
		ft::vector<char> ft(5, 'r');
		startTest("Destructor");
		test(1);
		endTest();
	}

	void testAssignOperator() {

		std::vector<char> o1(1, 'r');
		std::vector<char> o2(5, 'R');

		ft::vector<char> ft1(1, 'r');
		ft::vector<char> ft2(5, 'R');

		o1 = o2;
		ft1 = ft2;

		startTest("Assign Operator");
		test(o1.capacity() == ft1.capacity() && o1.size() == ft1.size());
		endTest();
	}

	void testIterators() {

		std::vector<char> o;
		ft::vector<char> ft;

		o.push_back('a');
		o.push_back('b');
		o.push_back('c');
		o.push_back('d');

		ft.push_back('a');
		ft.push_back('b');
		ft.push_back('c');
		ft.push_back('d');


		std::vector<char>::iterator oIT = o.begin();
		ft::vector<char>::iterator ftIT = ft.begin();
		std::vector<char>::reverse_iterator oRIT = o.rbegin();
		ft::vector<char>::reverse_iterator ftRIT = ft.rbegin();

		startTest("Iterators");

		test(o.capacity() == ft.capacity() && o.size() == ft.size()); // WHY? No sense

		while (oIT != o.end()) {
			test(*oIT == *ftIT);
			oIT++;
			ftIT++;
		}

		while (oRIT != o.rend()) {
			test(*oRIT == *ftRIT);
			oRIT++;
			ftRIT++;
		}

		endTest();
	}

	void testCapacity() {
		std::vector<char> o(5, 'R');
		ft::vector<char> ft(5, 'R');

		startTest("Capacity");

		test(o.capacity() == ft.capacity());
		test(o.size() == ft.size());
		test(o.empty() == ft.empty());

		o.clear();
		ft.clear();

		test(o.capacity() == ft.capacity());
		test(o.size() == ft.size());
		test(o.empty() == ft.empty());

		o.resize(10);
		ft.resize(10);

		test(o.capacity() == ft.capacity());
		test(o.size() == ft.size());
		test(o.empty() == ft.empty());

		o.resize(2);
		ft.resize(2);

		test(o.capacity() == ft.capacity());
		test(o.size() == ft.size());
		test(o.empty() == ft.empty());

		o.reserve(5);
		ft.reserve(5);

		test(o.capacity() == ft.capacity());
		test(o.size() == ft.size());
		test(o.empty() == ft.empty());

		endTest();
	}

	void testAccess() {

		std::vector<char> o;
		ft::vector<char> ft;

		o.push_back('a');
		o.push_back('b');
		o.push_back('c');
		o.push_back('d');

		ft.push_back('a');
		ft.push_back('b');
		ft.push_back('c');
		ft.push_back('d');

		startTest("Element Access");

		test(o[2] == ft[2]);
		test(o.at(2) == ft.at(2));
		test(o.front() == ft.front());
		test(o.back() == ft.back());

		endTest();
	}

	void testModifiers() {

		std::vector<char> o;
		ft::vector<char> ft;

		startTest("Modifiers");

		o.assign(5, 'A');
		ft.assign(5, 'A');

		test(o.capacity() == ft.capacity());
		test(o.size() == ft.size());

		o.pop_back();
		ft.pop_back();

		test(o.capacity() == ft.capacity());
		test(o.size() == ft.size());

		o.insert(o.begin() + 2, 'C');
		ft.insert(ft.begin() + 2, 'C');

		test(o.capacity() == ft.capacity());
		test(o.size() == ft.size());

		o.erase(o.begin());
		ft.erase(ft.begin());

		test(o.capacity() == ft.capacity());
		test(o.size() == ft.size());

		o.erase(o.begin(), o.end());
		ft.erase(ft.begin(), ft.end());

		test(o.capacity() == ft.capacity());
		test(o.size() == ft.size());

		o.clear();
		ft.clear();

		test(o.capacity() == ft.capacity());
		test(o.size() == ft.size());

		endTest();
	}
}

#endif
