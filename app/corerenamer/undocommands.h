#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QDebug>
#include <QUndoCommand>
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
    ARRTextCommand(QTableWidgetItem *item, const QString &addedT, COMMAND c, QUndoCommand *parent = 0)
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
    QTableWidgetItem *m_item;
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



class RemoveTextCommand : public QUndoCommand
{
public:
    RemoveTextCommand(QTableWidgetItem *item, const QString &remedT, QUndoCommand *parent = 0)
        : QUndoCommand(parent), m_item(item), m_nText(remedT), m_oText(item->text()) {
        setText("Remmoved " + item->text() + " " + m_nText);
    }

    ~RemoveTextCommand() {
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
    QTableWidgetItem *m_item;
    QString m_nText;
    QString m_oText;
};

class ReplaceTextCommand : public QUndoCommand
{
public:
    ReplaceTextCommand();
};

class FormatTextCommand : public QUndoCommand
{
public:
    FormatTextCommand();
};

#endif // UNDOCOMMANDS_H
