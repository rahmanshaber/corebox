#ifndef CORERENAMER_H
#define CORERENAMER_H

#include <QWidget>
#include <QUndoStack>
#include <QTableWidgetItem>


namespace Ui {
class corerenamer;
}

enum tPosition {
    FROMLEFT,
    FROMRIGHT
};

class corerenamer : public QWidget
{
    Q_OBJECT

public:
    explicit corerenamer(QWidget *parent = 0);
    ~corerenamer();

    void addFiles(QStringList list);

private slots:
    void on_bAddFiles_clicked();
    void on_addTextbtn_clicked();
    void on_addTextAllbtn_clicked();
    void on_removeTextTextbtn_clicked();
    void on_removeTextAllbtn_clicked();
    void on_replaceTextbtn_clicked();
    void on_replaceTextAllbtn_clicked();
    void on_upperCase_clicked();
    void on_capitalEach_clicked();
    void on_lowerCase_clicked();
    void on_formatc_activated(const QString &arg1);
    void on_browseSave_clicked();
    void on_saveRenamed_clicked();
    void on_bUndo_clicked();
    void on_bRedo_clicked();
    void on_rename_clicked();
    void on_bRefreshList_clicked();

private:
    Ui::corerenamer *ui;

    QUndoStack *uStack;
    void createActions();
    QIcon fileIcon(const QString &filePath);

    /*!
     * \brief addText Add text using position.
     * \param tpos From which side.
     * \param pos From where to start.
     * \param newText Text which is going to be added.
     * \param selcText Text at which needs add.
     * \return Return to the added text.
     */
    QString addText(tPosition tpos, int pos, const QString &newText, const QString &selcText);

    /*!
     * \brief remText Remove text by position.
     * \param tpos From which side.
     * \param pos From where to start.
     * \param count How many to remove.
     * \param selcText Text at which needs remove.
     * \return Return to the removed text.
     */
    QString remText(tPosition tpos, int pos, int count, const QString &selcText);

    /*!
     * \brief repText Replace text by a text.
     * \param sText Text which is going to be replaced.
     * \param rText Text which is going to be added.
     * \param selcText Text at which needs replace.
     * \param s Case sensitivity for replaced text (Default is CaseInsensitive).
     * \return Return to the replaced text.
     */
    QString repText(const QString &sText, const QString &rText, const QString &selcText, Qt::CaseSensitivity s = Qt::CaseInsensitive);

    /*!
     * \brief casitivity Text casing.
     * \param i 0 = UPPERCASE, 1 = Capitalize Each, 2 = lower
     */
    void casitivity(int i);

    QString CapitalizeEachWord(const QString &str);
    QString getNew(QTableWidgetItem *text);
    QString getOld(QTableWidgetItem *text);

    bool setAddText();
    bool setRemText();
    bool setRepText();
};

#endif // CORERENAMER_H
