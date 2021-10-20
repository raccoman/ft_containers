#ifndef FT_CONTAINERS_STACKTESTER_HPP
#define FT_CONTAINERS_STACKTESTER_HPP

#include <testing/tester.hpp>
#include <stack>
#include <stack.hpp>

namespace stack {

	void testConstructors() {
		std::stack<char, std::vector<char> > oDefault;
		ft::stack<char> ftDefault;

		oDefault.push('A');
		oDefault.push('A');
		ftDefault.push('A');
		ftDefault.push('A');

		startTest("Constructors");
		test(oDefault.size() == ftDefault.size());
		endTest();
	}

	void testDestructors() {
		std::stack<char, std::vector<char> > oDefault;
		ft::stack<char> ftDefault;
		startTest("Destructor");
		test(1);
		endTest();
	}

	void testAssignOperator() {

		std::stack<char, std::vector<char> > o1;
		std::stack<char, std::vector<char> > o2;

		ft::stack<char> ft1;
		ft::stack<char> ft2;

		o2.push('A');
		o2.push('A');
		ft2.push('A');
		ft2.push('A');

		o1 = o2;
		ft1 = ft2;

		startTest("Assign Operator");
		test(o1.size() == ft1.size());
		endTest();
	}

	void testCapacity() {
		std::stack<char, std::vector<char> > o;
		ft::stack<char> ft;

		o.push('A');
		o.push('B');
		ft.push('A');
		ft.push('C');

		startTest("Capacity");

		test(o.size() == ft.size());
		test(o.empty() == ft.empty());

		o.pop();
		ft.pop();

		test(o.size() == ft.size());
		test(o.empty() == ft.empty());

		char oC = o.top();
		char ftC = ft.top();

		test(oC == ftC);
		test(o.size() == ft.size());
		test(o.empty() == ft.empty());

		endTest();
	}
}

#endif
