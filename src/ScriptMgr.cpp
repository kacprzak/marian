/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "ScriptMgr.h"
#include <iostream>

#include "Game.h"
#include "Engine.h"

static int l_addBox(lua_State *L)
{
    // get args from Lua
    float x = luaL_checknumber(L, -2);
    float y = luaL_checknumber(L, -1);

    Game *currGame = static_cast<Game *>(Engine::singleton().game());
    currGame->addGameObject(BOX, "box", x, y);

    return 0;
} 

//------------------------------------------------------------------------------

ScriptMgr::ScriptMgr()
{
    L = luaL_newstate();

    lua_register(L, "addBox", l_addBox);

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

bool ScriptMgr::executeScript(const std::string& filename)
{
    std::string fullpath = dataFolder + filename;
    if (luaL_dofile(L, fullpath.c_str())) {
        throw ScriptMgrError(lua_tostring(L, -1));
    }

    std::cout << "Executed script from: " << fullpath << std::endl;
    return true;
}

//------------------------------------------------------------------------------

int ScriptMgr::getGlobalInt(const std::string& varname)
{
    lua_getglobal(L, varname.c_str());
    if (!lua_isnumber(L, -1)) {
        throw ScriptMgrError(varname + " should be a number.");
    }
    
    int retVal = lua_tonumber(L, -1);
    lua_pop(L, 1);

    return retVal;
}

//------------------------------------------------------------------------------

bool ScriptMgr::getGlobalBool(const std::string& varname)
{
    lua_getglobal(L, varname.c_str());
    if (!lua_isboolean(L, -1)) {
        throw ScriptMgrError(varname + " should be a boolean.");
    }

    bool retVal = lua_toboolean(L, -1);
    lua_pop(L, 1);

    return retVal;
}
