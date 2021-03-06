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

#ifndef _U2_SAVE_SELECTED_SEQUENCES_DIALOG_CONTROLLER
#define _U2_SAVE_SELECTED_SEQUENCES_DIALOG_CONTROLLER

#include <QDialog>

#include <U2Core/DocumentModel.h>

#include <U2Gui/SaveDocumentController.h>

class Ui_SaveSelectedSequenceFromMSADialog;

namespace U2 {

class SaveDocumentInFolderController;

class SaveSelectedSequenceFromMSADialogController : public QDialog {
    Q_OBJECT
public:
    SaveSelectedSequenceFromMSADialogController(const QString& defaultDir, QWidget* p, const QStringList& seqNames);
    ~SaveSelectedSequenceFromMSADialogController();

    virtual void accept();

    QString             url;
    QString             defaultDir;
    DocumentFormatId    format;
    QStringList         seqNames;
    QString             customFileName;
    bool                trimGapsFlag;
    bool                addToProjectFlag;

private slots:
    void sl_nameCBIndexChanged(int index);
private:
    void initSaveController();

    SaveDocumentInFolderController* saveController;
    Ui_SaveSelectedSequenceFromMSADialog* ui;
};

class SaveDocumentInFolderControllerConfig : public SaveDocumentControllerConfig {
public:
    SaveDocumentInFolderControllerConfig();

    QLineEdit *folderLineEdit;
};

class SaveDocumentInFolderController : public QObject {
    Q_OBJECT
public:
    SaveDocumentInFolderController(const SaveDocumentInFolderControllerConfig& config,
                            const DocumentFormatConstraints& formatConstraints,
                            QObject* parent);

    QString getSaveDirName() const;
    //DocumentFormatId getFormatIdToSave() const;
signals:
    void si_pathChanged(const QString &path);
private slots:
    void sl_fileDialogButtonClicked();
private:
    void init();
    void setPath(const QString &path);
    void initFormatComboBox();

    SaveDocumentInFolderControllerConfig        conf;
    SaveDocumentController::SimpleFormatsInfo   formatsInfo;
    //QString                                     currentFormat;

    static const QString HOME_DIR_IDENTIFIER;
};

}

#endif
