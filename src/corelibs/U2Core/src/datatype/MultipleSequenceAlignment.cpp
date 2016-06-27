/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2016 UniPro <ugene@unipro.ru>
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

#include <QtCore/QStack>

#include "MultipleSequenceAlignment.h"

#include <U2Core/DNAAlphabet.h>
#include <U2Core/DNASequenceUtils.h>
#include <U2Core/Log.h>
#include <U2Core/MsaDbiUtils.h>
#include <U2Core/TextUtils.h>
#include <U2Core/U2OpStatusUtils.h>


namespace U2 {

//////////////////////////////////////////////////////////////////////////
// MultipleSequenceAlignmentRow
MultipleSequenceAlignmentRow::MultipleSequenceAlignmentRow(const U2MsaRow& _rowInDb,
                             const DNASequence& _sequence,
                             const QList<U2MsaGap>& _gaps,
                             MultipleSequenceAlignment* _alignment)
    : alignment(_alignment),
      sequence(_sequence),
      gaps(_gaps),
      initialRowInDb(_rowInDb)
{
    SAFE_POINT(alignment != NULL, "Parent MultipleSequenceAlignment is NULL", );
    removeTrailingGaps();
}

MultipleSequenceAlignmentRow::MultipleSequenceAlignmentRow(MultipleSequenceAlignment* al)
    : alignment(al),
      sequence(DNASequence()),
      initialRowInDb(U2MsaRow())
{
    initialRowInDb.rowId = invalidRowId();
    removeTrailingGaps();
}

MultipleSequenceAlignmentRow::MultipleSequenceAlignmentRow(const MultipleSequenceAlignmentRow &r, MultipleSequenceAlignment* al)
    : alignment(al),
      sequence(r.sequence),
      gaps(r.gaps),
      initialRowInDb(r.initialRowInDb)
{
    SAFE_POINT(alignment != NULL, "Parent MultipleSequenceAlignment is NULL", );
}

QByteArray MultipleSequenceAlignmentRow::toByteArray(int length, U2OpStatus& os) const {
    if (length < getCoreEnd()) {
        coreLog.trace("Incorrect length was passed to MultipleSequenceAlignmentRow::toByteArray!");
        os.setError("Failed to get row data!");
        return QByteArray();
    }

    if (gaps.isEmpty() && sequence.length() == length) {
        return sequence.constSequence();
    }

    QByteArray bytes = joinCharsAndGaps(true, true);

    // Append additional gaps, if necessary
    if (length > bytes.count()) {
        QByteArray gapsBytes;
        gapsBytes.fill(MAlignment_GapChar, length - bytes.count());
        bytes.append(gapsBytes);
    }
    if (length < bytes.count()) {
        // cut extra trailing gaps
        bytes = bytes.left(length);
    }

    return bytes;
}

int MultipleSequenceAlignmentRow::getRowLength() const {
    SAFE_POINT(alignment != NULL, "Parent MultipleSequenceAlignment is NULL", getRowLengthWithoutTrailing());
    return alignment->getLength();
}

QByteArray MultipleSequenceAlignmentRow::getCore() const {
    return joinCharsAndGaps(false, false);
}

QByteArray MultipleSequenceAlignmentRow::getData() const {
    return joinCharsAndGaps(true, true);
}

void MultipleSequenceAlignmentRow::splitBytesToCharsAndGaps(const QByteArray& input, QByteArray& seqBytes, QList<U2MsaGap>& gapsModel) {
    MsaDbiUtils::splitBytesToCharsAndGaps(input, seqBytes, gapsModel);
}

void MultipleSequenceAlignmentRow::addOffsetToGapModel(QList<U2MsaGap>& gapModel, int offset) {
    if (0 == offset) {
        return;
    }

    if (!gapModel.isEmpty()) {

        U2MsaGap& firstGap = gapModel[0];
        if (0 == firstGap.offset) {
            firstGap.gap += offset;
        }
        else {
            SAFE_POINT(offset >= 0, "Negative gap offset!", );
            U2MsaGap beginningGap(0, offset);
            gapModel.insert(0, beginningGap);
        }

        // Shift other gaps
        if (gapModel.count() > 1) {
            for (int i = 1; i < gapModel.count(); ++i) {
                qint64 newOffset = gapModel[i].offset + offset;
                SAFE_POINT(newOffset >= 0, "Negative gap offset!", );
                gapModel[i].offset = newOffset;
            }
        }
    }
    else {
        SAFE_POINT(offset >= 0, "Negative gap offset!", );
        U2MsaGap gap(0, offset);
        gapModel.append(gap);
    }
}

QByteArray MultipleSequenceAlignmentRow::joinCharsAndGaps(bool keepOffset, bool keepTrailingGaps) const {
    QByteArray bytes = sequence.constSequence();
    int beginningOffset = 0;

    if (gaps.isEmpty()) {
        return bytes;
    }

    for (int i = 0; i < gaps.size(); ++i) {
        QByteArray gapsBytes;
        if (!keepOffset && (0 == gaps[i].offset)) {
            beginningOffset = gaps[i].gap;
            continue;
        }

        gapsBytes.fill(MAlignment_GapChar, gaps[i].gap);
        bytes.insert(gaps[i].offset - beginningOffset, gapsBytes);
    }
    SAFE_POINT(alignment != NULL, "Parent MultipleSequenceAlignment is NULL", QByteArray());
    if (keepTrailingGaps && bytes.size() < alignment->getLength()) {
        QByteArray gapsBytes;
        gapsBytes.fill(MAlignment_GapChar, alignment->getLength() - bytes.size());
        bytes.append(gapsBytes);
    }

    return bytes;
}

int MultipleSequenceAlignmentRow::getCoreLength() const {
    int coreStart = getCoreStart();
    int coreEnd = getCoreEnd();
    int length = coreEnd - coreStart;
    SAFE_POINT(length >= 0, QString("Internal error in MAlignamentRow:"
        " coreEnd is %1, coreStart is %2!").arg(coreEnd).arg(coreStart), length);
    return length;
}

void MultipleSequenceAlignmentRow::append(const MultipleSequenceAlignmentRow& anotherRow, int lengthBefore, U2OpStatus& os) {
    int rowLength = getRowLengthWithoutTrailing();

    if (lengthBefore < rowLength/*getRowLengthWithoutTrailing()*/) {
        coreLog.trace(QString("Internal error: incorrect length '%1' were passed to MultipleSequenceAlignmentRow::append,"
            "coreEnd is '%2'").arg(lengthBefore).arg(getCoreEnd()));
        os.setError("Failed to append one row to another!");
        return;
    }

    // Gap between rows
    if (lengthBefore > rowLength) {
        gaps.append(U2MsaGap(getRowLengthWithoutTrailing(),
                             lengthBefore - getRowLengthWithoutTrailing()));
    }

    // Merge gaps
    QList<U2MsaGap> anotherRowGaps = anotherRow.getGapModel();
    for (int i = 0; i < anotherRowGaps.count(); ++i) {
        anotherRowGaps[i].offset += lengthBefore;
    }
    gaps.append(anotherRowGaps);
    mergeConsecutiveGaps();

    // Merge sequences
    DNASequenceUtils::append(sequence, anotherRow.sequence, os);
}

U2MsaRow MultipleSequenceAlignmentRow::getRowDBInfo() const {
    U2MsaRow row;
    row.rowId = initialRowInDb.rowId;
    row.sequenceId = initialRowInDb.sequenceId;
    row.gstart = 0;
    row.gend = sequence.length();
    row.gaps = gaps;
    row.length = getRowLengthWithoutTrailing();
    return row;
}

void MultipleSequenceAlignmentRow::setRowDbInfo(const U2MsaRow &dbRow) {
    initialRowInDb = dbRow;
}

void MultipleSequenceAlignmentRow::setRowContent(const QByteArray& bytes, int offset, U2OpStatus& /* os */) {
    QByteArray newSequenceBytes;
    QList<U2MsaGap> newGapsModel;

    splitBytesToCharsAndGaps(bytes, newSequenceBytes, newGapsModel);
    DNASequence newSequence(getName(), newSequenceBytes);

    addOffsetToGapModel(newGapsModel, offset);

    sequence = newSequence;
    gaps = newGapsModel;
    removeTrailingGaps();
}

void MultipleSequenceAlignmentRow::setGapModel(const QList<U2MsaGap> &newGapModel) {
    gaps = newGapModel;
    removeTrailingGaps();
}

void MultipleSequenceAlignmentRow::setSequence(const DNASequence& newSequence) {
    SAFE_POINT(!newSequence.constSequence().contains(MAlignment_GapChar),
        "The sequence must be without gaps!", );
    sequence = newSequence;
}

char MultipleSequenceAlignmentRow::charAt(int pos) const {
    return MsaRowUtils::charAt(sequence.seq, gaps, pos);
}

void MultipleSequenceAlignmentRow::insertGaps(int pos, int count, U2OpStatus& os) {
    if (count < 0) {
        coreLog.trace(QString("Internal error: incorrect parameters were passed to MultipleSequenceAlignmentRow::insertGaps,"
            "pos '%1', count '%2'!").arg(pos).arg(count));
        os.setError("Failed to insert gaps into a row!");
        return;
    }

    if (pos < 0 || pos >= getRowLengthWithoutTrailing()) {
        return;
    }

    if (0 == pos) {
        addOffsetToGapModel(gaps, count);
    }
    else {
        // A gap is near
        if (MAlignment_GapChar == charAt(pos) ||
            MAlignment_GapChar == charAt(pos - 1))
        {
            // Find the gaps and append 'count' gaps to it
            // Shift all gaps that further in the row
            for (int i = 0; i < gaps.count(); ++i) {
                if (pos >= gaps[i].offset) {
                    if (pos <= gaps[i].offset + gaps[i].gap) {
                        gaps[i].gap += count;
                    }
                }
                else {
                    gaps[i].offset += count;
                }
            }
        }
        // Insert between chars
        else {
            bool found = false;

            int indexGreaterGaps = 0;
            for (int i = 0; i < gaps.count(); ++i) {
                if (pos > gaps[i].offset + gaps[i].gap) {
                    continue;
                }
                else {
                    found = true;
                    U2MsaGap newGap(pos, count);
                    gaps.insert(i, newGap);
                    indexGreaterGaps = i;
                    break;
                }
            }

            // If found somewhere between existent gaps
            if (found) {
                // Shift further gaps
                for (int i = indexGreaterGaps + 1; i < gaps.count(); ++i) {
                    gaps[i].offset += count;
                }
            }
            // This is the last gap
            else {
                U2MsaGap newGap(pos, count);
                gaps.append(newGap);
                return;
            }
        }
    }
}

void MultipleSequenceAlignmentRow::mergeConsecutiveGaps() {
    QList<U2MsaGap> newGapModel;
    if (gaps.isEmpty()) {
        return;
    }

    newGapModel << gaps[0];
    int indexInNewGapModel = 0;
    for (int i = 1; i < gaps.count(); ++i) {
        int previousGapEnd = newGapModel[indexInNewGapModel].offset +
            newGapModel[indexInNewGapModel].gap - 1;
        int currectGapStart = gaps[i].offset;
        SAFE_POINT(currectGapStart > previousGapEnd,
            "Incorrect gap model during merging consecutive gaps!",);
        if (currectGapStart == previousGapEnd + 1) {
            // Merge gaps
            qint64 newGapLength = newGapModel[indexInNewGapModel].gap + gaps[i].gap;
            SAFE_POINT(newGapLength > 0, "Non-positive gap length!", )
            newGapModel[indexInNewGapModel].gap = newGapLength;
        }
        else {
            // Add the gap to the list
            newGapModel << gaps[i];
            indexInNewGapModel++;
        }
    }
    gaps = newGapModel;
}

void MultipleSequenceAlignmentRow::removeTrailingGaps() {
    if (gaps.isEmpty()) {
        return;
    }

    // If the last char in the row is gap, remove the last gap
    if (MAlignment_GapChar == charAt(MsaRowUtils::getRowLength(sequence.constData(), gaps) - 1)) {
        gaps.removeLast();
    }
}

void MultipleSequenceAlignmentRow::removeChars(int pos, int count, U2OpStatus& os) {
    if (pos < 0 || count < 0) {
        coreLog.trace(QString("Internal error: incorrect parameters were passed to MultipleSequenceAlignmentRow::removeChars,"
            "pos '%1', count '%2'!").arg(pos).arg(count));
        os.setError("Can't remove chars from a row!");
        return;
    }

    if (pos >= getRowLengthWithoutTrailing()) {
        return;
    }

    if (pos < getRowLengthWithoutTrailing()) {
        int startPosInSeq = -1;
        int endPosInSeq = -1;
        getStartAndEndSequencePositions(pos, count, startPosInSeq, endPosInSeq);

        // Remove inside a gap
        if ((startPosInSeq < endPosInSeq) && (-1 != startPosInSeq) && (-1 != endPosInSeq)) {
            DNASequenceUtils::removeChars(sequence, startPosInSeq, endPosInSeq, os);
            CHECK_OP(os, );
        }
    }

    // Remove gaps from the gaps model
    removeGapsFromGapModel(pos, count);

    removeTrailingGaps();
    mergeConsecutiveGaps();
}

bool MultipleSequenceAlignmentRow::isRowContentEqual(const MultipleSequenceAlignmentRow& row) const {
    if (MatchExactly == DNASequenceUtils::compare(sequence, row.getSequence())) {
        if (sequence.length() == 0) {
            return true;
        }
        else {
            QList<U2MsaGap> firstRowGaps = gaps;
            if  (!firstRowGaps.isEmpty() &&
                (MAlignment_GapChar == charAt(0)))
            {
                firstRowGaps.removeFirst();
            }

            QList<U2MsaGap> secondRowGaps = row.getGapModel();
            if (!secondRowGaps.isEmpty() &&
                (MAlignment_GapChar == row.charAt(0)))
            {
                secondRowGaps.removeFirst();
            }

            if (firstRowGaps == secondRowGaps) {
                return true;
            }
        }
    }

    return false;
}

int MultipleSequenceAlignmentRow::getUngappedPosition(int pos) const {
    return MsaRowUtils::getUngappedPosition(sequence.seq, gaps, pos);
}

int MultipleSequenceAlignmentRow::getBaseCount(int before) const {
    const int rowLength = MsaRowUtils::getRowLength(sequence.seq, gaps);
    const int trimmedRowPos = before < rowLength ? before : rowLength;
    return MsaRowUtils::getUngappedPosition(sequence.seq, gaps, trimmedRowPos, true);
}

void MultipleSequenceAlignmentRow::getStartAndEndSequencePositions(int pos, int count, int& startPosInSeq, int& endPosInSeq) {
    int rowLengthWithoutTrailingGap = getRowLengthWithoutTrailing();
    SAFE_POINT(pos < rowLengthWithoutTrailingGap,
        QString("Incorrect position '%1' in MultipleSequenceAlignmentRow::getStartAndEndSequencePosition,"
        " row length without trailing gaps is '%2'!").arg(pos).arg(rowLengthWithoutTrailingGap),);

    // Remove chars from the sequence
    // Calculate start position in the sequence
    if (MAlignment_GapChar == charAt(pos)) {
        int i = 1;
        while (MAlignment_GapChar == charAt(pos + i)) {
            if (getRowLength() == pos + i) {
                break;
            }
            i++;
        }
        startPosInSeq = getUngappedPosition(pos + i);
    }
    else {
        startPosInSeq = getUngappedPosition(pos);
    }

    // Calculate end position in the sequence
    int endRegionPos = pos + count; // non-inclusive

    if (endRegionPos > rowLengthWithoutTrailingGap) {
        endRegionPos = rowLengthWithoutTrailingGap;
    }

    if (endRegionPos == rowLengthWithoutTrailingGap) {
        endPosInSeq = getUngappedLength();
    }
    else {
        if (MAlignment_GapChar == charAt(endRegionPos)) {
            int i = 1;
            while (MAlignment_GapChar == charAt(endRegionPos + i)) {
                if (getRowLength() == endRegionPos + i) {
                    break;
                }
                i++;
            }
            endPosInSeq = getUngappedPosition(endRegionPos + i);
        }
        else {
            endPosInSeq = getUngappedPosition(endRegionPos);
        }
    }
}

void MultipleSequenceAlignmentRow::removeGapsFromGapModel(int pos, int count) {
    QList<U2MsaGap> newGapModel;
    int endRegionPos = pos + count; // non-inclusive
    foreach (U2MsaGap gap, gaps)
    {
        qint64 gapEnd = gap.offset + gap.gap;
        if (gapEnd < pos) {
            newGapModel << gap;
        }
        else if (gapEnd <= endRegionPos) {
            if (gap.offset < pos) {
                gap.gap = pos - gap.offset;
                newGapModel << gap;
            }
            // Otherwise just remove the gap (do not write to the new gap model)
        }
        else {
            if (gap.offset < pos) {
                gap.gap -= count;
                SAFE_POINT(gap.gap >= 0, "Non-positive gap length!", );
                newGapModel << gap;
            }
            else if (gap.offset < endRegionPos) {
                gap.gap = gapEnd - endRegionPos;
                gap.offset = pos;
                SAFE_POINT(gap.gap > 0, "Non-positive gap length!", );
                SAFE_POINT(gap.offset >= 0, "Negative gap offset!", );
                newGapModel << gap;
            }
            else {
                // Shift the gap
                gap.offset -= count;
                SAFE_POINT(gap.offset >= 0, "Negative gap offset!", );
                newGapModel << gap;
            }
        }
    }

    gaps = newGapModel;
}

void MultipleSequenceAlignmentRow::crop(int pos, int count, U2OpStatus& os) {
    if (pos < 0 || count < 0) {
        coreLog.trace(QString("Internal error: incorrect parameters were passed to MultipleSequenceAlignmentRow::crop,"
            "startPos '%1', length '%2', row length '%3'!").arg(pos).arg(count).arg(getRowLength()));
        os.setError("Can't crop a row!");
        return;
    }

    int initialRowLength = getRowLength();
    int initialSeqLength = getUngappedLength();

    if (pos >= getRowLengthWithoutTrailing()) {
        // Clear the row content
        DNASequenceUtils::makeEmpty(sequence);
    }
    else {
        int startPosInSeq = -1;
        int endPosInSeq = -1;
        getStartAndEndSequencePositions(pos, count, startPosInSeq, endPosInSeq);

        // Remove inside a gap
        if ((startPosInSeq <= endPosInSeq) && (-1 != startPosInSeq) && (-1 != endPosInSeq))
        {
            if (endPosInSeq < initialSeqLength){
                DNASequenceUtils::removeChars(sequence, endPosInSeq, getUngappedLength(), os);
                CHECK_OP(os, );
            }

            if (startPosInSeq > 0) {
                DNASequenceUtils::removeChars(sequence, 0, startPosInSeq, os);
                CHECK_OP(os, );
            }
        }
    }

    if (pos + count < initialRowLength) {
        removeGapsFromGapModel(pos + count, initialRowLength - pos - count);
    }

    if (pos > 0) {
        removeGapsFromGapModel(0, pos);
    }
    removeTrailingGaps();
}

MultipleSequenceAlignmentRow MultipleSequenceAlignmentRow::mid(int pos, int count, U2OpStatus& os) const {
    MultipleSequenceAlignmentRow row = *this;
    row.crop(pos, count, os);
    return row;
}

bool MultipleSequenceAlignmentRow::operator==(const MultipleSequenceAlignmentRow& row) const {
    return isRowContentEqual(row);
}

void MultipleSequenceAlignmentRow::toUpperCase() {
    DNASequenceUtils::toUpperCase(sequence);
}

bool gapLessThan(const U2MsaGap& gap1, const U2MsaGap& gap2) {
    return gap1.offset < gap2.offset;
}

void MultipleSequenceAlignmentRow::replaceChars(char origChar, char resultChar, U2OpStatus& os) {
    if (MAlignment_GapChar == origChar) {
        coreLog.trace("The original char can't be a gap in MultipleSequenceAlignmentRow::replaceChars!");
        os.setError("Failed to replace chars in an alignment row!");
        return;
    }

    if (MAlignment_GapChar == resultChar) {
        // Get indexes of all 'origChar' characters in the row sequence
        QList<int> gapsIndexes;
        for (int i = 0; i < getRowLength(); i++) {
            if (origChar == charAt(i)) {
                gapsIndexes.append(i);
            }
        }

        if (gapsIndexes.isEmpty()) {
            return; // There is nothing to replace
        }

        // Remove all 'origChar' characters from the row sequence
        sequence.seq.replace(origChar, "");

        // Re-calculate the gaps model
        QList<U2MsaGap> newGapsModel = gaps;
        for (int i = 0; i < gapsIndexes.size(); ++i) {
            int index = gapsIndexes[i];
            U2MsaGap gap(index, 1);
            newGapsModel.append(gap);
        }
        qSort(newGapsModel.begin(), newGapsModel.end(), gapLessThan);

        // Replace the gaps model with the new one
        gaps = newGapsModel;
        mergeConsecutiveGaps();
    }
    else {
        // Just replace all occurrences of 'origChar' by 'resultChar'
        sequence.seq.replace(origChar, resultChar);
    }
}


//////////////////////////////////////////////////////////////////////////
// MultipleSequenceAlignment

// Helper class to call MultipleSequenceAlignment state check
class MAStateCheck {
public:
    MAStateCheck(const MultipleSequenceAlignment* _ma) : ma(_ma) {}

