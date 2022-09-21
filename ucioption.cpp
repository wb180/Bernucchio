#include "ctg.h"
#include "ucioption.h"

const std::string &UCIOption::name() const
{
    return mName;
}

const OptionVariant &UCIOption::value() const
{
    return mValue;
}

bool UCIOption::setValue(const OptionVariant &newValue)
{
    mValue = newValue;
    return true;
}

UCIOption::UCIOption(const std::string_view name, const OptionVariant &value) : mName{name},
    mValue{value}
{}

std::ostream &operator<<(std::ostream &out, const UCIOption &option)
{
    out << "option name " << option.name() << " type ";

    if(auto res = std::get_if<bool>(&option.value()))
        std::cout << "check default " << std::boolalpha << *res;
    else if(auto res = std::get_if<int>(&option.value()))
        std::cout << "spin default " << *res;
    else if(auto res = std::get_if<std::string>(&option.value()))
        std::cout << "string default " << ((*res).empty() ? "<empty>" : *res);

    return out;
}

CTGBookOption::CTGBookOption(const std::string_view &name, const OptionVariant &value) : UCIOption{name, value}
{}

bool CTGBookOption::setValue(const OptionVariant &newValue)
{
    if(auto name = std::get_if<std::string>(&newValue))
        CTGReader::GetInstance(*name);
    else
        return false;

    return UCIOption::setValue(newValue);
}
