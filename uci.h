#ifndef UCI_H
#define UCI_H

#include <iostream>
#include <memory>

#include "ucioption.h"

class UCI
{
public:
    UCI();

    void Loop();

    const std::vector<std::unique_ptr<UCIOption>> &options() const;

private:
    std::vector<std::unique_ptr<UCIOption>> mOptions;
};

#endif // UCI_H
