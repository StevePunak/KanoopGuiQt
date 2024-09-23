/******************************************************************************************
**
** guisettings.cpp
**
** Copyright (C) Tesseract Engineering, Inc - All Rights Reserved
**
** This source code is protected under international copyright law.  All rights
** reserved and protected by the copyright holders.
**
** This file is confidential and only available to authorized individuals with the
** permission of the copyright holders.  If you encounter this file and do not have
** permission, please contact the copyright holders and delete this file.
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#include "abstracttablemodel.h"
#include "guisettings.h"
#include "headerstate.h"
#include "treeviewbase.h"
#include <QCoreApplication>
#include <QHeaderView>
#include <QMutex>
#include <QSplitter>
#include <QUuid>
#include <Kanoop/log.h>

const QString GuiSettings::KEY_APP                          = "app";
const QString GuiSettings::KEY_FONT_SIZE                    = "font_size";
const QString GuiSettings::KEY_HEADER_STATE_HORIZ           = "header_state_h";
const QString GuiSettings::KEY_HEADER_STATE_VERT            = "header_state_v";
const QString GuiSettings::KEY_LAST_DIRECTORY               = "last_dir";
const QString GuiSettings::KEY_LAST_WIDGET_POS              = "widget_pos";
const QString GuiSettings::KEY_LAST_WIDGET_SIZE             = "widget_size";
const QString GuiSettings::KEY_MODEL_HEADER_STATE_HORIZ     = "model_header_state_h";
const QString GuiSettings::KEY_MODEL_HEADER_STATE_VERT      = "model_header_state_v";
const QString GuiSettings::KEY_SPLITTER_STATE_HORIZ         = "splitter_state_h";
const QString GuiSettings::KEY_SPLITTER_STATE_VERT          = "splitter_state_v";
const QString GuiSettings::KEY_TREEVIEW_STATE               = "treeview_state";

GuiSettings* GuiSettings::_globalInstance                   = nullptr;

GuiSettings::GuiSettings() :
    _settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::instance()->organizationName(), QCoreApplication::instance()->applicationName()),
    _maxRecentFiles(10)
{
    GuiSettings::ensureValidDefaults();
}

void GuiSettings::saveLastSplitterState(QSplitter *splitter)
{
    if(splitter->orientation() == Qt::Vertical) {
        _settings.setValue(makeCompoundObjectKey(KEY_SPLITTER_STATE_VERT, splitter), splitter->saveState());
    }
    else {
        _settings.setValue(makeCompoundObjectKey(KEY_SPLITTER_STATE_HORIZ, splitter), splitter->saveState());
    }
}

void GuiSettings::restoreLastSplitterState(QSplitter *splitter)
{
    QByteArray savedState = splitter->saveState();
    QByteArray splitterState;
    if(splitter->orientation() == Qt::Vertical) {
        splitterState = _settings.value(makeCompoundObjectKey(KEY_SPLITTER_STATE_VERT, splitter)).toByteArray();
    }
    else {
        splitterState = _settings.value(makeCompoundObjectKey(KEY_SPLITTER_STATE_HORIZ, splitter)).toByteArray();
    }
    if(splitterState.isEmpty() == false) {
        // if the count has changed, we should not restore the state
        int tempCount = splitter->count();
        splitter->restoreState(splitterState);
        if(tempCount != splitter->count()) {
            Log::logText(LVL_DEBUG, "Splitter count changed... will not restore it");
            splitter->restoreState(savedState);
        }
    }
}

void GuiSettings::saveLastHeaderState(QHeaderView *header)
{
    if(header->orientation() == Qt::Vertical) {
        _settings.setValue(makeKey(KEY_HEADER_STATE_VERT, makeObjectKey(header->parent())), header->saveState());
    }
    else {
        QString key = makeKey(KEY_HEADER_STATE_HORIZ, makeObjectKey(header->parent()));
        QByteArray headerState = header->saveState();
        _settings.setValue(key, headerState);
    }
}

void GuiSettings::restoreLastHeaderState(QHeaderView *header)
{
    QByteArray headerState;
    if(header->orientation() == Qt::Vertical) {
        headerState = _settings.value(makeKey(KEY_HEADER_STATE_VERT, makeObjectKey(header->parent()))).toByteArray();
    }
    else {
        QString key = makeKey(KEY_HEADER_STATE_HORIZ, makeObjectKey(header->parent()));
        headerState = _settings.value(key).toByteArray();
    }
    if(headerState.isEmpty() == false) {
        // Save the state. If the count of items has changed, we can't restore it
        int headerCount = header->count();
        QByteArray savedState = header->saveState();
        header->restoreState(headerState);
        if(header->count() != headerCount) {
            Log::logText(LVL_DEBUG, "Header count has changed... we will not restore it");
            header->restoreState(savedState);
        }
    }
}

void GuiSettings::saveLastHeaderState(QHeaderView *header, AbstractItemModel *model)
{
    TableHeader::List headers = model->columnHeaders();
    HeaderState headerState;
    for(int section = 0;section < headers.count();section++) {
        TableHeader tableHeader = headers.at(section);
        headerState.addSection(section, tableHeader.text(), header->sectionSize(section));
    }
    QByteArray jsonHeaderState = headerState.serializeToJson();
    if(header->orientation() == Qt::Vertical) {
        _settings.setValue(makeKey(KEY_MODEL_HEADER_STATE_VERT, makeObjectKey(header->parent())), jsonHeaderState);
    }
    else {
        _settings.setValue(makeKey(KEY_MODEL_HEADER_STATE_HORIZ, makeObjectKey(header->parent())), jsonHeaderState);
    }
}

void GuiSettings::restoreLastHeaderState(QHeaderView *header, AbstractItemModel *model)
{
    HeaderState headerState;
    QByteArray jsonState;
    QString key = header->orientation() == Qt::Vertical
                      ? makeKey(KEY_MODEL_HEADER_STATE_VERT, makeObjectKey(header->parent()))
                      : makeKey(KEY_MODEL_HEADER_STATE_HORIZ, makeObjectKey(header->parent()));
    jsonState = _settings.value(key).toByteArray();
    headerState.deserializeFromJson(jsonState);

    TableHeader::List headers = model->columnHeaders();
    for(int section = 0;section < headers.count();section++) {
        TableHeader tableHeader = headers.at(section);
        QString sectionText = tableHeader.text();
        HeaderState::SectionState sectionState = headerState.getSection(section);
        if(sectionState.isValid() && sectionState.text() == sectionText) {
            header->resizeSection(section, sectionState.size());
        }
    }
}

void GuiSettings::saveTreeViewState(TreeViewBase *treeView)
{
    QByteArray state = treeView->saveState();
    _settings.setValue(makeKey(KEY_TREEVIEW_STATE, treeView->objectName()), state);
}

void GuiSettings::restoreTreeViewState(TreeViewBase *treeView)
{
    QByteArray state = _settings.value(makeKey(KEY_TREEVIEW_STATE, treeView->objectName())).toByteArray();
    treeView->restoreState(state);
}

GuiSettings *GuiSettings::globalInstance()
{
    QMutex _lock;
    _lock.lock();
    if(_globalInstance == nullptr) {
        _globalInstance = new GuiSettings;
    }
    _lock.unlock();
    return _globalInstance;
}

QString GuiSettings::makeObjectKey(const QObject *object)
{
    QString result = object->objectName();
    if(object->parent() != nullptr) {
        result.prepend(object->parent()->objectName());
    }
    return result;
}

QString GuiSettings::makeFileTypeKey(const QString &key, const QString &extension)
{
    QString fixed = extension;
    fixed.removeIf([extension](const QChar& it) { return it.isPunct() || it.isSpace(); });
    return QString("%1/%2").arg(key, fixed);
}

QString GuiSettings::makeCompoundObjectKey(const QString &key, const QObject *object)
{
    QString result = makeKey(key, makeObjectKey(object));
    return result;
}

void GuiSettings::ensureValidDefaults()
{
    if(fontSize() == 0) {
        setFontSize(QFont().pointSize());
    }
}

QStringList GuiSettings::uuidListToStringList(const QList<QUuid> &uuids)
{
    QStringList result;
    for(const QUuid& uuid : uuids) {
        result.append(uuid.toString(QUuid::WithoutBraces));
    }
    return result;
}

QList<QUuid> GuiSettings::stringListToUuidList(const QStringList &values)
{
    QList<QUuid> result;
    for(const QString& value : values) {
        result.append(QUuid::fromString(value));
    }
    return result;
}
