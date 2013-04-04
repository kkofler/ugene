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

#include "MAlignmentObject.h"

#include <U2Core/DNASequence.h>
#include <U2Core/MAlignmentExporter.h>
#include <U2Core/MAlignmentImporter.h>
#include <U2Core/MsaDbiUtils.h>
#include <U2Core/MSAUtils.h>
#include <U2Core/U2AlphabetUtils.h>
#include <U2Core/U2MsaDbi.h>
#include <U2Core/U2Mod.h>
#include <U2Core/U2ObjectDbi.h>
#include <U2Core/U2OpStatusUtils.h>
#include <U2Core/U2SafePoints.h>

namespace U2 {

MSAMemento::MSAMemento():lastState(MAlignment()){}

MAlignment MSAMemento::getState() const{
    return lastState;
}

void MSAMemento::setState(const MAlignment& state){
    lastState = state;
}


MAlignmentObject::MAlignmentObject(const QString& name, const U2EntityRef& msaRef, const QVariantMap& hintsMap /* = QVariantMap */)
    : GObject(GObjectTypes::MULTIPLE_ALIGNMENT, name, hintsMap),
      cachedMAlignment(MAlignment()),
      memento(new MSAMemento)
{
    entityRef = msaRef;

    U2OpStatus2Log os;
    DbiConnection con(entityRef.dbiRef, os);
    CHECK_OP(os,);

    updateCachedMAlignment();
}

MAlignmentObject::~MAlignmentObject(){
    delete memento;
}

void MAlignmentObject::setTrackMod(U2TrackModType trackMod, U2OpStatus& os) {
    // Prepare the connection
    DbiConnection con(entityRef.dbiRef, os);
    CHECK_OP(os, );

    U2ObjectDbi* objDbi = con.dbi->getObjectDbi();
    SAFE_POINT(NULL != objDbi, "NULL Object Dbi!",);

    // Set the new status
    objDbi->setTrackModType(entityRef.entityId, trackMod, os);
}

MAlignment MAlignmentObject::getMAlignment() const {
    return cachedMAlignment;
}

void MAlignmentObject::updateCachedMAlignment(MAlignmentModInfo mi) {
    MAlignment maBefore = cachedMAlignment;
    QString oldName = maBefore.getName();

    U2OpStatus2Log os;
    MAlignmentExporter alExporter;
    cachedMAlignment = alExporter.getAlignment(entityRef.dbiRef, entityRef.entityId, os);
    SAFE_POINT_OP(os, );

    setModified(true);
    if (mi.middleState == false) {
        emit si_alignmentChanged(maBefore, mi);

        QString newName = cachedMAlignment.getName();
        if (newName != oldName) {
            GObject::setGObjectName(cachedMAlignment.getName());
        }
    }
}

void MAlignmentObject::setMAlignment(const MAlignment& newMa, MAlignmentModInfo mi, const QVariantMap& hints) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    U2OpStatus2Log os;
    MsaDbiUtils::updateMsa(entityRef, newMa, os);
    SAFE_POINT_OP(os, );

    mi.hints = hints;
    updateCachedMAlignment(mi);
}

void MAlignmentObject::copyGapModel(const QList<MAlignmentRow> &copyRows) {
    const QList<MAlignmentRow> &oldRows = getMAlignment().getRows();
    SAFE_POINT(oldRows.count() == copyRows.count(), "Different rows count", );

    QMap<qint64, QList<U2MsaGap> > newGapModel;
    QList<MAlignmentRow>::ConstIterator ori = oldRows.begin();
    QList<MAlignmentRow>::ConstIterator cri = copyRows.begin();
    for (; ori != oldRows.end(); ori++, cri++) {
        newGapModel[ori->getRowId()] = cri->getGapModel();
    }

    U2OpStatus2Log os;
    updateGapModel(newGapModel, os);
}

char MAlignmentObject::charAt(int seqNum, int pos) const {
    MAlignment msa = getMAlignment();
    return msa.charAt(seqNum, pos);
}

void MAlignmentObject::saveState(){
    MAlignment msa = getMAlignment();
    emit si_completeStateChanged(false);
    memento->setState(msa);
}

void MAlignmentObject::releaseState(){
    if(!isStateLocked()) {
        emit si_completeStateChanged(true);

        MAlignment maBefore = memento->getState();
        setModified(true);
        MAlignmentModInfo mi;

        emit si_alignmentChanged(maBefore, mi);
    }
}


