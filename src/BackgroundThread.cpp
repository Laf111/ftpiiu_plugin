#include "BackgroundThread.hpp"
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <coreinit/cache.h>

#include <utils/logger.h>

#include "ftp.h"
#include "net.h"

BackgroundThread * BackgroundThread::instance = NULL;

BackgroundThread::BackgroundThread(): BackgroundThreadWrapper(BackgroundThread::getPriority()) {
    DEBUG_FUNCTION_LINE("Create new Server\n");
    mutex.lock();
        this->serverSocket = create_server(PORT);
        DCFlushRange(&(this->serverSocket), 4);
    mutex.unlock();
    DEBUG_FUNCTION_LINE("handle %d\n", this->serverSocket);
    resumeThread();
}

BackgroundThread::~BackgroundThread() {
    DEBUG_FUNCTION_LINE("Clean up FTP\n");
    if(this->serverSocket != -1){
        mutex.lock();
            cleanup_ftp();
            network_close(this->serverSocket);
        mutex.unlock();
        this->serverSocket = -1;
    }
    DEBUG_FUNCTION_LINE("Cleaned up FTP\n");
}

BOOL BackgroundThread::whileLoop() {
    if(this->serverSocket != -1){
        mutex.lock();
            network_down = process_ftp_events(this->serverSocket);
        mutex.unlock();
        if(network_down) {
            DEBUG_FUNCTION_LINE("Network is down %d\n", this->serverSocket);
            mutex.lock();
                cleanup_ftp();
                network_close(this->serverSocket);
                this->serverSocket = -1;
                DCFlushRange(&(this->serverSocket), 4);
            mutex.unlock();
        }
        OSSleepTicks(OSMillisecondsToTicks(16));
    }
    return true;
}
