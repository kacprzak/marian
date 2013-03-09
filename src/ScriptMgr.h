/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef SCRIPT_MGR_H
#define SCRIPT_MGR_H

#include <exception>
#include <boost/utility.hpp>

extern "C" {
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}

//------------------------------------------------------------------------------

class ScriptMgrError : public std::exception
{
 public:
    ScriptMgrError(const std::string& msg) : m_msg(msg) {}
    ~ScriptMgrError() throw() {}

#ifdef _MSC_VER
    const char* what() const //noexcept (MSVC er C3646)
#else
    const char* what() const noexcept
#endif
    {
        return m_msg.c_str();
    }

 private:
    const std::string m_msg;
};

//------------------------------------------------------------------------------

class ScriptMgr : boost::noncopyable
{
 public:
    static ScriptMgr& instance()
    {
        static ScriptMgr s_instance;
        return s_instance;
    }

    ~ScriptMgr();

    // Set data folder ex: "scripts/"
    void setDataFolder(const std::string& folder);

    // Scripts management
    bool executeScript(const std::string& filename);

    int  getGlobalInt(const std::string& varname);
    bool getGlobalBool(const std::string& varname);

 private:
    ScriptMgr();
    std::string dataFolder;

    lua_State *L;
};

//------------------------------------------------------------------------------

#endif
