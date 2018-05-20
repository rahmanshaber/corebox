#ifndef COREPDF_H
#define COREPDF_H

#include <QWidget>
#include <QPdfWidget>
#include <QCloseEvent>


class corepdf : public QWidget
{
    Q_OBJECT

public:
    corepdf(QWidget *parent = 0);
    ~corepdf();
    void openPdfFile(const QString path);

    void eclose();
    QString workFilePath;

protected:
    void closeEvent(QCloseEvent *event);
private:
    QPdfWidget *PdfWidget;
};

#endif // COREPDF_H
