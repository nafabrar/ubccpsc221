//! @file Unit.cc 
//! Definitions of Unit class functions. 
#include "Unit.h"
#include <iostream>
#include <sstream>
#include <cassert> 

Unit::Unit() {
}

Unit::~Unit() {
}

/**
 * Verify that the expected int value equals the actual int value. 
 *
 * If it is, push a success message onto the back of the successes vector<string> 
 * and return true. 
 * Otherwise, push an error message onto the back of the failures vector<string> 
 * and return false. 
 * 
 * @param msg = string to use for the beginning of message  
 * @param expected = the expected int value 
 * @param actual = the actual int value 
 * @returns true if (expected EQ actual), false otherwise 
 */ 
bool Unit::assertEquals(std::string msg, int expected, int actual) {
    if (expected != actual) {
        std::stringstream fmt;
        fmt << msg << ": Expected <" << expected << "> but was <" << actual << ">";
        failures.push_back(fmt.str());
        return false; 
    }
    else { 
        std::stringstream fmt;
        fmt << msg << ": Expected <" << expected << "> and was <" << actual << ">";
        successes.push_back(fmt.str());
        return true; 
    }
}

/**
 * Verify that left-hand-side (lhs) LT right-hand-side (rhs)
 *
 * If it is, push a success message onto the back of the successes vector<string> 
 * and return true. 
 * Otherwise, push an error message onto the back of the failures vector<string> 
 * and return false. 
 * 
 * @param msg = string to use for the beginning of message  
 * @param lhs = the left-hand-side int value 
 * @param rhs = the right-hand-side int value 
 * @returns true if (lhs LT rhs), false otherwise 
 */ 
bool Unit::assert_LT(std::string msg, int lhs, int rhs) {
    if (lhs >= rhs) {
        std::stringstream fmt;
        fmt << msg << ": Expected " << lhs << " less than " << rhs;
        failures.push_back(fmt.str());
        return false; 
    }
    else { 
        std::stringstream fmt;
        fmt << msg << ": Verified " << lhs << " less than " << rhs;
        successes.push_back(fmt.str());
        return true; 
    }
}
/**
 * Insist that left-hand-side (lhs) LT right-hand-side (rhs) -- abort if it isn't
 *
 * If it is, push a success message onto the back of the successes vector<string> 
 * and return true. 
 * Otherwise, push an error message onto the back of the failures vector<string>, 
 * report all errors, and abend 
 * 
 * @param msg = string to use for the beginning of message  
 * @param lhs = the left-hand-side int value 
 * @param rhs = the right-hand-side int value 
 * @returns true if (lhs LT rhs), abends otherwise 
 */ 
bool Unit::insist_LT(std::string msg, int lhs, int rhs) {
    if (lhs >= rhs) {
        std::stringstream fmt;
        fmt << msg << ": Expected " << lhs << " less than " << rhs;
        failures.push_back(fmt.str());
        // if test fails, then reportResults() and abend 
        printResults(); 
        assert( false  ); 
    }
    else { 
        std::stringstream fmt;
        fmt << msg << ": Verified " << lhs << " less than " << rhs;
        successes.push_back(fmt.str());
    }
    return true; 
}

/**
 * Verify that left-hand-side (lhs) LE right-hand-side (rhs)
 *
 * If it is, push a success message onto the back of the successes vector<string> 
 * and return true. 
 * Otherwise, push an error message onto the back of the failures vector<string> 
 * and return false. 
 * 
 * @param msg = string to use for the beginning of message  
 * @param lhs = the left-hand-side int value 
 * @param rhs = the right-hand-side int value 
 * @returns true if (lhs LE rhs), false otherwise 
 */ 
bool Unit::assert_LE(std::string msg, int lhs, int rhs) {
    if (lhs > rhs) {
        std::stringstream fmt;
        fmt << msg << ": Expected " << lhs << " less than or equal " << rhs;
        failures.push_back(fmt.str());
        return false; 
    }
    else { 
        std::stringstream fmt;
        fmt << msg << ": Verified " << lhs << " less than or equal " << rhs;
        successes.push_back(fmt.str());
        return true; 
    }
}

/**
 * Verify that the expected vector<int> equals the actual vector<int>
 *
 * If it is, push a success message onto the back of the successes vector<string> 
 * and return true. 
 * Otherwise, push an error message onto the back of the failures vector<string> 
 * and return false. 
 * 
 * @param msg = string to use for the beginning of message  
 * @param expected = a reference to the expected vector<int>
 * @param actual = a reference to the actual vector<int>
 * @returns true if (expected.size() EQ actual.size() AND all elements are equal), false otherwise 
 */ 
bool Unit::assertvectorEquals(std::string msg, std::vector<int>& expected, std::vector<int>& actual) {
    bool fail = expected.size() != actual.size();
    for(unsigned int i=0; i < expected.size() && !fail; i++) {
        fail = expected[i] != actual[i];
    }
    if (fail) {
        std::stringstream fmt;
        fmt << msg << ": Expected\n";
        fmt << toStr(expected);
        fmt << "\nbut was\n";
        fmt << toStr(actual);
        failures.push_back(fmt.str());
        return false; 
    }
    else { 
        std::stringstream fmt;
        fmt << msg << ": Verified ";
        fmt << toStr(expected);
        successes.push_back(fmt.str());
        return true; 
    }
}

