#pragma once
#ifndef _PROCESS_MANAGER_H
#define _PROCESS_MANAGER_H
#include <map>
#include "debugger.h"
#include <arduino-timer.h>

// reminder: review function signatures on next lesson / casting
bool onComplete(void *void_cb);
struct ProcessManager
{
    Timer<10> timer;
    std::map<bool (*)(void *), Timer<10>::Task> processes;

    enum TaskState
    {
        PAUSED,
        RUNNING,
    };

    TaskState currentState = PAUSED;

    void in(unsigned long delay, bool (*cb)(void *))
    {
        // q: how to fix issue use of non-static member function
        Timer<10>::Task task = timer.in(delay, cb);
        addProcess(cb, task);
    }
    void at(unsigned long delay, bool (*cb)(void *))
    {
        Timer<10>::Task task = timer.at(delay, cb);
        addProcess(cb, task);
    }
    void every(unsigned long delay, bool (*cb)(void *))
    {
        Timer<10>::Task task = timer.every(delay, onComplete, (void *)cb);
        addProcess(cb, task);
    }
    void addProcess(bool (*cb)(void *), Timer<10>::Task task)
    {

        if (processes.find(cb) != processes.end())
        {
            debuglnV("Process already exists");
            timer.cancel(task);
        }
        else
        {
            processes[cb] = task;
        }
    }
    void remove(bool (*cb)(void *))
    {
        if (processes.find(cb) != processes.end())
        {
            timer.cancel(processes[cb]);
            processes.erase(cb);
        }
    }

    void start()
    {
        currentState = RUNNING;
    }
    void pause()
    {
        currentState = PAUSED;
    }
    void update()
    {
        if (currentState == RUNNING)
        {
            timer.tick();
        }
    }
};

ProcessManager processManager;
bool onComplete(void *void_cb)
{
    bool (*cb)(void *) = (bool (*)(void *))void_cb;
    if (!cb(nullptr))
    {
        processManager.processes.erase(cb);
        return false;
    }
    return true;
}
#endif