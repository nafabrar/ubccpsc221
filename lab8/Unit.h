//! @file Unit.h 
//! Declaration of Unit class (used for Unit testing).
#ifndef UNIT_H_
#define UNIT_H_

#include <string>
#include <vector>

class Unit {
public:
	Unit();
	virtual ~Unit();

    bool assertEquals(std::string msg, int expected, int actual);
    bool assert_LT(std::string msg, int lhs, int rhs);
    bool assert_LE(std::string msg, int lhs, int rhs);
	bool assertvectorEquals(std::string msg, std::vector<int>& expected, std::vector<int>& actual);
    bool assertNonNull(std::string msg, void* actual);
    bool assertNull(std::string msg, void* actual);
    bool assertTrue(std::string msg, bool actual); 

    bool insistNonNull(std::string msg, void* actual);
    bool insist_LT(std::string msg, int lhs, int rhs);
    bool insistTrue(std::string msg, bool actual); 

	bool hasErrors( void ); 
	bool isActive( void ); 
	void printResults();

private:
	std::vector<std::string> failures;

	std::vector<std::string> successes;

	std::string toStr(std::vector<int>& v);
};

#endif /* UNIT_H_ */
