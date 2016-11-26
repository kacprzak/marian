#ifndef SCRIPT_MGR_H
#define SCRIPT_MGR_H

#include "Singleton.h"

#include <lua.hpp>

#include <functional>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>

class ScriptError : public std::runtime_error
{
  public:
    explicit ScriptError(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
};

//------------------------------------------------------------------------------

class ScriptListener
{
  public:
    virtual ~ScriptListener() = default;

    virtual void onScriptOutput(const std::string& out) = 0;
    virtual void onScriptError(const std::string& out)  = 0;
};

//------------------------------------------------------------------------------

class ScriptMgr : public Singleton<ScriptMgr>
{
  public:
    ScriptMgr();
    ~ScriptMgr() override;

    // Set data folder ex: "scripts/"
    void setDataFolder(const std::string& folder);

    // Scripts management
    bool executeString(const std::string& code);
    bool executeFile(const std::string& filename);

    int getGlobalInt(const std::string& varname);
    bool getGlobalBool(const std::string& varname);
    const char* getGlobalString(const std::string& varname);

    void addListener(ScriptListener* listener);
    void removeListener(ScriptListener* listener);

    void notifyListenersOnOutput(const std::string& msg);
    void notifyListenersOnError(const std::string& msg);

  private:
    using ListenersList = std::list<ScriptListener*>;

    std::string m_dataFolder;
    lua_State* m_L;
    ListenersList m_listeners;
};

//------------------------------------------------------------------------------

#endif
