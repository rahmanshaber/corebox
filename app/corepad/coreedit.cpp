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

#include "coreedit.h"


coreedit::coreedit(QString fPath, QWidget *parent) : QPlainTextEdit(parent)
{
    // It will need in future
    filePath = fPath;

    cHighlighter *highlighter = new cHighlighter(document());
    setStyleSheet("QWidget{background-color: #2E2F30; border: none;}"
                  "QMenu::item{background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);}"
                  "QMenu::item::selected{background-color: rgb(p0, 85, 127);color: rgb(255,255,255);}"
                  "QScrollBar:vertical {background-color: rgba( 0, 0, 0, 72% ); max-width: 8px;border-radius: 4px; padding: 1px; }"
                  "QScrollBar::handle:vertical {background-color: silver; max-width: 6px; min-height: 12px; border-radius: 3px; }"
                  "QScrollBar::sub-page:vertical {background-color: rgba( 0, 0, 0, 72% ); }"
                  "QScrollBar::add-page:vertical {background-color: rgba( 0, 0, 0, 72% ); }"
                  "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { width: 0px; height: 0px; }");

    lineNumberArea = new LineNumberArea(this);
    this->lineNumberArea->setContentsMargins(0, 0, 0, 0);
    //this->lineNumberArea->setMinimumWidth(20);
//---------------------------- Setting the CoreEdit background -----------------------------------------
    QPalette p = palette();
    p.setColor(QPalette::Text, "#B9A388");  //Text color set to white.
    p.setColor(QPalette::Active, QPalette::Base, "#2E2F30");  //Active base color black.
    p.setColor(QPalette::Inactive, QPalette::Base,"#2E2F30"); //Inactive base color black.
    setPalette(p);
//------------------------------------------------------------------------------------------------------
//---------------------------- Connecting to slots and signals -----------------------------------------
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
//------------------------------------------------------------------------------------------------------
    updateLineNumberAreaWidth(0);   //Set the line number area.
    //highlightCurrentLine();         //Highlight the current line.
}

int coreedit::lineNumberAreaWidth()
{
    int digits = 2;
    int max = qMax(1, blockCount());
    while (max >= 100) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
    return space;
}

void coreedit::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
    setViewportMargins(lineNumberAreaWidth() + 2, 0, 0, 0);
}

void coreedit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void coreedit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    // Geometry of the line number area
    lineNumberArea->setGeometry(QRect(cr.left() - 2, cr.top(), lineNumberAreaWidth() + 3, cr.height() + 2));
}

void coreedit::highlightCurrentLine()
{
//    QList<QTextEdit::ExtraSelection> extraSelections;

//    if (!isReadOnly()) {
//        QTextEdit::ExtraSelection selection;

//        QColor lineColor = QColor(Qt::gray).lighter(100);

//        selection.format.setBackground(lineColor);
//        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
//        selection.cursor = textCursor();
//        selection.cursor.clearSelection();
//        extraSelections.append(selection);
//    }

//    setExtraSelections(extraSelections);
}

void coreedit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor("#2E2F30").lighter(121));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen("#ffffff");
            painter.drawText(-1, top, lineNumberArea->width() + 3, fontMetrics().height() + 2, Qt::AlignRight, number + " ");
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
