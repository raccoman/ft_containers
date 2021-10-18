#ifndef FT_CONTAINERS_TESTER_HPP
# define FT_CONTAINERS_TESTER_HPP

# define CC_RESET "\e[0m"
# define CC_RED "\e[0;31m"
# define CC_GRN "\e[0;32m"
# define CC_CYN "\e[0;36m"

# include <iostream>
# include <string>

static int max = 0;
static int point = 0;

void startTest(const std::string name) {
	max = point = 0;
	std::cout << CC_RESET << name << ": ";
}

void test(bool result) {
	max++;
	if (result) {
		std::cout << CC_GRN << "[OK] ";
		point++;
	}
	else
		std::cout << CC_RED << "[KO] ";
}

void endTest() {
	std::cout << CC_RESET << "| " << point << "/" << max << std::endl;
}

#endif
