#include "info.h"
#include "ui_info.h"

info::info(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::info)
{
  ui->setupUi(this);
}

info::~info()
{
  delete ui;
}

void info::setFieldSize(qreal width, qreal height)
{
  qreal cx, cy;
  cx = width / 301;
  cy = height / 281;
  qreal c = qMax(cx, cy);
  ui->field->setGeometry(10, 10, width / c, height / c);
  ui->field->setRealSize(width, height);
}

void info::setRects(QVector<detail> *details, qreal w, qreal h, qreal freex, qreal freey)
{
  ui->field->setRects(details);
  ui->field->setBorders(w, h);
  ui->field->setFreeSpace(freex, freey);
  ui->field->repaint();
  ui->detailReport->clear();
  QString format = "(%1,%2) - %3 %4";

  foreach (detail d, *details)
    {
      ui->detailReport->appendPlainText(format.arg(d.x).arg(d.y).arg(d.type + 1).arg(d.flipped));
    }
}
