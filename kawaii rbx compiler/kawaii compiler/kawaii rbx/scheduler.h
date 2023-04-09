#pragma once
#include "Luau/include/lualib.h"
#include <cstdint>
#include <vector>
#include <string>
#include "addresses.h"
#include <iostream>

//task scheduler that checks for jobs in WaitingHybridScriptsJobs
//made by nezyghoul but i rewrote it so W

int get_job_by_name(const std::string& job_name)
{
    int job_return = 0;//default null

    uintptr_t scheduler_address = rbx_getscheduler();//pass to getscheduler
    //for some reason these offsets never change so if they crash then yk why
    uintptr_t job_start = *(uintptr_t*)(scheduler_address + 308);
    uintptr_t job_end = *(uintptr_t*)(scheduler_address + 312);

    while (job_start != job_end)
    {
        uintptr_t job_address = *(uintptr_t*)(job_start);
        std::string* job_string = reinterpret_cast<std::string*>(job_address + 16);

        if (*job_string == job_name)//check if the job matches
        {
            job_return = job_address;//return it with the address
            break;
        }

        job_start += 8;
    }

    return job_return;//get the amount of jobs
}


int get_script_context()
{
    uintptr_t job_address = get_job_by_name("WaitingHybridScriptsJob");//search for this string for jobs
    uintptr_t context_address = *(uintptr_t*)(job_address + 304);//offset never changes just like your habit of skidding :kek:
    return context_address;
}

int get_robloxstate()//get roblox state aka -> switch to lua state
{
    uintptr_t script_context_address = get_script_context();
    uintptr_t roblox_state_address = luastate(script_context_address);//pass down to lua state
    return roblox_state_address;//return
}