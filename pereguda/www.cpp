#include "www.h"
#include "ui_www.h"

www::www(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::www)
{
  ui->setupUi(this);
}

www::~www()
{
  delete ui;
}

void www::updateL(double *arr)
{
  ui->textEdit->clear();

  for (int i = 0; i < 30; i++)
    { ui->textEdit->append(QString::number(arr[i]));}
}
