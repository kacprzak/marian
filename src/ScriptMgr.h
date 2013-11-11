/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef SCRIPT_MGR_H
#define SCRIPT_MGR_H

#include "Singleton.h"

#include <lua.hpp>

#include <stdexcept>
#include <string>

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
    ScriptMgr();
    ~ScriptMgr();

    // Set data folder ex: "scripts/"
    void setDataFolder(const std::string& folder);

    // Scripts management
    bool executeString(const std::string& code);
    bool executeFile(const std::string& filename);

    int  getGlobalInt(const std::string& varname);
    bool getGlobalBool(const std::string& varname);

 private:
    std::string dataFolder;

    lua_State *L;
};

//------------------------------------------------------------------------------

#endif
