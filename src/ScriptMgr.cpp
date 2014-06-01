/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
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
    L = luaL_newstate();

    // Expose functions to lua code
    lua_register(L, "addBox", l_addBox);
    lua_register(L, "print", l_print);

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
    dataFolder = appendDirSeparator(folder);
}

//------------------------------------------------------------------------------

bool ScriptMgr::executeString(const std::string& code)
{
    if (luaL_dostring(L, code.c_str())) {
        const char *errMsg = lua_tostring(L, -1);
        notifyListeners(ERR, errMsg);
        throw ScriptError(errMsg);
    }

    LOG << "Executed code: " << code << std::endl;
    return true;
}

//------------------------------------------------------------------------------

bool ScriptMgr::executeFile(const std::string& filename)
{
    std::string fullpath = dataFolder + filename;
    if (luaL_dofile(L, fullpath.c_str())) {
        const char *errMsg = lua_tostring(L, -1);
        notifyListeners(ERR, errMsg);
        throw ScriptError(errMsg);
    }

    LOG << "Executed script from: " << fullpath << std::endl;
    return true;
}

//------------------------------------------------------------------------------

int ScriptMgr::getGlobalInt(const std::string& varname)
{
    lua_getglobal(L, varname.c_str());
    if (!lua_isnumber(L, -1)) {
        throw ScriptError(varname + " should be a number.");
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
        throw ScriptError(varname + " should be a boolean.");
    }

    int retVal = lua_toboolean(L, -1);
    lua_pop(L, 1);

    return ((retVal) ? true : false);
}

//------------------------------------------------------------------------------

const char *ScriptMgr::getGlobalString(const std::string &varname)
{
    lua_getglobal(L, varname.c_str());
    if (!lua_isstring(L, -1)) {
        throw ScriptError(varname + " should be a string.");
    }

    const char *retVal = lua_tostring(L, -1);
    lua_pop(L, 1);

    return retVal;
}

//------------------------------------------------------------------------------

void ScriptMgr::addListener(OutputType ot, std::shared_ptr<ScriptListener> listener)
{
    listenersForOutput(ot).push_back(listener);
}

//------------------------------------------------------------------------------

void ScriptMgr::removeListener(OutputType ot, std::shared_ptr<ScriptListener> listener)
{
    listenersForOutput(ot).remove(listener);
}

//------------------------------------------------------------------------------

void ScriptMgr::notifyListeners(OutputType ot, const std::string msg)
{
    for (auto listenerPtr : listenersForOutput(ot)) {
        (*listenerPtr)(msg);
    }
}

//------------------------------------------------------------------------------

ScriptMgr::ListenersList& ScriptMgr::listenersForOutput(OutputType ot)
{
    switch (ot) {
    case OUT: return m_outListeners;
    case ERR: return m_errListeners;
    default:
        return m_outListeners; // Silence warning
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

    ScriptMgr::singleton().notifyListeners(ScriptMgr::OUT, os.str());

    return 0;
}
