/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef SCRIPT_MGR_H
#define SCRIPT_MGR_H

#include "Singleton.h"

#include <lua.hpp>

#include <stdexcept>
#include <string>
#include <list>
#include <memory>
#include <functional>

using ScriptListener = std::function<void (const std::string& msg)>;

//------------------------------------------------------------------------------

class ScriptError : public std::runtime_error
{
 public:
    ScriptError(const std::string& msg)
        : std::runtime_error(msg)
    {}
};

//------------------------------------------------------------------------------

class ScriptMgr : public Singleton<ScriptMgr>
{
 public:
    enum class OutputType {
        OUT,
        ERR,
    };

    ScriptMgr();
    ~ScriptMgr() override;

    // Set data folder ex: "scripts/"
    void setDataFolder(const std::string& folder);

    // Scripts management
    bool executeString(const std::string& code);
    bool executeFile(const std::string& filename);

    int  getGlobalInt(const std::string& varname);
    bool getGlobalBool(const std::string& varname);
    const char *getGlobalString(const std::string& varname);

    // todo: Maybe they should be weak_ptr's
    void addListener(OutputType ot, std::shared_ptr<ScriptListener> listener);
    void removeListener(OutputType ot, std::shared_ptr<ScriptListener> listener);

    void notifyListeners(OutputType ot, const std::string& msg);

 private:
    using ListenersList = std::list<std::shared_ptr<ScriptListener>>;

    ListenersList& listenersForOutput(OutputType); 

    std::string dataFolder;
    lua_State *L;
    ListenersList m_outListeners;
    ListenersList m_errListeners;
};

//------------------------------------------------------------------------------

#endif