GObject* MAlignmentObject::clone(const U2DbiRef& dbiRef, U2OpStatus& os) const {
    MAlignment msa = getMAlignment();
    U2EntityRef clonedMsaRef = MAlignmentImporter::createAlignment(dbiRef, msa, os);
    CHECK_OP(os, NULL);

    MAlignmentObject* clonedObj = new MAlignmentObject(msa.getName(), clonedMsaRef, getGHintsMap());
    clonedObj->setIndexInfo(getIndexInfo());
    return clonedObj;
}

void MAlignmentObject::insertGap(U2Region rows, int pos, int count) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );
    MAlignment msa = getMAlignment();
    int startSeq = rows.startPos;
    int endSeq = startSeq + rows.length;

    U2OpStatus2Log os;
    QList<qint64> rowIdsToInsert;
    QList<qint64> rowIdsToShift;
    for (int i = 0; i < startSeq; ++i) {
        qint64 rowId = msa.getRow(i).getRowId();
        rowIdsToShift.append(rowId);
    }
    for (int i = startSeq; i < endSeq; ++i) {
        qint64 rowId = msa.getRow(i).getRowId();
        rowIdsToInsert.append(rowId);
    }
    for (int i = endSeq; i < msa.getNumRows(); ++i) {
        qint64 rowId = msa.getRow(i).getRowId();
        rowIdsToShift.append(rowId);
    }

    MsaDbiUtils::insertGaps(entityRef, rowIdsToInsert, pos, count, os);
    SAFE_POINT_OP(os, );

    MAlignmentModInfo mi;
    mi.sequenceListChanged = false;
    updateCachedMAlignment(mi);
}

int MAlignmentObject::deleteGap(const U2Region &rows, int pos, int maxGaps) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", 0);

    MAlignment msa = getMAlignment();

    int n = 0, max = qBound(0, maxGaps, msa.getLength() - pos);
    int rowCount = rows.startPos;
    while (rowCount < rows.endPos()) {
        while (n < max) {
            char c = msa.charAt(rowCount, pos + n);
            if (c != MAlignment_GapChar) { //not a gap
                break;
            }
            n++;
        }
        if (n == 0 && (rows.endPos() - 1 == rowCount)) {
            return 0;
        }
        U2OpStatus2Log os;
        msa.removeChars(rowCount, pos, n, os);
        const MAlignmentRow &row = msa.getRow(rowCount);
        MsaDbiUtils::updateRowGapModel(entityRef, row.getRowId(), row.getGapModel(), os);
        SAFE_POINT_OP(os, 0);
        ++rowCount;
    }
    updateCachedMAlignment();
    return n;
}

int MAlignmentObject::deleteGap(int pos, int maxGaps) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", 0);

    MAlignment msa = getMAlignment();
    //find min gaps in all sequences starting from pos
    int minGaps = maxGaps;
    int max = qBound(0, maxGaps, msa.getLength() - pos);
    foreach(const MAlignmentRow& row, msa.getRows()) {
        int nGaps = 0;
        for (int i = pos; i < pos + max; i++, nGaps++) {
            if (row.charAt(i) != MAlignment_GapChar) { 
                break;
            }
        }
        minGaps = qMin(minGaps, nGaps);
        if (minGaps == 0) {
            break;
        }
    }
    if (minGaps == 0) {
        return  0;
    }
    int nDeleted = minGaps;
    U2OpStatus2Log os;
    for (int i = 0, n = msa.getNumRows(); i < n; i++) {
        msa.removeChars(i, pos, nDeleted, os);
        SAFE_POINT_OP(os, 0);

        const MAlignmentRow &row = msa.getRow(i);
        MsaDbiUtils::updateRowGapModel(entityRef, row.getRowId(), row.getGapModel(), os);
        SAFE_POINT_OP(os, 0);
    }

    updateCachedMAlignment();
    return nDeleted;
}

void MAlignmentObject::addRow(U2MsaRow& rowInDb, const DNASequence& seq, int rowIdx) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    MAlignment msa = getMAlignment();

    DNAAlphabet* newAlphabet = U2AlphabetUtils::deriveCommonAlphabet(seq.alphabet, getAlphabet());
    assert(newAlphabet != NULL);
    msa.setAlphabet(newAlphabet);

    U2OpStatus2Log os;
    MsaDbiUtils::addRow(entityRef, rowIdx, rowInDb, os);
    SAFE_POINT_OP(os, );

    updateCachedMAlignment();
}

