/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2013 UniPro <ugene@unipro.ru>
 * http://ugene.unipro.ru
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef _CRASH_HANDLER_H_
#define _CRASH_HANDLER_H_


#include "StackWalker.h"

#include <U2Core/global.h>
#include <U2Core/LogCache.h>
#include <U2Core/AppResources.h>
#include <U2Core/U2SqlHelpers.h>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QMutex>


#if defined( Q_OS_WIN )

#include <windows.h>
    //LONG NTAPI CrashHandlerFunc(PEXCEPTION_POINTERS pExceptionInfo );
#else 
#include <stdlib.h>

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <execinfo.h>
#endif

namespace U2 {

struct ExceptionInfo {
    QString errorType;
};

#if defined( Q_OS_WIN )
typedef PVOID (__stdcall* addExceptionHandler)( __in ULONG, __in PVECTORED_EXCEPTION_HANDLER);
typedef PVOID (__stdcall* removeExceptionHandler)(__in PVOID);
#endif

class LogCache;
class Task;
class LogMessage;

class U2PRIVATE_EXPORT CrashHandler {
public:
    static void setupHandler();
    static bool isEnabled();
#if defined( Q_OS_WIN )
    static LONG NTAPI CrashHandlerFunc(PEXCEPTION_POINTERS pExceptionInfo );
    static LONG NTAPI CrashHandlerFuncSecond(PEXCEPTION_POINTERS pExceptionInfo );
    static LONG NTAPI CrashHandlerFuncThird(PEXCEPTION_POINTERS pExceptionInfo );

    static PVOID handler;
    static PVOID handler2;

    static addExceptionHandler addHandlerFunc;
    static removeExceptionHandler removeHandlerFunc;
    static StackWalker st;
#else
    static void signalHandler(int signo, siginfo_t *info, void *context);
    static struct sigaction sa;
#endif
    static void runMonitorProcess(const QString &exceptionType);
    static void getSubTasks(Task *t, QString& list, int lvl);
    static void preallocateReservedSpace();
    static void releaseReserve();

    static char*            buffer;
    static LogCache*        crashLogCache;
};

class CrashLogCache : public LogCache {
    Q_OBJECT
public:
    virtual void onMessage(const LogMessage& msg) {
        static int count=0;
        if (!(count++ % logMemoryInfoEvery)) {
            cmdLog.trace(formMemInfo());
        }

        LogCache::onMessage(msg);
    }

private:
    static const int logMemoryInfoEvery = 20;

    QString formMemInfo() {
        AppResourcePool* pool = AppResourcePool::instance();
        CHECK(pool, QString());

        size_t memoryBytes = pool->getCurrentAppMemory();
        QString memInfo = QString("AppMemory: %1Mb").arg(memoryBytes/(1000*1000));
        AppResource *mem = pool->getResource(RESOURCE_MEMORY);
        if (mem) {
            memInfo += QString("; Locked memory AppResource: %1/%2").arg(mem->maxUse() - mem->available()).arg(mem->maxUse());
        }

        int currentMemory=0, maxMemory=0;
        if (SQLiteUtils::getMemoryHint(currentMemory, maxMemory, 0)) {
            memInfo += QString("; SQLite memory %1Mb, max %2Mb").arg(currentMemory/(1000*1000)).arg(maxMemory/(1000*1000));
        }

        return memInfo;
    }
};


} //namespace

#endif
