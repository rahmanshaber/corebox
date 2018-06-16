/*
CoreBox is combination of some common desktop apps.

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

#ifndef COREEDIT_H
#define COREEDIT_H

#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>


class coreedit : public QPlainTextEdit
{
    Q_OBJECT

public:
    coreedit(QWidget *parent = 0);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    QWidget* lineNumberArea_() const {
        return lineNumberArea;
    }

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget *lineNumberArea;
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(coreedit *editor) : QWidget(editor) {
        coreedit_ = editor;
    }

    QSize sizeHint() const override {
        return QSize(coreedit_->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        coreedit_->lineNumberAreaPaintEvent(event);
    }

private:
    coreedit *coreedit_;

};

#endif // COREEDIT_H
