#include "wcalender.h"
#include "ui_wcalender.h"

wCalender::wCalender(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wCalender)
{
    ui->setupUi(this);
}

wCalender::~wCalender()
{
    delete ui;
}
