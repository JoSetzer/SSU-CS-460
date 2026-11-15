//
// Created by Julian Contreras on 10/22/26.
//

#include "ValueDescriptor.h"
#include <sstream>
#include <stdexcept>

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
/** ListValueDescriptor Implementation
 * constructor takes in no parameters or a pointer to ListData
 * type() returns the enum Type "list"
 * typeName() returns string "list"
 * toString() returns the stringValue of all list objects
 */
ListValueDescriptor::ListValueDescriptor () : data(new ListData()) {}

ListValueDescriptor::ListValueDescriptor (ListData* copyData) : data(copyData) {
	data->referenceCount++;
}

ListValueDescriptor::~ListValueDescriptor () {
	data->referenceCount--;
	if (data->referenceCount == 0) {
		for (ValueDescriptor* index : data->elements) {
			delete index;
		}
		delete data;
	}
}

ListValueDescriptor::Type ListValueDescriptor::type() const {
	return Type::list;
}

std::string ListValueDescriptor::typeName() const {
    return "list";
}

std::string ListValueDescriptor::toString() const {
	std::string outputString = "[";

	for (std::size_t i = 0; i < data->elements.size(); i++) {
		ValueDescriptor* element = data->elements[i];
		if (i > 0)
			outputString += ", ";
		if (element->type() == ValueDescriptor::Type::string)
			outputString += "\"" + element->toString() + "\"";
		else
			outputString += element->toString();

	}
	outputString += "]";
	return outputString;
}

ValueDescriptor* ListValueDescriptor::clone() const {
	return new ListValueDescriptor(data);
}

void ListValueDescriptor::append(ValueDescriptor* value) {
	if (!checkCompatibility(value))
		throw std::runtime_error("cannot append" + value->typeName() + "to a non-" + value->typeName() +" list");
	
	data->elements.push_back(value);

	if (data->listType == ListObjectType::untyped){
		if (value->type() == Type::integer || value->type() == Type::doubleValue)
			data->listType = ListObjectType::numeric;
		if (value->type() == Type::string)
			data->listType = ListObjectType::string;
		if (value->type() == Type::boolean)
			data->listType = ListObjectType::boolean;
	}
}

ValueDescriptor* ListValueDescriptor::pop() {
	if (data->elements.empty())
		throw std::runtime_error("Cannot pop from empty list");
	
	ValueDescriptor* back = data->elements.back();
	data->elements.pop_back();
	return back;
}

ValueDescriptor* ListValueDescriptor::getElement(std::size_t index) const {
	if (data->elements.size() <= index)
		throw std::runtime_error("Index out of range");
	return data->elements[index]->clone(); //list owns object return clone instead
}

void ListValueDescriptor::setElement(std::size_t index, ValueDescriptor* value) {
	if (data->elements.size() <= index)
		throw std::runtime_error("Index out of range");
	
	if (!checkCompatibility(value))
		throw std::runtime_error("cannot append" + value->typeName() + "to a non-" + value->typeName() +" list");
	
	delete data->elements[index];
	data->elements[index] = value;
}

std::size_t ListValueDescriptor::size() const{
	return data->elements.size();
}

bool ListValueDescriptor::equals(const ListValueDescriptor& other) const{
	if (size() != other.size())
		return false;
	for (std::size_t i = 0; i < size(); i++) {
		const ValueDescriptor* left = data->elements[i];
		const ValueDescriptor* right = other.data->elements[i];
		Type lType = left->type();
		Type rType = right->type();
		
		if ((lType == Type::integer || lType == Type::doubleValue)
		 && (rType == Type::integer || rType == Type::doubleValue)) {
				
			if (lType == Type::integer && rType == Type::integer) {
				if (static_cast<const IntegerValueDescriptor*>(left)->value()
			 	 != static_cast<const IntegerValueDescriptor*>(right)->value())
					return false;
			} else {
				double l,r;
				if (lType == Type::integer) {
					const IntegerValueDescriptor* integer = static_cast<const IntegerValueDescriptor*>(left);
					l = static_cast<double>(integer->value());
				}
				else {
					const DoubleValueDescriptor* doubleValue = static_cast<const DoubleValueDescriptor*>(left);
					l = doubleValue->value();
				}
				
				if (rType == Type::integer) {
					const IntegerValueDescriptor* integer = static_cast<const IntegerValueDescriptor*>(right);
					r = static_cast<double>(integer->value());
				}
				else {
					const DoubleValueDescriptor* doubleValue = static_cast<const DoubleValueDescriptor*>(right);
					r = doubleValue->value();
				}

				if (l !=r )
					return false;
			}
			continue;
		}

		if (lType != rType)
			return false;

		if (lType == Type::string){
			if (static_cast<const StringValueDescriptor*>(left)->value()
			 != static_cast<const StringValueDescriptor*>(right)->value())
				return false;
		}
		else if (lType == Type::boolean){
			if (static_cast<const BooleanValueDescriptor*>(left)->value()
			 != static_cast<const BooleanValueDescriptor*>(right)->value())
				return false;
	
		} 
		else
			return false;
	}
	return true;
}

bool ListValueDescriptor::checkCompatibility(const ValueDescriptor* value) const {
	if (value->type() == Type::list)
		return false;
	if (data->listType == ListObjectType::untyped)
		return true;
	if (data->listType == ListObjectType::numeric)
		return value->type() == Type::integer || value->type() == Type::doubleValue;
	if (data->listType == ListObjectType::string)
		return value->type() == Type::string;
	if (data->listType == ListObjectType::boolean)
		return value->type() == Type::boolean;
	return false;
}
