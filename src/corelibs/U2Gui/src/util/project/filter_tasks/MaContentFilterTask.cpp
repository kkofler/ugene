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

#include <U2Core/DNAAlphabet.h>
#include <U2Core/L10n.h>
#include <U2Core/MSAUtils.h>
#include <U2Core/MultipleSequenceAlignmentObject.h>
#include <U2Core/MultipleChromatogramAlignmentObject.h>
#include <U2Core/DNASequenceObject.h>
#include <U2Core/U2OpStatusUtils.h>
#include <U2Core/U2SafePoints.h>

#include "MaContentFilterTask.h"
#include "../ProjectFilterNames.h"

namespace U2 {

//////////////////////////////////////////////////////////////////////////
/// MaContentFilterTask
//////////////////////////////////////////////////////////////////////////

MaContentFilterTask::MaContentFilterTask(const ProjectTreeControllerModeSettings &settings, const QList<QPointer<Document> > &docs)
    : AbstractProjectFilterTask(settings, ProjectFilterNames::MSA_CONTENT_FILTER_NAME, docs)
{
    filteredObjCountPerIteration = 1;
}

bool MaContentFilterTask::filterAcceptsObject(GObject *obj) {
    MultipleAlignmentObject *maObj = qobject_cast<MultipleAlignmentObject *>(obj);
    CHECK(NULL != maObj, false);

    MultipleChromatogramAlignmentObject* mcaObj = qobject_cast<MultipleChromatogramAlignmentObject*>(maObj);

    foreach(const QString &pattern, settings.tokensToShow) {
        if (!patternFitsMaAlphabet(maObj, pattern)) {
            continue;
        }
        if (maContainsPattern(maObj, pattern)) {
            return true;
        }

        // if this is MCA -> check reference sequence content too.
        if (mcaObj != NULL) {
            U2SequenceObject* refObj = mcaObj->getReferenceObj();
            if (refObj != NULL && seqContainsPattern(refObj, pattern)) {
                return true;
            }
        }
    }
    return false;
}

bool MaContentFilterTask::patternFitsMaAlphabet(const MultipleAlignmentObject *maObject, const QString &pattern) {
    SAFE_POINT(NULL != maObject, L10N::nullPointerError("MSA object"), false);
    SAFE_POINT(!pattern.isEmpty(), "Empty pattern to search", false);

    const DNAAlphabet *alphabet = maObject->getAlphabet();
    SAFE_POINT(NULL != alphabet, L10N::nullPointerError("MSA alphabet"), false);

    const QByteArray searchStr = pattern.toUpper().toLatin1();
    return alphabet->containsAll(searchStr.constData(), searchStr.length());
}

bool MaContentFilterTask::maContainsPattern(const MultipleAlignmentObject *maObject, const QString &pattern) {
    SAFE_POINT(NULL != maObject, L10N::nullPointerError("MSA object"), false);
    SAFE_POINT(!pattern.isEmpty(), "Empty pattern to search", false);

    const MultipleAlignmentData* mData = maObject->getMultipleAlignment().data();
    const QByteArray searchStr = pattern.toUpper().toLatin1();

    for (int i = 0, n = mData->getNumRows(); i < n; ++i) {
        const MultipleAlignmentRow& row = mData->getRow(i);
        for (int j = 0; j < (mData->getLength() - searchStr.length() + 1); ++j) {
            char c = row->charAt(j);
            int altenateLength = 0;
            if (U2Msa::GAP_CHAR != c && MSAUtils::equalsIgnoreGaps(row, j, searchStr, altenateLength)) {
                return true;
            }
        }
    }
    return false;
}

bool MaContentFilterTask::seqContainsPattern(const U2SequenceObject* seqObject, const QString &pattern) {
    SAFE_POINT(seqObject != NULL, L10N::nullPointerError("Sequence object"), false);
    SAFE_POINT(!pattern.isEmpty(), "Empty pattern to search", false);

    U2OpStatusImpl op;
    QByteArray seqData = seqObject->getWholeSequenceData(op);
    CHECK_OP(op, false);

    const QByteArray searchStr = pattern.toUpper().toLatin1();
    return seqData.indexOf(searchStr) >= 0;
}

//////////////////////////////////////////////////////////////////////////
/// MaContentFilterTaskFactory
//////////////////////////////////////////////////////////////////////////

AbstractProjectFilterTask * MaContentFilterTaskFactory::createNewTask(const ProjectTreeControllerModeSettings &settings,
    const QList<QPointer<Document> > &docs) const
{
    return new MaContentFilterTask(settings, docs);
}

} // namespace U2
