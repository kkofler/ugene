/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2012 UniPro <ugene@unipro.ru>
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

#include "LogUtils.h"
#include "QtUtils.h"

#include <U2Core/U2SafePoints.h>

namespace U2 {

LogTracer::LogTracer()
: wasError(false) {
    moveToThread(LogServer::getInstance()->thread());
    connect(LogServer::getInstance(), SIGNAL(si_message(const LogMessage&)), SLOT(sl_onMessage(const LogMessage&)));
}


LogTracer::~LogTracer() {
    disconnect(LogServer::getInstance(), SIGNAL(si_message(const LogMessage&)));
}

void LogTracer::sl_onMessage(const LogMessage &msg) {

    if (msg.level == LogLevel_ERROR) {
        wasError = true;
    }
}

void LogUtils::checkHasError(U2OpStatus &os, const LogTracer& l) {
    CHECK_SET_ERR(l.hasError() == false, "There is an error in log");
}

} // namespace
