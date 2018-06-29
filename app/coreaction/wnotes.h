#ifndef WNOTES_H
#define WNOTES_H

#include <QWidget>

namespace Ui {
class wNotes;
}

class wNotes : public QWidget
{
    Q_OBJECT

public:
    explicit wNotes(QWidget *parent = 0);
    ~wNotes();

private:
    Ui::wNotes *ui;
    void collectNotes();
    bool saveNotes();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // WNOTES_H
