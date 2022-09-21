#pragma once

#include <iostream>
#include <string>
#include <variant>

using OptionVariant = std::variant<bool, int, std::string>;

class UCIOption
{
public:
    UCIOption(const std::string_view name, const OptionVariant &value);

    ~UCIOption() = default;

    const std::string &name() const;
    const OptionVariant &value() const;
    virtual bool setValue(const OptionVariant &newValue) = 0;

    friend std::ostream &operator<<(std::ostream &out, const UCIOption &option);

private:
    std::string mName;
    OptionVariant mValue;
};

class CTGBookOption : public UCIOption
{
public:
    CTGBookOption(const std::string_view &name, const OptionVariant &value);

    bool setValue(const OptionVariant &newValue);
};
