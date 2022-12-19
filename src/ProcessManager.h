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

    // potentially review passing in state with jacob but not really necessary for this project
    void in(unsigned long delay, bool (*cb)(void *))
    {
        // q: how to fix issue use of non-static member function
        Timer<10>::Task task = timer.in(delay, onComplete, (void *)cb);
        addProcess(cb, task);
    }
    void at(unsigned long delay, bool (*cb)(void *))
    {
        Timer<10>::Task task = timer.at(delay, onComplete, (void *)cb);
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
            // debuglnV("Process already exists");
            timer.cancel(task);
        }
        else
        {
            debuglnV("Adding process");
            processes[cb] = task;
        }
    }
    // TODO: this isn't properly removing
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
            // Serial.print(processes.size());
            timer.tick();
        }
    }
};
// keep this as a globa static var since its used in main.cpp
ProcessManager processManager;
bool onComplete(void *void_cb)
{
    bool (*cb)(void *) = (bool (*)(void *))void_cb;
    cb(nullptr);
    // if (!cb(nullptr))
    // {
    processManager.remove(cb);
    return false;
    // }
    // return true;
}
#endif