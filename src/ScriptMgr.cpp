/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "ScriptMgr.h"

#include "BaseGameLogic.h"
#include "Engine.h"
#include "Logger.h"

static int l_addBox(lua_State *L)
{
    // get args from Lua
    float x = luaL_checknumber(L, -2);
    float y = luaL_checknumber(L, -1);

    BaseGameLogic *currGame = static_cast<BaseGameLogic *>(Engine::singleton().game());
    currGame->addGameObject(BOX, "box", x, y);

    return 0;
} 

//------------------------------------------------------------------------------

ScriptMgr::ScriptMgr()
{
    L = luaL_newstate();

    // Expose functions to lua code
    lua_register(L, "addBox", l_addBox);

    LOG << "created ScriptMgr\n";
}

//------------------------------------------------------------------------------

ScriptMgr::~ScriptMgr()
{
    lua_close(L);

    LOG << "destroyed ScriptMgr\n";
}

//------------------------------------------------------------------------------

void ScriptMgr::setDataFolder(const std::string& folder)
{
    dataFolder = folder;
}

//------------------------------------------------------------------------------

bool ScriptMgr::executeString(const std::string& code)
{
    if (luaL_dostring(L, code.c_str())) {
        throw ScriptMgrError(lua_tostring(L, -1));
    }

    LOG << "Executed code: " << code << std::endl;
    return true;
}

//------------------------------------------------------------------------------

bool ScriptMgr::executeFile(const std::string& filename)
{
    std::string fullpath = dataFolder + filename;
    if (luaL_dofile(L, fullpath.c_str())) {
        throw ScriptMgrError(lua_tostring(L, -1));
    }

    LOG << "Executed script from: " << fullpath << std::endl;
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

    int retVal = lua_toboolean(L, -1);
    lua_pop(L, 1);

    return ((retVal) ? true : false);
}
