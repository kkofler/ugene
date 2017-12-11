/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2017 UniPro <ugene@unipro.ru>
 * http://ugene.net
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

#ifndef _U2_KRAKEN_BUILD_TASK_H_
#define _U2_KRAKEN_BUILD_TASK_H_

#include <U2Core/ExternalToolRunTask.h>

#include "KrakenBuildTaskSettings.h"

namespace U2 {

class KrakenBuildTask : public ExternalToolSupportTask {
    Q_OBJECT
public:
    KrakenBuildTask(const KrakenBuildTaskSettings &settings);

    const QString &getResultDatabaseUrl() const;
    const KrakenBuildTaskSettings &getSettings() const;

private:
    void prepare();

    void checkSettings();
    QStringList getDownloadTaxonomyArguments() const;
    QStringList getAddToLibraryArguments(const QString &additionalGenomeUrl) const;
    QStringList getBuildArguments() const;
    QStringList getCleanArguments() const;
    QStringList getShrinkArguments() const;

    const KrakenBuildTaskSettings settings;
};

}   // namespace U2

#endif // _U2_KRAKEN_BUILD_TASK_H_