void MAlignmentObject::removeRow(int rowIdx) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    SAFE_POINT(rowIdx >= 0 && rowIdx < cachedMAlignment.getNumRows(), "Invalid row index!", );
    const MAlignmentRow& row = cachedMAlignment.getRow(rowIdx);
    qint64 rowId = row.getRowDBInfo().rowId;

    U2OpStatus2Log os;
    MsaDbiUtils::removeRow(entityRef, rowId, os);
    SAFE_POINT_OP(os, );

    MAlignmentModInfo mi;
    mi.sequenceContentChanged = false;
    updateCachedMAlignment(mi);
}

void MAlignmentObject::updateRow(int rowIdx, const QString& name, const QByteArray& seqBytes, const QList<U2MsaGap>& gapModel, U2OpStatus& os) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    SAFE_POINT(rowIdx >= 0 && rowIdx < cachedMAlignment.getNumRows(), "Invalid row index!", );
    const MAlignmentRow& row = cachedMAlignment.getRow(rowIdx);
    qint64 rowId = row.getRowDBInfo().rowId;

    U2UseCommonUserModStep modStep(entityRef, os);
    SAFE_POINT_OP(os, );

    MsaDbiUtils::updateRowContent(entityRef, rowId, seqBytes, gapModel, os);
    CHECK_OP(os, );

    MsaDbiUtils::renameRow(entityRef, rowId, name, os);
    CHECK_OP(os, );

    updateCachedMAlignment();
}

void MAlignmentObject::setGObjectName(const QString& newName) {
    U2OpStatus2Log os;
    MsaDbiUtils::renameMsa(entityRef, newName, os);
    SAFE_POINT_OP(os, );

    updateCachedMAlignment();

    GObject::setGObjectName(newName);
}

void MAlignmentObject::removeRegion(int startPos, int startRow, int nBases, int nRows, bool removeEmptyRows, bool track) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );
    QList<qint64> rowIds;
    MAlignment msa = getMAlignment();
    SAFE_POINT(nRows > 0 && startRow >= 0 && startRow + nRows - 1 < msa.getNumRows(), "Invalid parameters!", );
    QList<MAlignmentRow>::ConstIterator it = msa.getRows().begin() + startRow;
    QList<MAlignmentRow>::ConstIterator end = it + nRows;
    for (; it != end; it++) {
        rowIds << it->getRowId();
    }

    U2OpStatus2Log os;
    MsaDbiUtils::removeRegion(entityRef, rowIds, startPos, nBases, os);
    SAFE_POINT_OP(os, );

    MsaDbiUtils::trim(entityRef, os);
    SAFE_POINT_OP(os, );

    if (removeEmptyRows) {
        MsaDbiUtils::removeEmptyRows(entityRef, rowIds, os);
    }
    if (track) {
        updateCachedMAlignment();
    }
}

void MAlignmentObject::renameRow(int rowIdx, const QString& newName) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    SAFE_POINT(rowIdx >= 0 && rowIdx < cachedMAlignment.getNumRows(), "Invalid row index!", );
    const MAlignmentRow& row = cachedMAlignment.getRow(rowIdx);
    qint64 rowId = row.getRowDBInfo().rowId;

    U2OpStatus2Log os;
    MsaDbiUtils::renameRow(entityRef, rowId, newName, os);
    SAFE_POINT_OP(os, );

    updateCachedMAlignment();
}

void MAlignmentObject::crop(U2Region window, const QSet<QString>& rowNames) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );
    MAlignment msa = getMAlignment();

    QList<qint64> rowIds;
    for (int i = 0; i < msa.getNumRows(); ++i) {
        QString rowName = msa.getRow(i).getName();
        if (rowNames.contains(rowName)) {
            qint64 rowId = msa.getRow(i).getRowId();
            rowIds.append(rowId);
        }
    }

    U2OpStatus2Log os;
    MsaDbiUtils::crop(entityRef, rowIds, window.startPos, window.length, os);
    SAFE_POINT_OP(os, );

    updateCachedMAlignment();
}

