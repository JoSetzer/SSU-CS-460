//
// Created by Julian Contreras on 10/22/26.
//

#include "ValueDescriptor.h"
#include <sstream>

/** IntegerValueDescriptor Implementation
 * constructor takes in a int64_t value
 * type() returns the enum Type "integer"
 * typeName() returns string "integer"
 * toString() returns a string conversion of the integerValue
 * value() returns the integerValue
 */
IntegerValueDescriptor::IntegerValueDescriptor(std::int64_t value) : integerValue(value){}


IntegerValueDescriptor::Type IntegerValueDescriptor::type() const {
    return Type::integer;
}

std::string IntegerValueDescriptor:: typeName() const{
    return "integer";
}

std::string IntegerValueDescriptor::toString() const {
    return std::to_string(integerValue);
}

ValueDescriptor *IntegerValueDescriptor::clone() const {
    ValueDescriptor * cloned = new IntegerValueDescriptor(integerValue);
    return cloned;
}

std::int64_t IntegerValueDescriptor::value() const {
    return integerValue;
}

/** DoubleValueDescriptor Implementation
 * constructor takes in a double value
 * type() returns the enum Type "doubleValue"
 * typeName() returns string "double"
 * toString() returns a string conversion of the doubleValue
 * value() returns the doubleValue
 */
DoubleValueDescriptor::DoubleValueDescriptor(double value) : doubleValue(value){}

DoubleValueDescriptor::Type DoubleValueDescriptor::type() const {
    return Type::doubleValue;
}

std::string DoubleValueDescriptor::typeName() const {
    return "double";
}

std::string DoubleValueDescriptor::toString() const {
    std::ostringstream output;
    output << doubleValue;
    return output.str();
}

ValueDescriptor *DoubleValueDescriptor::clone() const {
    ValueDescriptor * cloned = new DoubleValueDescriptor(doubleValue);
    return cloned;
}

double DoubleValueDescriptor::value() const {
    return doubleValue;
}

/** BooleanValueDescriptor Implementation
 * constructor takes in a bool value
 * type() returns the enum Type "boolean"
 * typeName() returns string "boolean"
 * toString() returns a string conversion of the booleanValue
 * value() returns the booleanValue
 */
BooleanValueDescriptor::BooleanValueDescriptor(bool value) : booleanValue(value){}

BooleanValueDescriptor::Type BooleanValueDescriptor::type() const {
    return Type::boolean;
}

std::string BooleanValueDescriptor::typeName() const {
    return "boolean";
}

std::string BooleanValueDescriptor::toString() const {
    	if (booleanValue) return "True";
	return "False";
}

ValueDescriptor *BooleanValueDescriptor::clone() const {
    ValueDescriptor *cloned = new BooleanValueDescriptor(booleanValue);
    return cloned;
}

bool BooleanValueDescriptor::value() const {
    return booleanValue;
}

/** StringValueDescriptor Implementation
 * constructor takes in a string value
 * type() returns the enum Type "string"
 * typeName() returns string "string"
 * toString() returns the stringValue
 * value() returns the stringValue
 */
StringValueDescriptor::StringValueDescriptor(std::string value) : stringValue(value){}

StringValueDescriptor::Type StringValueDescriptor::type() const {
    return Type::string;
}
std::string StringValueDescriptor::typeName() const {
    return "string";
}
std::string StringValueDescriptor::toString() const {
    return stringValue;
}
ValueDescriptor *StringValueDescriptor::clone() const {
    ValueDescriptor * cloned = new StringValueDescriptor(stringValue);
    return cloned;
}

const std::string &StringValueDescriptor::value() const {
    return stringValue;
}
