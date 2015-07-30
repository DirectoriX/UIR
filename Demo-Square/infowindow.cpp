#include "infowindow.h"
#include "ui_infowindow.h"
const QString pre="<html><head/><body><p><span style=\" font-size:18pt;\">";
const QString post="</span></p></body></html>";

InfoWindow::InfoWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoWindow)
{
    ui->setupUi(this);
}

InfoWindow::~InfoWindow()
{
    delete ui;
}

void InfoWindow::updateL(qreal a, qreal b)
{
    QString aba=pre+QString::number(a)+post;
    ui->label_a->setText(aba);
    aba=pre+QString::number(b)+post;
    ui->label_b->setText(aba);
}