void MAlignmentObject::deleteColumnWithGaps(int requiredGapCount) {
    MAlignment msa = getMAlignment();
    const int length = msa.getLength();
    if (GAP_COLUMN_ONLY == requiredGapCount) {
        requiredGapCount = msa.getNumRows();
    }
    QList<qint64> colsForDelete;
    for(int i = 0; i < length; i++) { //columns
        int gapCount = 0;
        for(int j = 0; j < msa.getNumRows(); j++) { //sequences
            if(charAt(j,i) == '-') {
                gapCount++;
            }
        }

        if(gapCount >= requiredGapCount) {
            colsForDelete.prepend(i); //invert order
        }
    }
    if (length == colsForDelete.count()) {
        return;
    }
    QList<qint64>::const_iterator column = colsForDelete.constBegin();
    const QList<qint64>::const_iterator end = colsForDelete.constEnd();
    for ( ; column != end; ++column) {
        if (*column >= cachedMAlignment.getLength()) {
            continue;
        }
        removeRegion(*column, 0, 1, msa.getNumRows(), true, (end - 1 == column));
    }
    //removeColumns(colsForDelete, true);
    msa = getMAlignment();
    updateCachedMAlignment();
}

void MAlignmentObject::updateGapModel(QMap<qint64, QList<U2MsaGap> > rowsGapModel, U2OpStatus& os) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    MAlignment msa = getMAlignment();

    foreach (qint64 rowId, rowsGapModel.keys()) {
        if (!msa.getRowsIds().contains(rowId)) {
            os.setError("Can't update gaps of a multiple alignment!");
            return;
        }

        MsaDbiUtils::updateRowGapModel(entityRef, rowId, rowsGapModel.value(rowId), os);
        CHECK_OP(os, );
    }

    updateCachedMAlignment();
}

void MAlignmentObject::moveRowsBlock(int firstRow, int numRows, int shift)
{
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    QList<qint64> rowIds = cachedMAlignment.getRowsIds();
    QList<qint64> rowsToMove;

    for (int i = 0; i < numRows; ++i) {
        rowsToMove << rowIds[firstRow + i];
    }

    U2OpStatusImpl os;
    MsaDbiUtils::moveRows(entityRef, rowsToMove, shift, os);
    CHECK_OP(os, );

    updateCachedMAlignment();
}

void MAlignmentObject::updateRowsOrder(const QList<qint64>& rowIds, U2OpStatus& os) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    MsaDbiUtils::updateRowsOrder(entityRef, rowIds, os);
    CHECK_OP(os, );

    updateCachedMAlignment();
}

bool MAlignmentObject::shiftRegion( int startPos, int startRow, int nBases, int nRows, int shift )
{
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", false );
    SAFE_POINT(!isRegionEmpty(startPos, startRow, nBases, nRows), "Region is empty!", false );

    int n = 0;
    if (shift > 0) {
        insertGap(U2Region(startRow,nRows), startPos, shift);
        n = shift;
    } else {
        if (startPos + shift < 0) {
            return false;
        }
        n = deleteGap(U2Region(startRow, nRows), startPos + shift, -shift);
    }    

    return n > 0;
}


bool MAlignmentObject::isRegionEmpty(int startPos, int startRow, int numChars, int numRows) const
{
    MAlignment msa = getMAlignment();
    bool emptyBlock = true;
    for (int row = startRow; row < startRow + numRows; ++row ) {
        for( int pos = startPos; pos < startPos + numChars; ++pos ) {
            const MAlignmentRow& r = msa.getRows().at(row);
            if (r.charAt(pos) != MAlignment_GapChar) {
                emptyBlock = false;
                break;
            }
        }
    }

    return emptyBlock;
}

DNAAlphabet* MAlignmentObject::getAlphabet() const {
    return cachedMAlignment.getAlphabet();
}

qint64 MAlignmentObject::getLength() const {
    return cachedMAlignment.getLength();
}

qint64 MAlignmentObject::getNumRows() const {
    return cachedMAlignment.getNumRows();
}

const MAlignmentRow& MAlignmentObject::getRow(int row) const {
    MAlignment msa = getMAlignment();
    return msa.getRow(row);
}

static bool _registerMeta() {
    qRegisterMetaType<MAlignmentModInfo>("MAlignmentModInfo");
    return true;
}

bool MAlignmentModInfo::registerMeta = _registerMeta();

void MAlignmentObject::sortRowsByList(const QStringList& order) {
    GTIMER(c, t, "MAlignmentObject::sortRowsByList");
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    MAlignment msa = getMAlignment();
    msa.sortRowsByList(order);

    U2OpStatusImpl os;
    MsaDbiUtils::updateRowsOrder(entityRef, msa.getRowsIds(), os);
    SAFE_POINT_OP(os, );

    updateCachedMAlignment();
}

}//namespace
