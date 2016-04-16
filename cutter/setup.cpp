#include "setup.h"
#include "ui_setup.h"

QVector<detailType> *dTtypes;
QVector<quint32> *count;

setup::setup(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::setup)
{
  ui->setupUi(this);
}

setup::~setup()
{
  delete ui;
  delete count;
  delete dTtypes;
}

QVector<detailType> *setup::getTypes()
{
  return dTtypes;
}

qreal setup::getWidth()
{
  return ui->w->value();
}

qreal setup::getHeight()
{
  return ui->h->value();
}

QVector<quint32> *setup::getCount()
{
    return count;
}

quint8 setup::getMode()
{
    return ui->mode->currentIndex();
}

void setup::on_pushButton_clicked()
{
  dTtypes = new QVector<detailType>();
  ///TODO Заполнение из файла
  dTtypes->append(detailType{ ui->w1SpinBox->value(), ui->h1SpinBox->value() });
  dTtypes->append(detailType{ ui->w2SpinBox->value(), ui->h2SpinBox->value() });
  dTtypes->append(detailType{ ui->w3SpinBox->value(), ui->h3SpinBox->value() });
  dTtypes->append(detailType{ ui->w4SpinBox->value(), ui->h4SpinBox->value() });
  dTtypes->append(detailType{ ui->w5SpinBox->value(), ui->h5SpinBox->value() });
  dTtypes->append(detailType{ ui->w6SpinBox->value(), ui->h6SpinBox->value() });
  dTtypes->append(detailType{ ui->w7SpinBox->value(), ui->h7SpinBox->value() });
  count = new QVector<quint32>();
  count->append(ui->spinBox1->value());
  count->append(ui->spinBox2->value());
  count->append(ui->spinBox3->value());
  count->append(ui->spinBox4->value());
  count->append(ui->spinBox5->value());
  count->append(ui->spinBox6->value());
  count->append(ui->spinBox7->value());
  close();
}

void setup::on_setup_rejected()
{
    on_pushButton_clicked();
}
