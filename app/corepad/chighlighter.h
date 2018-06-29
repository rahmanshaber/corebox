#ifndef CHIGHLIGHTER_H
#define CHIGHLIGHTER_H

#include <QWidget>
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
