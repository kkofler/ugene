#include "ProjectViewTests.h"

#include <QtGui>
#include <U2Core/ProjectModel.h>
#include <U2Core/Task.h>

namespace U2 {

const QString ProjectViewTests::projectViewName = "documentTreeWidget";



void ProjectViewTests::openFile(const QString &path) {
    Task *t = AppContext::getProjectLoader()->openProjectTask(path, false);
    t->moveToThread(QApplication::instance()->thread());
    emit runTask(t);
    //waitForTask(t);
}

void ProjectViewTests::addObjectToView(const QString &objectName) {
    QTreeView *projectViewTree = static_cast<QTreeView*>(findWidgetByName(projectViewName));

    QPoint pos = getItemPosition(objectName, projectViewName);
    moveTo(projectViewName, pos);
    mousePressOnItem(projectViewName, Qt::LeftButton, pos);
    contextMenuOnItem(projectViewName, pos);
    clickContextMenu("Add to view");
    waitForMenuWithAction("Add to view: _1 3INS chain 2 sequence");
    clickContextMenu("Add to view: _1 3INS chain 2 sequence");
}

void ProjectViewTests::openDocumentInView(const QString &objectName) {
    QTreeView *projectViewTree = static_cast<QTreeView*>(findWidgetByName(projectViewName));

    QPoint pos = getItemPosition(objectName, projectViewName);
    moveTo(projectViewName, pos);
    mousePressOnItem(projectViewName, Qt::LeftButton, pos);
    contextMenuOnItem(projectViewName, pos);
    sleep(500);
    clickContextMenu("Open view");
    sleep(500);
    clickContextMenu("Open new view: Sequence view");
    sleep(2000);
}

QString TaskViewTest::getTaskProgress(const QString &taskName) {
    QTreeWidget * w = static_cast<QTreeWidget*>(findWidgetByName(taskViewWidgetName));
    QList<QTreeWidgetItem*>items =  w->findItems(taskName, Qt::MatchRecursive | Qt::MatchExactly);
    if(items.isEmpty()) {
        throw TestException(tr("Item %1 not found").arg(taskName));
    }

    return items.first()->text(2);
}

void TaskViewTest::cancelTask(const QString &taskName) {
    QPoint pos = getItemPosition(taskName, taskViewWidgetName);
    moveTo(taskViewWidgetName, pos);
    mouseClickOnItem(taskViewWidgetName, Qt::LeftButton, pos);
    contextMenuOnItem(taskViewWidgetName, pos);
    //sleep(500);
    clickContextMenu("Cancel task");
    
}

QString TaskViewTest::getTaskState(const QString &taskName){
    QTreeWidget * w = static_cast<QTreeWidget*>(findWidgetByName(taskViewWidgetName));
    QList<QTreeWidgetItem*>items =  w->findItems(taskName, Qt::MatchRecursive | Qt::MatchExactly);
    if(items.isEmpty()) {
        throw TestException(tr("Item %1 not found").arg(taskName));
    }

    return items.first()->text(1);

}

const QString TaskViewTest::taskViewWidgetName = "taskViewTree";

}
