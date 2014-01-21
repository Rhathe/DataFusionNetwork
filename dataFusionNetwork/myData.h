#ifndef MY_DATA_H
#define MY_DATA_H

#include <vector>
#include <string>

class myData {

	public:
		std::vector<int> order;
		std::string test;
		myData();
		myData(std::vector<int> x);
};

#endif

