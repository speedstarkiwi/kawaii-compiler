#pragma once
#include <cstdint>
#include <Windows.h>
#include <string>

uintptr_t aslr(uintptr_t address) {//aslr the address so you dont need to do it manually
    uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));
    return address - 0x400000 + baseAddress;
}

uintptr_t rl;//-> passed down so we can retrieve lua state
lua_State* l;//-> lua state for other purposes you will see in execution.h

int top = 12, base = 24;
int id1 = 72, id2 = 24;

//dont need to update btw**
uintptr_t luastate(uintptr_t sc)//please note: if you cant update ig use getstate but case default:
{
    return (sc + 244)  ^ *(DWORD*)(sc + 244);
}

uintptr_t getscheduler_address = 0xB320B0;//updated for 4/19
uintptr_t vmload_address = 0x756B40;//updated for 4/19
uintptr_t taskdefer_address = 0x7DC560;//updated for 4/19
uintptr_t print_address = 0x10EE9C0;//updated for 4/19
uintptr_t nilobject_address = aslr(0x000000);//not updated
uintptr_t performinit_address = 0xB49A20;//not updated
uintptr_t getstate_address = 0x79DAC0;//updated for 4/19

using getscheduler_rbx = std::uintptr_t(__cdecl*)();
getscheduler_rbx rbx_getscheduler = reinterpret_cast<getscheduler_rbx>(aslr(getscheduler_address));

using getstate_rbx = std::uintptr_t(__thiscall*)(std::uintptr_t rl, int* nigger);
getstate_rbx rbx_getstate = reinterpret_cast<getstate_rbx>(aslr(getstate_address));

using luavmload_rbx = std::uintptr_t(__fastcall*)(std::uintptr_t rl, std::string* source, const char* chunk, int env);
luavmload_rbx rbx_luavmload = reinterpret_cast<luavmload_rbx>(aslr(vmload_address));

using taskdefer_rbx = std::uintptr_t(__cdecl*)(std::uintptr_t rl);
taskdefer_rbx rbx_taskdefer = reinterpret_cast<taskdefer_rbx>(aslr(taskdefer_address));

using print_rbx = uintptr_t(__cdecl*)(int rl, const char* source);
print_rbx rbx_print = reinterpret_cast<print_rbx>(aslr(print_address));

using perinit_rbx = uintptr_t(__thiscall*)(const char* nigggerrr);
perinit_rbx rbx_performinit = reinterpret_cast<perinit_rbx>(aslr(performinit_address));


std::uintptr_t api_incr_top(std::uintptr_t a1) {//increments the lua
    std::uintptr_t* ptr = reinterpret_cast<std::uintptr_t*>(a1 + top);
    *ptr += sizeof(std::uintptr_t);
    return *ptr;
}

