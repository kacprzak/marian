/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "ScriptMgr.h"
#include <iostream>

ScriptMgr::ScriptMgr()
{
    L = luaL_newstate();

    std::cout << "ScriptMgr created\n";
}

ScriptMgr::~ScriptMgr()
{
    lua_close(L);

    std::cout << "ScriptMgr destroyed\n";
}

//------------------------------------------------------------------------------

void ScriptMgr::setDataFolder(const std::string& folder)
{
    dataFolder = folder;
}

//------------------------------------------------------------------------------

bool ScriptMgr::addScript(const std::string& filename)
{
    std::string fullpath = dataFolder + filename;
    if (luaL_dofile(L, fullpath.c_str())) {
        throw ScriptMgrError(lua_tostring(L, -1));
    }

    std::cout << "Loaded script from: " << fullpath << std::endl;
    return true;
}

//------------------------------------------------------------------------------

int ScriptMgr::getGlobalInt(const std::string& varname)
{
    lua_getglobal(L, varname.c_str());
    if (!lua_isnumber(L, -1)) {
        throw ScriptMgrError(varname + " should be a number.");
    }

    return lua_tonumber(L, -1);
}

bool ScriptMgr::getGlobalBool(const std::string& varname)
{
    lua_getglobal(L, varname.c_str());
    if (!lua_isboolean(L, -1)) {
        throw ScriptMgrError(varname + " should be a boolean.");
    }

    return lua_toboolean(L, -1);
}