/**
 * Verify that actual is not NULL.
 *
 * If it isn't, push a success message onto the back of the successes vector<string> 
 * and return true. 
 * Otherwise, push an error message onto the back of the failures vector<string> 
 * and return false. 
 * 
 * @param msg = string to use for the beginning of message  
 * @param actual = the pointer value to verify
 * @returns true if (actual NE NULL), false otherwise 
 */ 
bool Unit::assertNonNull(std::string msg, void* actual) {
    if (actual == NULL) {
        std::stringstream fmt;
        fmt << msg << ": Expected non-null value.";
        failures.push_back(fmt.str());
        return false; 
    }
    else { 
        std::stringstream fmt;
        fmt << msg << ": Verified non-null value.";
        successes.push_back(fmt.str());
        return true; 
    }
}
/**
 * Insist that actual is not NULL, and abort if it is. 
 *
 * If it isn't, push a success message onto the back of the successes vector<string> 
 * and return true. 
 * Otherwise, push an error message onto the back of the failures vector<string>, 
 * print all results, and assert( actual != NULL) -- which will fail  
 * 
 * @param msg = string to use for the beginning of message  
 * @param actual = the pointer value to verify
 * @returns true if (actual NE NULL), otherwise abends  
 */ 
bool Unit::insistNonNull(std::string msg, void* actual) {
// this is the same as assertNonNULL, except... 
    if (actual == NULL) {
        std::stringstream fmt;
        fmt << msg << ": Expected non-null value.";
        failures.push_back(fmt.str());
        // if test fails, then reportResults() and abend 
        printResults(); 
        assert( false  ); 
    }
    else { 
        std::stringstream fmt;
        fmt << msg << ": Verified non-null value.";
        successes.push_back(fmt.str());
    }
    return true; 
}

/**
 * Verify that actual is NULL.
 *
 * If it is, push a success message onto the back of the successes vector<string> 
 * and return true. 
 * Otherwise, push an error message onto the back of the failures vector<string> 
 * and return false. 
 * 
 * @param msg = string to use for the beginning of message  
 * @param actual = the pointer value to verify
 * @returns true if (actual EQ NULL), false otherwise 
 */ 
bool Unit::assertNull(std::string msg, void* actual) {
    if (actual != NULL) {
        std::stringstream fmt;
        fmt << msg << ": Expected null value.";
        failures.push_back(fmt.str());
        return false; 
    }
    else {
        std::stringstream fmt;
        fmt << msg << ": Verified null value.";
        successes.push_back(fmt.str());
        return true; 
    }
}

/**
 * Verify that actual is true.
 *
 * If it is, push a success message onto the back of the successes vector<string> 
 * and return true. 
 * Otherwise, push an error message onto the back of the failures vector<string> 
 * and return false. 
 * 
 * @param msg = string to use for the beginning of message  
 * @param actual = the bool value to verify
 * @returns true if (actual NE false), false otherwise 
 */ 
bool Unit::assertTrue(std::string msg, bool actual) {
    if (actual == false) {
        std::stringstream fmt;
        fmt << msg << ": Found false.";
        failures.push_back(fmt.str());
        return false; 
    }
    else {
        std::stringstream fmt;
        fmt << msg << ": Verified true.";
        successes.push_back(fmt.str());
        return true; 
    }
}
/**
 * Insist that actual is true, else abend
 *
 * If it is, push a success message onto the back of the successes vector<string> 
 * and return true. 
 * Otherwise, push an error message onto the back of the failures vector<string> 
 * report all errors and abend 
 * 
 * @param msg = string to use for the beginning of message  
 * @param actual = the bool value to verify
 * @returns true if (actual NE false), false otherwise 
 */ 
bool Unit::insistTrue(std::string msg, bool actual) {
    if (actual == false) {
        std::stringstream fmt;
        fmt << msg << ": Found false.";
        failures.push_back(fmt.str());
        // if test fails, then reportResults() and abend 
        printResults(); 
        assert( false  ); 
    }
    else {
        std::stringstream fmt;
        fmt << msg << ": Verified true.";
        successes.push_back(fmt.str());
    }
    return true; 
}

std::string Unit::toStr(std::vector<int>& v) {
    std::stringstream os;
    os << "[";
    for(unsigned int i=0; i < v.size(); i++) {
        os << v[i];
        if (i < v.size()-1) {
            os << ", ";
        }
    }
    os << "]";
    return os.str();
}

void Unit::printResults() {
    if (failures.empty()) {
        std::cout << "All " << successes.size() << " tests passed.\n";
    } 
    else {
        std::cerr << successes.size() << " successess, but "
                  << failures.size() << " failures:\n\n";
        for(unsigned int i=0; i < failures.size(); i++) {
            std::cerr << i+1 << ". " << failures[i] << "\n\n";
        }
    }
}

/**
 * Returns true iff there are some failures.
 */ 
bool Unit::hasErrors(){ 
    if (failures.size() != 0) return (bool) 1;  
    return (bool) 0; 
}
/**
 * Return false iff both failures and successes vectors are empty. 
 * To force isActive() to be true, assertNull("Forcing active", NULL), or similar.
 */ 
bool Unit::isActive(){ 
    if (failures.size() != 0) return (bool) 1;  
    if (successes.size() != 0) return (bool) 1;  
    return (bool) 0; 
}


