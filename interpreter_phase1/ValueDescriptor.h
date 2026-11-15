//
// Created by Julian Contreras on 10/22/26.
//

#ifndef EXPRINTER_VALUEDESCRIPTOR_H
#define EXPRINTER_VALUEDESCRIPTOR_H


#include <cstdint>
#include <string>
#include <vector>
#include <cstddef>

class ValueDescriptor {
public:
    enum class Type {
	list,
	integer,
        doubleValue,
        boolean,
        string
    };

    virtual ~ValueDescriptor() = default;

    [[nodiscard]] virtual Type type() const = 0;
    [[nodiscard]] virtual std::string typeName() const = 0;
    [[nodiscard]] virtual std::string toString() const = 0;
    [[nodiscard]] virtual ValueDescriptor *clone() const = 0;
};

class IntegerValueDescriptor final : public ValueDescriptor {
public:
    explicit IntegerValueDescriptor(std::int64_t value);

    [[nodiscard]] Type type() const override;
    [[nodiscard]] std::string typeName() const override;
    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] ValueDescriptor *clone() const override;
    [[nodiscard]] std::int64_t value() const;

private:
    std::int64_t integerValue;
};

class DoubleValueDescriptor final : public ValueDescriptor {
public:
    explicit DoubleValueDescriptor(double value);

    [[nodiscard]] Type type() const override;
    [[nodiscard]] std::string typeName() const override;
    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] ValueDescriptor *clone() const override;
    [[nodiscard]] double value() const;

private:
    double doubleValue;
};

class BooleanValueDescriptor final : public ValueDescriptor {
public:
    explicit BooleanValueDescriptor(bool value);

    [[nodiscard]] Type type() const override;
    [[nodiscard]] std::string typeName() const override;
    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] ValueDescriptor *clone() const override;
    [[nodiscard]] bool value() const;

private:
    bool booleanValue;
};

class StringValueDescriptor final : public ValueDescriptor {
public:
    explicit StringValueDescriptor(std::string value);

    [[nodiscard]] Type type() const override;
    [[nodiscard]] std::string typeName() const override;
    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] ValueDescriptor *clone() const override;
    [[nodiscard]] const std::string &value() const;

private:
    std::string stringValue;
};

class ListValueDescriptor final : public ValueDescriptor {
    enum class ListObjectType {
        untyped,
        numeric,
        boolean,
        string
    };

public:
    	explicit ListValueDescriptor();
    	~ListValueDescriptor();

    	[[nodiscard]] Type type() const;
	[[nodiscard]] std::string typeName() const;
    	[[nodiscard]] std::string toString() const;
	[[nodiscard]] ValueDescriptor *clone() const override;

	void append(ValueDescriptor* value);
	ValueDescriptor* pop();
	ValueDescriptor* getElement(std::size_t index) const;
	void setElement(std::size_t index, ValueDescriptor* value);
	std::size_t size() const;
private:
	bool checkCompatibility (const ValueDescriptor* value) const;

	struct ListData {
		ListObjectType listType = ListObjectType::untyped;
		std::vector<ValueDescriptor*> elements;
		std::size_t referenceCount = 1;
	};

	ListData* data;

    	explicit ListValueDescriptor(ListData* copyData); 
};

#endif //EXPRINTER_VALUEDESCRIPTOR_H
