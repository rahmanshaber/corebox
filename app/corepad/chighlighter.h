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

#ifndef CHIGHLIGHTER_H
#define CHIGHLIGHTER_H

#include <QWidget>
#include <QMessageBox>
#include <QSyntaxHighlighter>


class cHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    struct Rule {
        QRegExp regEx;
        QTextCharFormat format;
    };
public:
    explicit cHighlighter(QTextDocument *textDocument = 0);
    //~cHighlighter();

    void highlightBlock(const QString& text);

private:
    Rule rule;
    QVector<Rule> rulesList;
    QTextDocument *m_text;

};

#endif // CHIGHLIGHTER_H
