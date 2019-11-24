#pragma once

#include "CThread.h"
#include "CMutex.h"
#include <wut_types.h>

class BackgroundThreadWrapper: public CThread {
public:
    BackgroundThreadWrapper(int32_t priority);
    virtual ~BackgroundThreadWrapper();
protected:
    BOOL shouldExit() {
        return (exitThread == 1);
    }

    void setThreadPriority(int32_t priority) {
       this->setThreadPriority(priority);
    }
    CMutex mutex;
private:
    void executeThread();

    /**
        Called when a connection has be accepted.
    **/
    virtual BOOL whileLoop() = 0;

    volatile int32_t exitThread = 0;


};
