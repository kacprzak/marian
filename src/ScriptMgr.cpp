/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "ScriptMgr.h"

#include "BaseGameLogic.h"
#include "Engine.h"
#include "Logger.h"
#include "Util.h"

#include <sstream>

/* Functions exposed to Lua */
static int l_addBox(lua_State *L);
static int l_print(lua_State *L);

//------------------------------------------------------------------------------

ScriptMgr::ScriptMgr()
{
    m_L = luaL_newstate();

    // Expose functions to lua code
    lua_register(m_L, "addBox", l_addBox);
    lua_register(m_L, "print", l_print);

    LOG << "created ScriptMgr\n";
}

//------------------------------------------------------------------------------

ScriptMgr::~ScriptMgr()
{
    lua_close(m_L);

    LOG << "destroyed ScriptMgr\n";
}

//------------------------------------------------------------------------------

void ScriptMgr::setDataFolder(const std::string& folder)
{
    m_dataFolder = appendDirSeparator(folder);
}

//------------------------------------------------------------------------------

bool ScriptMgr::executeString(const std::string& code)
{
    if (luaL_dostring(m_L, code.c_str())) {
        const char *errMsg = lua_tostring(m_L, -1);
        notifyListenersOnError(errMsg);
        throw ScriptError(errMsg);
    }

    LOG << "Executed code: " << code << std::endl;
    return true;
}

//------------------------------------------------------------------------------

bool ScriptMgr::executeFile(const std::string& filename)
{
    std::string fullpath = m_dataFolder + filename;
    if (luaL_dofile(m_L, fullpath.c_str())) {
        const char *errMsg = lua_tostring(m_L, -1);
        notifyListenersOnError(errMsg);
        throw ScriptError(errMsg);
    }

    LOG << "Executed script from: " << fullpath << std::endl;
    return true;
}

//------------------------------------------------------------------------------

int ScriptMgr::getGlobalInt(const std::string& varname)
{
    lua_getglobal(m_L, varname.c_str());
    if (!lua_isnumber(m_L, -1)) {
        throw ScriptError(varname + " should be a number.");
    }
    
    int retVal = lua_tonumber(m_L, -1);
    lua_pop(m_L, 1);

    return retVal;
}

//------------------------------------------------------------------------------

bool ScriptMgr::getGlobalBool(const std::string& varname)
{
    lua_getglobal(m_L, varname.c_str());
    if (!lua_isboolean(m_L, -1)) {
        throw ScriptError(varname + " should be a boolean.");
    }

    int retVal = lua_toboolean(m_L, -1);
    lua_pop(m_L, 1);

    return ((retVal) ? true : false);
}

//------------------------------------------------------------------------------

const char *ScriptMgr::getGlobalString(const std::string &varname)
{
    lua_getglobal(m_L, varname.c_str());
    if (!lua_isstring(m_L, -1)) {
        throw ScriptError(varname + " should be a string.");
    }

    const char *retVal = lua_tostring(m_L, -1);
    lua_pop(m_L, 1);

    return retVal;
}

//------------------------------------------------------------------------------

void ScriptMgr::addListener(ScriptListener* listener)
{
    m_listeners.push_back(listener);
}

//------------------------------------------------------------------------------

void ScriptMgr::removeListener(ScriptListener* listener)
{
    m_listeners.remove(listener);
}

//------------------------------------------------------------------------------

void ScriptMgr::notifyListenersOnOutput(const std::string & msg)
{
    for (auto listener : m_listeners) {
        listener->onScriptOutput(msg);
    }
}

//------------------------------------------------------------------------------

void ScriptMgr::notifyListenersOnError(const std::string & msg)
{
    for (auto listener : m_listeners) {
        listener->onScriptError(msg);
    }
}

//==============================================================================

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

static int l_print(lua_State *L)
{
    int nargs = lua_gettop(L);

    std::ostringstream os;

    for (int i = 1; i <= nargs; ++i) {
        if (lua_isstring(L, i)) {
            os << lua_tostring(L, i);
        } else if (lua_isnil(L, i)) {
            os << "nil";
        }

        if (i < nargs) {
            os << '\t';
        }
    }

    ScriptMgr::singleton().notifyListenersOnOutput(os.str());

    return 0;
}
