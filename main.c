#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

typedef struct {
    char name[50];
    int argsSize;
    double args[10];
} function;

typedef struct {
    char name[50];
    int argsSize;
    int outputSize;
} implementation;

void error (lua_State *L, const char *fmt, ...);

const implementation implementationList[] = {
        {"soma", 2, 1},
        {"subtracao", 2, 1},
        {"multiplicacao", 2, 1},
        {"divisao", 2, 1},
        {"potenciacao", 2, 1},
        {"fatorial", 1, 1},
        {"bhaskara", 3, 2}
};

int argsIsValid(int argc) {
    return argc >= 2;
}

int functionHasImplementation(function func) {
    int len = sizeof(implementationList) / sizeof(implementationList[0]);

    for (int i = 0; i < len; ++i){
        implementation impl = implementationList[i];

        if (!strcmp(impl.name, func.name)) {
            return 1;
        }
    }

    return 0;
}

function createFunction(char* argv[]) {
    function func;

    strcpy(func.name, argv[1]);
    func.argsSize = atoi(argv[2]);

     for (int i = 0; i < func.argsSize; i++) {
        func.args[i] = atof(argv[i + 3]);
    }

    return func;
}

int functionIsValid(function func) {
    int len = sizeof(implementationList) / sizeof(implementationList[0]);

    for (int i = 0; i < len; ++i){
        implementation impl = implementationList[i];
        if (!strcmp(impl.name, func.name)) {
            return impl.argsSize == func.argsSize;
        }
    }

    return 0;
}

int getOutputSize(function func) {
    int len = sizeof(implementationList) / sizeof(implementationList[0]);

    for (int i = 0; i < len; ++i){
        implementation impl = implementationList[i];
        if (!strcmp(impl.name, func.name)) {
            return impl.outputSize;
        }
    }

    return -1;
}

lua_State* openLuaFile(char* file) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, file);

    return L;
}

lua_State* executeLuaFunction(lua_State* L, function func) {
    lua_getglobal(L, func.name);

    int i;
    for (i = 0; i < func.argsSize; i++) {
        lua_pushnumber(L, func.args[i]);
    }

    int outputSize = getOutputSize(func);

    if (lua_pcall(L, func.argsSize, getOutputSize(func), 0)) {
        error(L, "error running function %s: %s",
              func.name,
              lua_tostring(L, -1));
    }

    return L;
}

void printResult(lua_State* L, function func) {
    int outputSize = getOutputSize(func);

    printf("Resultado da %s é", func.name);

    for (int i = 0; i < outputSize; i++) {
        printf(" %f", lua_tonumber(L, -1));
        lua_pop(L, 1);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (!argsIsValid(argc)) {
        printf("Numero de argumentos invalidos, para executar o programa forneca o nome da funcao\n");
        return 1;
    }

    function func = createFunction(argv);

    if (!functionHasImplementation(func)) {
        printf("Funcao nao implementada\n");
        return 1;
    }

    if (!functionIsValid(func)) {
        printf("Funcao invalida, verifique os argumentos da funcao\n");
        return 1;
    }

    lua_State* L = openLuaFile("functions.lua");
    L = executeLuaFunction(L, func);
    printResult(L, func);

    lua_close(L);
    return 0;
}

void error (lua_State *L, const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    exit(EXIT_FAILURE);
}