    ~MAStateCheck() {
#ifdef _DEBUG
        ma->check();
#endif
    }
    const MultipleSequenceAlignment* ma;
};

MultipleSequenceAlignment::MultipleSequenceAlignment(const QString& _name, const DNAAlphabet* al, const QList<MultipleSequenceAlignmentRow>& r)
: alphabet(al), rows(r)
{
    MAStateCheck check(this);

    SAFE_POINT(al==NULL || !_name.isEmpty(), "Incorrect parameters in MultipleSequenceAlignment ctor!", );

    setName(_name );
    length = 0;
    for (int i = 0, n = rows.size(); i < n; i++) {
        const MultipleSequenceAlignmentRow& r = rows.at(i);
        length = qMax(length, r.getCoreEnd());
    }
}

MultipleSequenceAlignment::MultipleSequenceAlignment(const MultipleSequenceAlignment &m)
    : alphabet(m.alphabet),
      length(m.length),
      info(m.info)
{
    U2OpStatusImpl os;
    for (int i = 0; i < m.rows.size(); i++) {
        MultipleSequenceAlignmentRow r = createRow( m.rows.at(i), os);
        addRow(r, m.length, i, os);
        SAFE_POINT_OP(os, );
    }
}

MultipleSequenceAlignment & MultipleSequenceAlignment::operator=(const MultipleSequenceAlignment &other) {
    clear();

    alphabet = other.alphabet;
    length = other.length;
    info = other.info;

    U2OpStatusImpl os;
    for (int i = 0; i < other.rows.size(); i++) {
        const MultipleSequenceAlignmentRow r = createRow(other.rows.at(i), os);
        addRow(r, other.length, i, os);
        SAFE_POINT_OP(os, *this);
    }

    return *this;
}

void MultipleSequenceAlignment::setAlphabet(const DNAAlphabet* al) {
    SAFE_POINT(NULL != al, "Internal error: attempted to set NULL alphabet fro an alignment!",);
    alphabet = al;
}

bool MultipleSequenceAlignment::trim( bool removeLeadingGaps ) {
    MAStateCheck check(this);
    Q_UNUSED(check);

    bool result = false;

    if ( removeLeadingGaps ) {
        // Verify if there are leading columns of gaps
        // by checking the first gap in each row
        qint64 leadingGapColumnsNum = 0;
        foreach (MultipleSequenceAlignmentRow row, rows) {
            if (row.getGapModel().count() > 0) {
                U2MsaGap firstGap = row.getGapModel().first();
                if (firstGap.offset > 0) {
                    leadingGapColumnsNum = 0;
                    break;
                }
                else {
                    if (leadingGapColumnsNum == 0) {
                        leadingGapColumnsNum = firstGap.gap;
                    }
                    else {
                        leadingGapColumnsNum = qMin(leadingGapColumnsNum, firstGap.gap);
                    }
                }
            }
            else {
                leadingGapColumnsNum = 0;
                break;
            }
        }

        // If there are leading gap columns, remove them
        U2OpStatus2Log os;
        if (leadingGapColumnsNum > 0) {
            for (int i = 0; i < rows.count(); ++i) {
                rows[i].removeChars(0, leadingGapColumnsNum, os);
                CHECK_OP(os, true);
                result = true;
            }
        }
    }

    // Verify right side of the alignment (trailing gaps and rows' lengths)
    int newLength = 0;
    foreach (MultipleSequenceAlignmentRow row, rows) {
        if (newLength == 0) {
            newLength = row.getRowLengthWithoutTrailing();
        }
        else {
            newLength = qMax(row.getRowLengthWithoutTrailing(), newLength);
        }
    }

    if (newLength != length) {
        length = newLength;
        result = true;
    }

    return result;
}

bool MultipleSequenceAlignment::simplify() {
    MAStateCheck check(this);
    Q_UNUSED(check);

    int newLen = 0;
    bool changed = false;
    for (int i=0, n = rows.size(); i < n; i++) {
        MultipleSequenceAlignmentRow& r = rows[i];
        changed = r.simplify() || changed;
        newLen = qMax(newLen, r.getCoreEnd());
    }
    if (!changed) {
        assert(length == newLen);
        return false;
    }
    length = newLen;
    return true;
}

bool MultipleSequenceAlignment::hasEmptyGapModel( ) const {
    for ( int i = 0, n = rows.size( ); i < n; ++i ) {
        const MultipleSequenceAlignmentRow &row = rows.at( i );
        if ( !row.getGapModel( ).isEmpty( ) ) {
            return false;
        }
    }
    return true;
}

bool MultipleSequenceAlignment::hasEqualLength() const {
    const int defaultSequenceLength = -1;
    int sequenceLength = defaultSequenceLength;
    for ( int i = 0, n = rows.size( ); i < n; ++i ) {
        const MultipleSequenceAlignmentRow &row = rows.at( i );
        if ( defaultSequenceLength != sequenceLength
            && sequenceLength != row.getUngappedLength() )
        {
            return false;
        } else {
            sequenceLength = row.getUngappedLength();
        }
    }
    return true;
}

void MultipleSequenceAlignment::clear() {
    MAStateCheck check(this);
    Q_UNUSED(check);

    rows.clear();
    length = 0;
}

MultipleSequenceAlignment MultipleSequenceAlignment::mid(int start, int len) const {
    static MultipleSequenceAlignment emptyAlignment;
    SAFE_POINT(start >= 0 && start + len <= length,
        QString("Incorrect parameters were passed to MultipleSequenceAlignment::mid:"
        "start '%1', len '%2', the alignment length is '%3'!").arg(start).arg(len).arg(length),
        emptyAlignment);

    MultipleSequenceAlignment res(getName(), alphabet);
    MAStateCheck check(&res);
    Q_UNUSED(check);

    U2OpStatus2Log os;
    foreach(const MultipleSequenceAlignmentRow& r, rows) {
        MultipleSequenceAlignmentRow mRow = r.mid(start, len, os);
        mRow.setParentAlignment(&res);
        res.rows.append(mRow);
    }
    res.length = len;
    return res;
}

U2MsaGapModel MultipleSequenceAlignment::getGapModel() const {
    U2MsaGapModel gapModel;
    foreach (const MultipleSequenceAlignmentRow &row, rows) {
        gapModel << row.getGapModel();
    }
    return gapModel;
}

MultipleSequenceAlignment& MultipleSequenceAlignment::operator+=(const MultipleSequenceAlignment& ma) {
    MAStateCheck check(this);
    Q_UNUSED(check);

    SAFE_POINT(ma.alphabet == alphabet, "Different alphabets in MultipleSequenceAlignment::operator+= !", *this);

    int nSeq = getNumRows();
    SAFE_POINT(ma.getNumRows() == nSeq, "Different number of rows in MultipleSequenceAlignment::operator+= !", *this);

    U2OpStatus2Log os;
    for (int i=0; i < nSeq; i++) {
        MultipleSequenceAlignmentRow& myRow = rows[i];
        const MultipleSequenceAlignmentRow& anotherRow = ma.rows.at(i);
        myRow.append(anotherRow, length, os);
    }

    length += ma.length;
    return *this;
}

bool MultipleSequenceAlignment::operator==(const MultipleSequenceAlignment& other) const {
    bool lengthsAreEqual = (length==other.length);
    bool alphabetsAreEqual = (alphabet == other.alphabet);
    bool rowsAreEqual = (rows == other.rows);
//    bool infosAreEqual = (info == other.info);

    return lengthsAreEqual && alphabetsAreEqual && rowsAreEqual;// && infosAreEqual;
}

bool MultipleSequenceAlignment::operator!=(const MultipleSequenceAlignment& other) const {
    return !operator==(other);
}

int MultipleSequenceAlignment::estimateMemorySize() const {
    int result = info.size() * 20; //approximate info size estimation
    foreach(const MultipleSequenceAlignmentRow& r, rows) {
        result += r.getCoreLength() + getName().length() * 2  + 12; //+12 -> overhead for byte array
    }
    return result;
}

bool MultipleSequenceAlignment::crop(const U2Region& region, const QSet<QString>& rowNames, U2OpStatus& os) {
    if (!(region.startPos >= 0 && region.length > 0 && region.length < length && region.startPos < length)) {
        os.setError(QString("Incorrect region was passed to MultipleSequenceAlignment::crop,"
                            "startPos '%1', length '%2'!").arg(region.startPos).arg(region.length));
        return false;
    }

    int cropLen = region.length;
    if (region.endPos() > length) {
        cropLen -=  (region.endPos() - length);
    }

    MAStateCheck check(this);
    Q_UNUSED(check);

    QList<MultipleSequenceAlignmentRow> newList;
    for (int i = 0 ; i < rows.size(); i++) {
        MultipleSequenceAlignmentRow row = rows[i];
        const QString& rowName = row.getName();
        if (rowNames.contains(rowName)){
            row.crop(region.startPos, cropLen, os);
            CHECK_OP(os, false);
            newList.append(row);
        }
    }
    rows = newList;

    length = cropLen;
    return true;
}

bool MultipleSequenceAlignment::crop(const U2Region &region, U2OpStatus& os) {
    return crop(region, getRowNames().toSet(), os);
}

bool MultipleSequenceAlignment::crop(int start, int count, U2OpStatus& os) {
    return crop(U2Region(start, count), os);
}

MultipleSequenceAlignmentRow MultipleSequenceAlignment::createRow(const QString& name, const QByteArray& bytes, U2OpStatus& /* os */) {
    QByteArray newSequenceBytes;
    QList<U2MsaGap> newGapsModel;

    MultipleSequenceAlignmentRow::splitBytesToCharsAndGaps(bytes, newSequenceBytes, newGapsModel);
    DNASequence newSequence(name, newSequenceBytes);

    U2MsaRow row;
    row.rowId = MultipleSequenceAlignmentRow::invalidRowId();

    return MultipleSequenceAlignmentRow(row, newSequence, newGapsModel, this);
}

MultipleSequenceAlignmentRow MultipleSequenceAlignment::createRow(const U2MsaRow& rowInDb, const DNASequence& sequence, const QList<U2MsaGap>& gaps, U2OpStatus& os) {
    QString errorDescr = "Failed to create a multiple alignment row!";
    if (-1 != sequence.constSequence().indexOf(MAlignment_GapChar)) {
        coreLog.trace("Attempted to create an alignment row from a sequence with gaps!");
        os.setError(errorDescr);
        return MultipleSequenceAlignmentRow();
    }

    int length = sequence.length();
    foreach (const U2MsaGap& gap, gaps) {
        if (gap.offset > length || !gap.isValid()) {
            coreLog.trace("Incorrect gap model was passed to MultipleSequenceAlignmentRow::createRow!");
            os.setError(errorDescr);
            return MultipleSequenceAlignmentRow();
        }
        length += gap.gap;
    }

    return MultipleSequenceAlignmentRow(rowInDb, sequence, gaps, this);
}

MultipleSequenceAlignmentRow MultipleSequenceAlignment::createRow(const MultipleSequenceAlignmentRow &r, U2OpStatus &/*os*/) {
    return MultipleSequenceAlignmentRow(r, this);
}


void MultipleSequenceAlignment::addRow(const MultipleSequenceAlignmentRow& row, int rowLenWithTrailingGaps, int rowIndex, U2OpStatus& /* os */) {
    MAStateCheck check(this);
    Q_UNUSED(check);

    length = qMax(rowLenWithTrailingGaps, length);
    int idx = rowIndex == -1 ? getNumRows() : qBound(0, rowIndex, getNumRows());
    rows.insert(idx, row);
}

void MultipleSequenceAlignment::addRow(const QString& name, const QByteArray& bytes, U2OpStatus& os) {
    MultipleSequenceAlignmentRow newRow = createRow(name, bytes, os);
    CHECK_OP(os, );

    addRow(newRow, bytes.size(), -1, os);
}

void MultipleSequenceAlignment::addRow(const QString& name, const QByteArray& bytes, int rowIndex, U2OpStatus& os) {
    MultipleSequenceAlignmentRow newRow = createRow(name, bytes, os);
    CHECK_OP(os, );

    addRow(newRow, bytes.size(), rowIndex, os);
}

void MultipleSequenceAlignment::addRow(const U2MsaRow& rowInDb, const DNASequence& sequence, U2OpStatus& os) {
    MultipleSequenceAlignmentRow newRow = createRow(rowInDb, sequence, rowInDb.gaps, os);
    CHECK_OP(os, );

    addRow(newRow, rowInDb.length, -1, os);
}

void MultipleSequenceAlignment::addRow(const QString& name, const DNASequence &sequence, const QList<U2MsaGap> &gaps, U2OpStatus &os) {
    U2MsaRow row;
    row.rowId = MultipleSequenceAlignmentRow::invalidRowId();

    MultipleSequenceAlignmentRow newRow = createRow(row, sequence, gaps, os);
    CHECK_OP(os, );

    int len = sequence.length();
    foreach (const U2MsaGap& gap, gaps) {
        len += gap.gap;
    }

    newRow.setName(name);
    addRow(newRow, len, -1, os);
}

void MultipleSequenceAlignment::removeRow(int rowIndex, U2OpStatus& os) {
    if (rowIndex < 0 || rowIndex >= getNumRows()) {
        coreLog.trace(QString("Internal error: incorrect parameters was passed to MultipleSequenceAlignment::removeRow,"
            "rowIndex '%1', the number of rows is '%2'!").arg(rowIndex).arg(getNumRows()));
        os.setError("Failed to remove a row!");
        return;
    }
    MAStateCheck check(this);
    Q_UNUSED(check);

    rows.removeAt(rowIndex);

    if (rows.isEmpty()) {
        length = 0;
    }
}

void MultipleSequenceAlignment::insertGaps(int row, int pos, int count, U2OpStatus& os) {
    if (row >= getNumRows() || row < 0 || pos > length || pos < 0 || count < 0) {
        coreLog.trace(QString("Internal error: incorrect parameters were passed"
            " to MultipleSequenceAlignment::insertGaps: row index '%1', pos '%2', count '%3'!").arg(row).arg(pos).arg(count));
        os.setError("Failed to insert gaps into an alignment!");
        return;
    }

    if (pos == length) {
        // add trailing gaps --> just increase alignment len
        length += count;
        return;
    }

    MAStateCheck check(this);
    Q_UNUSED(check);

    MultipleSequenceAlignmentRow& r = rows[row];
    if (pos >= r.getRowLengthWithoutTrailing()) {
        length += count;
        return;
    }
    r.insertGaps(pos, count, os);

    int rowLength = r.getRowLengthWithoutTrailing();
    length = qMax(length, rowLength);
}

void MultipleSequenceAlignment::appendChars(int row, const char* str, int len) {
    SAFE_POINT(0 <= row && row < getNumRows(),
        QString("Incorrect row index '%1' in MultipleSequenceAlignment::appendChars!").arg(row),);

    U2OpStatus2Log os;
    MultipleSequenceAlignmentRow appendedRow = createRow("", QByteArray(str, len), os);
    CHECK_OP(os, );

    int rowLength = rows[row].getRowLength();;

    rows[row].append(appendedRow, rowLength, os);
    CHECK_OP(os, );

    length = qMax(length, rowLength + len);
}

void MultipleSequenceAlignment::appendChars(int row, int afterPos, const char *str, int len) {
    SAFE_POINT(0 <= row && row < getNumRows(),
        QString("Incorrect row index '%1' in MultipleSequenceAlignment::appendChars!").arg(row),);

    U2OpStatus2Log os;
    MultipleSequenceAlignmentRow appendedRow = createRow("", QByteArray(str, len), os);
    CHECK_OP(os, );

    rows[row].append(appendedRow, afterPos, os);
    CHECK_OP(os, );

    length = qMax(length, afterPos + len);

}

void MultipleSequenceAlignment::appendRow(int row, const MultipleSequenceAlignmentRow &r, bool ignoreTrailingGaps, U2OpStatus& os) {
    appendRow(row, ignoreTrailingGaps ? rows[row].getRowLengthWithoutTrailing()
                                      : rows[row].getRowLength(), r, os);
}

void MultipleSequenceAlignment::appendRow(int row, int afterPos, const MultipleSequenceAlignmentRow &r, U2OpStatus& os) {
    SAFE_POINT(0 <= row && row < getNumRows(),
        QString("Incorrect row index '%1' in MultipleSequenceAlignment::appendRow!").arg(row),);

    rows[row].append(r, afterPos, os);
    CHECK_OP(os, );

    length = qMax(length, afterPos + r.getRowLength());
}

void MultipleSequenceAlignment::removeChars(int row, int pos, int count, U2OpStatus& os) {
    if (row >= getNumRows() || row < 0 || pos > length || pos < 0 || count < 0) {
        coreLog.trace(QString("Internal error: incorrect parameters were passed"
            " to MultipleSequenceAlignment::removeChars: row index '%1', pos '%2', count '%3'!").arg(row).arg(pos).arg(count));
        os.setError("Failed to remove chars from an alignment!");
        return;
    }

    MAStateCheck check(this);
    Q_UNUSED(check);

    MultipleSequenceAlignmentRow& r = rows[row];
    r.removeChars(pos, count, os);
}

void MultipleSequenceAlignment::removeRegion(int startPos, int startRow, int nBases, int nRows, bool removeEmptyRows) {
    SAFE_POINT(startPos >= 0 && startPos + nBases <= length && nBases > 0,
        QString("Incorrect parameters were passed to MultipleSequenceAlignment::removeRegion: startPos '%1',"
        " nBases '%2', the length is '%3'!").arg(startPos).arg(nBases).arg(length),);
    SAFE_POINT(startRow >= 0 && startRow + nRows <= getNumRows() && nRows > 0,
        QString("Incorrect parameters were passed to MultipleSequenceAlignment::removeRegion: startRow '%1',"
        " nRows '%2', the number of rows is '%3'!").arg(startRow).arg(nRows).arg(getNumRows()),);

    MAStateCheck check(this);
    Q_UNUSED(check);

    U2OpStatus2Log os;
    for (int i = startRow + nRows; --i >= startRow;) {
        MultipleSequenceAlignmentRow& r = rows[i];

        r.removeChars(startPos, nBases, os);
        SAFE_POINT_OP(os, );

        if (removeEmptyRows && (0 == r.getSequence().length())) {
            rows.removeAt(i);
        }
    }

    if (nRows == rows.size()) {
        // full columns were removed
        length -= nBases;
        if (length == 0) {
            rows.clear();
        }
    }
}

void MultipleSequenceAlignment::setLength(int newLength) {
    SAFE_POINT(newLength >=0, QString("Internal error: attempted to set length '%1' for an alignment!").arg(newLength),);

    MAStateCheck check(this);
    Q_UNUSED(check);

    if (newLength >= length) {
        length = newLength;
        return;
    }

    U2OpStatus2Log os;
    for (int i=0, n = getNumRows(); i < n; i++) {
        MultipleSequenceAlignmentRow& row = rows[i];
        row.crop(0, newLength, os);
        CHECK_OP(os, );
    }
    length = newLength;
}

void MultipleSequenceAlignment::renameRow(int row, const QString& name) {
    SAFE_POINT(row >= 0 && row < getNumRows(),
        QString("Incorrect row index '%1' was passed to MultipleSequenceAlignment::renameRow: "
        " the number of rows is '%2'!").arg(row).arg(getNumRows()),);
    SAFE_POINT(!name.isEmpty(),
        "Incorrect parameter 'name' was passed to MultipleSequenceAlignment::renameRow: "
        " Can't set the name of a row to an empty string!",);
    MultipleSequenceAlignmentRow& r = rows[row];
    r.setName(name);
}


void MultipleSequenceAlignment::replaceChars(int row, char origChar, char resultChar) {
    SAFE_POINT(row >= 0 && row < getNumRows(),
        QString("Incorrect row index '%1' in MultipleSequenceAlignment::replaceChars").arg(row),);

    if (origChar == resultChar) {
        return;
    }
    MultipleSequenceAlignmentRow& r = rows[row];
    U2OpStatus2Log os;
    r.replaceChars(origChar, resultChar, os);
}

void MultipleSequenceAlignment::setRowContent(int row, const QByteArray& sequence, int offset) {
    SAFE_POINT(row >= 0 && row < getNumRows(),
        QString("Incorrect row index '%1' was passed to MultipleSequenceAlignment::setRowContent: "
        " the number of rows is '%2'!").arg(row).arg(getNumRows()),);
    MAStateCheck check(this);
    Q_UNUSED(check);

    MultipleSequenceAlignmentRow& r = rows[row];

    U2OpStatus2Log os;
    r.setRowContent(sequence, offset, os);
    SAFE_POINT_OP(os, );

    length = qMax(length, sequence.size() + offset);
}

void MultipleSequenceAlignment::toUpperCase() {
    for (int i = 0, n = getNumRows(); i < n; i++) {
        MultipleSequenceAlignmentRow& row = rows[i];
        row.toUpperCase();
    }
}

class CompareMARowsByName {
public:
    CompareMARowsByName(bool _asc = true) : asc(_asc){}
    bool operator()(const MultipleSequenceAlignmentRow& row1, const MultipleSequenceAlignmentRow& row2) const {
        bool res = QString::compare(row1.getName(), row2.getName(), Qt::CaseInsensitive) > 0;
        return asc ? !res : res;
    }

