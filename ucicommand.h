#pragma once

#include <string_view>
#include <memory>

#include "uci.h"
#include "ucioption.h"

class UCICommand
{
public:
    virtual ~UCICommand() = default;

    virtual bool execute() = 0;
};

class ParamCommand : public virtual UCICommand
{
public:
    explicit ParamCommand(const std::string_view position);
protected:
    std::string_view mParam;
};

class OptionCommand : public virtual UCICommand
{
public:
    explicit OptionCommand(UCI *uci);
protected:
    UCI *mUci;
};

class CommandFactory
{
public:
    static std::shared_ptr<UCICommand> CreateCommand(std::string_view commandString, UCI *uci = nullptr);
};

class CommandUci : public OptionCommand
{
public:
    explicit CommandUci(UCI *uci);

    bool execute() override;
};

class CommandQuit : public UCICommand
{
public:
    bool execute() override;
};

class CommandReady : public UCICommand
{
public:
    bool execute() override;
};

class CommandPosition : public ParamCommand
{
public:
    explicit CommandPosition(const std::string_view position);

    bool execute() override;
};

class CommandGo : public ParamCommand
{
public:
    explicit CommandGo(const std::string_view go);

    bool execute() override;
};

class CommandStop : public UCICommand
{
public:
    bool execute() override;
};

class CommandSpeedTest : public UCICommand
{
public:
    bool execute() override;
};

class CommandSetOption : public OptionCommand, public ParamCommand
{
public:
    explicit CommandSetOption(UCI *uci, std::string_view option);

    bool execute() override;
};
