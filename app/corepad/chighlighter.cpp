#include "chighlighter.h"

#include <QMessageBox>

cHighlighter::cHighlighter(QTextDocument *textDocument) : QSyntaxHighlighter(textDocument), m_text(textDocument)
{
    /************************************************************************
    * We will only create one Rule struct on the stack and reuse this same  *
    * struct for every rule we add to our QStringList<Rule> rulesList       *
    ************************************************************************/
    Rule rule;
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(Qt::darkBlue);

    /****************************************************************************
     * Here we begin the construction of our QStringList<Rule> rulesList.       *
     * We start by putting in Regex's for all the C++ keyworks. These will all  *
     * share the same formatting.                                               *
     ***************************************************************************/
    QStringList keywordList;
    keywordList  << "\\bchar\\b"     << "\\bclass\\b"     << "\\bconst\\b"
                 << "\\bdouble\\b"   << "\\benum\\b"      << "\\bexplicit\\b"
                 << "\\bfriend\\b"   << "\\binline\\b"    << "\\bint\\b"
                 << "\\blong\\b"     << "\\bnamespace\\b" << "\\boperator\\b"
                 << "\\bprivate\\b"  << "\\bprotected\\b" << "\\bpublic\\b"
                 << "\\bshort\\b"    << "\\bsignals\\b"   << "\\bsigned\\b"
                 << "\\bslots\\b"    << "\\bstatic\\b"    << "\\bstruct\\b"
                 << "\\btemplate\\b" << "\\btypedef\\b"   << "\\btypename\\b"
                 << "\\bunion\\b"    << "\\bunsigned\\b"  << "\\bvirtual\\b"
                 << "\\bvoid\\b"     << "\\bvolatile\\b";


    /*******************************************************************************
     * Iterate through the list we just constructed and add rules for each pattern *
     ******************************************************************************/
    foreach(const QString& pattern, keywordList) {
        rule.regEx = QRegExp(pattern);
        rulesList.append(rule);
    }



    /* **************************************************************************
     * Now we can move on to other C++ Regex's and append them to the rulesList *
     * *************************************************************************/
    // C++ Class Rule
    rule.regEx = QRegExp( "\\b[A-Z][_A-Za-z0-9]*\\b" );
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(Qt::darkRed);
    rulesList.append(rule);

    // C++ Quotes Rule
    rule.regEx = QRegExp( "\\b\".*\"\\b" );
    rule.format.setForeground(Qt::blue);
    rulesList.append(rule);
    rule.regEx = QRegExp( "\\b\'.*\'\\b" );
    rulesList.append( rule );

    // C++ Functions Rule
    rule.regEx = QRegExp( "\\b[A-Za-z0-9_]+(?=\\()" );
    rule.format.setForeground(QBrush(QColor(175,207,219)));
    rulesList.append(rule);

    // C++ Comments Rule
    rule.regEx = QRegExp( "//[^\n]*" );
    rule.format.setFontWeight(QFont::Normal);
    rule.format.setForeground(Qt::lightGray);;
    rulesList.append(rule);
}

/************************************************************************************
*  The highlightBlock method is called for every line of the document every time it *
*  is updated. We have to traverse the line for every rule in our rulesList and     *
*  check for a match. Where there is a match, we update the text format of the      *
*  match to the format specified in the rule.format field.                          *
************************************************************************************/
void cHighlighter::highlightBlock(const QString& text)
{
    if(rulesList.isEmpty()) {
        QMessageBox::information( nullptr, QString("Uh oh"), QString("The rulesList is empty"), QMessageBox::Ok );
        return;
    }
    foreach(const Rule rule, rulesList) {
        int startIndex = rule.regEx.indexIn(text);                       // Returns 0 or positive number if there is a match
        int length;
        while( startIndex >= 0 ) {
            length = rule.regEx.matchedLength();
            setFormat(startIndex, length, rule.format);
            startIndex = rule.regEx.indexIn(text, startIndex+length);    // Move on to the next match AFTER the first match. If another match isn't found, startIndex will be < 0 and loop will exit.
        }
    }
}


/******************************************************************************
 * Very simply, this will delete itself. Qt takes care of deleting everything *
 * associated with this object.                                               *
 *****************************************************************************/
//cHighlighter::~cHighlighter()
//{
//    delete this;
//}