    bool asc;
};

void MultipleSequenceAlignment::sortRowsByName(bool asc) {
    MAStateCheck check(this);

    qStableSort(rows.begin(), rows.end(), CompareMARowsByName(asc));
}

bool MultipleSequenceAlignment::sortRowsBySimilarity(QVector<U2Region>& united) {
    QList<MultipleSequenceAlignmentRow> oldRows = rows;
    QList<MultipleSequenceAlignmentRow> sortedRows;
    while (!oldRows.isEmpty()) {
        const MultipleSequenceAlignmentRow& r = oldRows.takeFirst();
        sortedRows.append(r);
        int start = sortedRows.size() - 1;
        int len = 1;
        QMutableListIterator<MultipleSequenceAlignmentRow> iter(oldRows);
        while (iter.hasNext()) {
            const MultipleSequenceAlignmentRow& next = iter.next();
            if(next.isRowContentEqual(r)) {
                sortedRows.append(next);
                iter.remove();
                ++len;
            }
        }
        if (len > 1) {
            united.append(U2Region(start, len));
        }
    }
    if(rows != sortedRows) {
        rows = sortedRows;
        return true;
    }
    return false;
}

void MultipleSequenceAlignment::moveRowsBlock(int startRow, int numRows, int delta)
{
    MAStateCheck check(this);

    // Assumption: numRows is rather big, delta is small (1~2)
    // It's more optimal to move abs(delta) of rows then the block itself

    int i = 0;
    int k = qAbs(delta);

    SAFE_POINT(( delta > 0 && startRow + numRows + delta - 1 < rows.length())
        || (delta < 0 && startRow + delta >= 0),
        QString("Incorrect parameters in MultipleSequenceAlignment::moveRowsBlock: "
        "startRow: '%1', numRows: '%2', delta: '%3'").arg(startRow).arg(numRows).arg(delta),);

    QList<MultipleSequenceAlignmentRow> toMove;
    int fromRow = delta > 0 ? startRow + numRows  : startRow + delta;

    while (i <  k) {
        MultipleSequenceAlignmentRow row = rows.takeAt(fromRow);
        toMove.append(row);
        i++;
    }

    int toRow = delta > 0 ? startRow : startRow + numRows - k;

    while (toMove.count() > 0) {
        int n = toMove.count();
        MultipleSequenceAlignmentRow row = toMove.at(n - 1);
        toMove.removeAt(n - 1);
        rows.insert(toRow, row);
    }
}

QStringList MultipleSequenceAlignment::getRowNames() const {
    QStringList rowNames;
    foreach (const MultipleSequenceAlignmentRow& r, rows) {
        rowNames.append(r.getName());
    }
    return rowNames;
}

QList<qint64> MultipleSequenceAlignment::getRowsIds() const {
    QList<qint64> rowIds;
    foreach (const MultipleSequenceAlignmentRow& row, rows) {
        rowIds.append(row.getRowId());
    }
    return rowIds;
}

MultipleSequenceAlignmentRow MultipleSequenceAlignment::getRowByRowId(qint64 rowId, U2OpStatus& os) const {
    foreach (const MultipleSequenceAlignmentRow& row, rows) {
        if (row.getRowId() == rowId) {
            return row;
        }
    }
    os.setError("Failed to find a row in an alignment!");
    return MultipleSequenceAlignmentRow();
}

int MultipleSequenceAlignment::getRowIndexByRowId( qint64 rowId, U2OpStatus &os ) const {
    for ( int rowIndex = 0; rowIndex < rows.size( ); ++rowIndex ) {
        if ( rows.at( rowIndex ).getRowId( ) == rowId ) {
            return rowIndex;
        }
    }
    os.setError("Invalid row id!");
    return MultipleSequenceAlignmentRow::invalidRowId();
}

char MultipleSequenceAlignment::charAt(int rowIndex, int pos) const {
    const MultipleSequenceAlignmentRow& mai = rows[rowIndex];
    char c = mai.charAt(pos);
    return c;
}

void MultipleSequenceAlignment::setRowGapModel(int rowIndex, const QList<U2MsaGap>& gapModel) {
    SAFE_POINT(rowIndex >= 0 && rowIndex < getNumRows(), "Invalid row index!", );
    MultipleSequenceAlignmentRow& row = rows[rowIndex];
    length = qMax(length, MsaRowUtils::getGapsLength(gapModel) + row.sequence.length());
    row.setGapModel(gapModel);
}

void MultipleSequenceAlignment::setRowId(int rowIndex, qint64 rowId) {
    SAFE_POINT(rowIndex >= 0 && rowIndex < getNumRows(), "Invalid row index!", );

    MultipleSequenceAlignmentRow& row = rows[rowIndex];
    row.setRowId(rowId);
}

void MultipleSequenceAlignment::setSequenceId(int rowIndex, U2DataId sequenceId) {
    SAFE_POINT(rowIndex >= 0 && rowIndex < getNumRows(), "Invalid row index!", );

    MultipleSequenceAlignmentRow& row = rows[rowIndex];
    row.setSequenceId(sequenceId);
}

void MultipleSequenceAlignment::check() const {
#ifdef DEBUG
    assert(getNumRows() != 0 || length == 0);
    for (int i = 0, n = getNumRows(); i < n; i++) {
        const MultipleSequenceAlignmentRow& row = rows.at(i);
        assert(row.getCoreEnd() <= length);
    }
#endif
}

bool MultipleSequenceAlignment::sortRowsByList(const QStringList& rowsOrder) {
    MAStateCheck check(this);

    const QStringList& rowNames = getRowNames();
    foreach(const QString& rowName, rowNames) {
        CHECK(rowsOrder.contains(rowName), false);
    }

    QList<MultipleSequenceAlignmentRow> sortedRows;
    foreach(const QString& rowName, rowsOrder) {
        int rowIndex = rowNames.indexOf(rowName);
        if(rowIndex >= 0) {
            const MultipleSequenceAlignmentRow& curRow = rows.at(rowIndex);
            sortedRows.append(curRow);
        }
    }

    rows = sortedRows;
    return true;
}

const MultipleSequenceAlignmentRow& MultipleSequenceAlignment::getRow( QString name ) const{
    static MultipleSequenceAlignmentRow emptyRow;
    for(int i = 0;i < rows.count();i++){
        if(rows.at(i).getName() == name){
            return rows.at(i);
        }
    }
    SAFE_POINT(false,
        "Internal error: row name passed to MAlignmnet::getRow function not exists!",
        emptyRow);
}

static bool _registerMeta() {
    qRegisterMetaType<MultipleSequenceAlignment>("MultipleSequenceAlignment");
    return true;
}

bool MultipleSequenceAlignment::registerMeta = _registerMeta();

} // namespace U2
