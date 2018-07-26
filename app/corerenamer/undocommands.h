/*
CoreBox is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see {http://www.gnu.org/licenses/}. */

#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QDebug>
#include <QUndoCommand>
#include <QStandardItem>
#include <QTableWidgetItem>

enum COMMAND {
    ADD = 0,
    REMOVE = 1,
    REPLACE = 2,
    NUMBER = 3,
    CASE = 4
};


class ARRTextCommand : public QUndoCommand
{
public:
    ARRTextCommand(QStandardItem *item, const QString &addedT, COMMAND c, QUndoCommand *parent = 0)
        : QUndoCommand(parent), m_item(item), m_nText(addedT), m_oText(item->text()) {
        setText(getCommandText(c) + " " + item->text() + " " + m_nText);
    }

    ~ARRTextCommand() {
        if (!m_item)
            delete m_item;
    }

    void undo() override {
        m_item->setText(m_oText);
    }

    void redo() override {
        m_item->setText(m_nText);
    }

private:
    QStandardItem *m_item;
    QString m_nText;
    QString m_oText;

    QString getCommandText(COMMAND c) {
        if (c == ADD) {
            return "ADD";
        } else if (c == REMOVE) {
            return "REMOVE";
        } else if (c == REPLACE) {
            return "REPLACE";
        } else if (c == NUMBER) {
            return "NUMBER";
        } else if (c == CASE) {
            return "CASE";
        }
        return "";
    }

};

#endif // UNDOCOMMANDS_H
