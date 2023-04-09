#pragma once
#include <cstdint>
#include <Windows.h>
#include <string>

//update EVERYTHING

uintptr_t aslr(uintptr_t address) {//aslr the address so you dont need to do it manually
    uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));
    return address - 0x400000 + baseAddress;
}

uintptr_t rl;//-> passed down so we can retrieve lua state
lua_State* l;//-> lua state for other purposes you will see in execution.h

int top = 24, base = 20;
int id1 = 72, id2 = 24;

uintptr_t luastate(uintptr_t sc)//get the lua state from script context
{
    uintptr_t offset = 244;
    uintptr_t address = sc + offset;
    DWORD* dwordPtr = (DWORD*)address;
    uintptr_t result = *dwordPtr - address;
    return result;
}

uintptr_t getscheduler_address = 0xB2D460;
uintptr_t vmload_address = 0x7561B0;
uintptr_t taskdefer_address = 0x7D0ED0;
uintptr_t print_address = 0x10E4910;
uintptr_t nilobject_address = aslr(0x2950220);

using getscheduler_rbx = std::uintptr_t(__cdecl*)();
getscheduler_rbx rbx_getscheduler = reinterpret_cast<getscheduler_rbx>(aslr(getscheduler_address));

using luavmload_rbx = std::uintptr_t(__fastcall*)(std::uintptr_t rl, std::string* source, const char* chunk, int env);
luavmload_rbx rbx_luavmload = reinterpret_cast<luavmload_rbx>(aslr(vmload_address));

using taskdefer_rbx = std::uintptr_t(__cdecl*)(std::uintptr_t rl);
taskdefer_rbx rbx_taskdefer = reinterpret_cast<taskdefer_rbx>(aslr(taskdefer_address));

using print_rbx = uintptr_t(__cdecl*)(int rl, const char* source);
print_rbx rbx_print = reinterpret_cast<print_rbx>(aslr(print_address));

const char* rbx_typename(int rl, int type)//rewritten typename with types
{
    const char* result = "no value";
    if (type != LUA_TNONE)
        result = (const char*)*((DWORD*)&nilobject_address + type);//whatever type user gives
    return result;//bool string should pop up like taht or smth 
}

int rbx_getstate(int script_context, int* state_type) {//rewritten getstate function also alternative of directly using the lua state
    int context;

    switch (*state_type) {//should always call state_type = none of the cases below
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
        context = *(int*)(script_context + 396) - (script_context + 396);
        break;
    default://this is what the scheduler does.. use it default no cases.
        context = *(int*)(script_context + 244) - (script_context + 244);
        break;
    }

    return context;
}


void* rbx_pseudo2addr(DWORD* a1, int a2)//rewritten pseudo2addr for without casing index2addr
{
    //for skids that dont know what this is let me explain
    //if index2addrs index doesnt match the index cases, its getting passed onto pseudo2 with the index
    int v2; void* result; int v4; DWORD* v5; char* v6; int v7; DWORD* v8; char* v9;//keep in 1 line

    switch (a2)//switch the index type
    {
    case LUA_GLOBALSINDEX:
        v8 = a1 + 2;
        v9 = (char*)v8 - *v8;
        *((DWORD*)v9 + 384) = a1[14];
        *((DWORD*)v9 + 387) = 8;
        result = (char*)v8 - *v8 + 1536;
        break;

    case LUA_ENVIRONINDEX:
        v4 = a1[4];
        v5 = a1 + 2;
        v6 = (char*)v5 - *v5;
        if (v4 == a1[9])
            v7 = a1[14];
        else
            v7 = *(DWORD*)(**(DWORD**)(v4 + 12) + 12);
        *((DWORD*)v6 + 384) = v7;
        *((DWORD*)v6 + 387) = 8;
        result = (char*)v5 - *v5 + 1536;
        break;

    case LUA_REGISTRYINDEX:
        result = (char*)a1 - a1[2] + 1560;
        break;

    default://none of them match up
        v2 = **(DWORD**)(a1[4] + 12);
        if (LUA_GLOBALSINDEX - a2 > *(unsigned char*)(v2 + 4))
            result = &nilobject_address;//MAY or MAY not work i still have to test this
        else
            result = (void*)(v2 + 16 * (LUA_GLOBALSINDEX - a2 + 1));
        break;
    }
    return result;//if your retarded and cant get the func then scan this SKID 8D 41 08 8B 49 38
}


