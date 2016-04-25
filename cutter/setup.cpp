#include "setup.h"
#include "ui_setup.h"
#include <QtMath>
#include <QFileDialog>

QVector<detailType> *dTtypes;
QVector<quint32> *count;
qreal sumSquare = 0;

setup::setup(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::setup)
{
  ui->setupUi(this);
  ui->tabWidget->setCurrentIndex(2);
  ui->tabWidget->setCurrentIndex(0);
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

qreal setup::getSquare()
{
  return sumSquare;
}

void setup::on_pushButton_clicked()
{
  dTtypes = new QVector<detailType>();
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
  sumSquare = ui->w1SpinBox->value() * ui->h1SpinBox->value() * ui->spinBox1->value();
  sumSquare += ui->w2SpinBox->value() * ui->h2SpinBox->value() * ui->spinBox2->value();
  sumSquare += ui->w3SpinBox->value() * ui->h3SpinBox->value() * ui->spinBox3->value();
  sumSquare += ui->w4SpinBox->value() * ui->h4SpinBox->value() * ui->spinBox4->value();
  sumSquare += ui->w5SpinBox->value() * ui->h5SpinBox->value() * ui->spinBox5->value();
  sumSquare += ui->w6SpinBox->value() * ui->h6SpinBox->value() * ui->spinBox6->value();
  sumSquare += ui->w7SpinBox->value() * ui->h7SpinBox->value() * ui->spinBox7->value();
  // Сложные фигуры
  // Эллипс
  dTtypes->append(detailType{ ui->ellASpinBox->value(), ui->ellBSpinBox->value() });
  count->append(ui->ellspinBox->value());
  sumSquare += ui->ellASpinBox->value() * ui->ellBSpinBox->value() * M_PI_4 * ui->ellspinBox->value();
  // Треугольник
  dTtypes->append(detailType{ ui->treASpinBox->value(), ui->trehSpinBox->value() });
  count->append(ui->trespinBox->value());
  sumSquare += ui->treASpinBox->value() * ui->trehSpinBox->value() / 2 * ui->trespinBox->value();
  // Параллелограмм
  qreal addwidth = ui->parhSpinBox->value() / qTan(qDegreesToRadians(ui->paralphaSpinBox->value()));
  dTtypes->append(detailType{ ui->parASpinBox->value() + addwidth, ui->parhSpinBox->value() });
  count->append(ui->parspinBox->value());
  sumSquare += ui->parASpinBox->value() * ui->parhSpinBox->value() * ui->parspinBox->value();
  // Трапеция
  dTtypes->append(detailType{ ui->traASpinBox->value(), ui->trahSpinBox->value() });
  count->append(ui->traspinBox->value());
  sumSquare += (ui->traASpinBox->value() + ui->traBSpinBox->value()) / 2 * ui->trahSpinBox->value() * ui->traspinBox->value();
  close();
}

void setup::on_setup_rejected()
{
  if (sumSquare == 0)
    { on_pushButton_clicked(); }
}

void setup::on_tabWidget_currentChanged(int index)
{
  ui->SVG->load(":/pics/" + ui->tabWidget->tabWhatsThis(index));
}

void setup::on_fileButton_clicked()
{
  QString fileName;

  if ((fileName = QFileDialog::getOpenFileName(this, "Открыть", NULL, "Таблица(*.csv)")) != "")
    {
      QString data;
      QFile importedCSV(fileName);
      QStringList rowOfData;

      if (importedCSV.open(QFile::ReadOnly))
        {
          data = importedCSV.readAll();
          rowOfData = data.split(QRegExp("\r?\n"));
          importedCSV.close();
        }

      rowOfData.removeFirst();
      rowOfData.removeLast();
      quint32 c;
      qreal A, B, h;
      dTtypes = new QVector<detailType>();
      count = new QVector<quint32>();
      sumSquare = 0;

      foreach (QString currentText, rowOfData)
        {
          QStringList s = currentText.split(QRegExp("[;, \t]"));
          c = s[1].toInt();
          A = s[2].toDouble();
          B = s[3].toDouble();
          count->append(c);

          switch (currentText[0].toLatin1())
            {
              case 'r': // Прямоугольник
              {
                dTtypes->append(detailType{A, B});
                sumSquare += A * B * c;
                break;
              }

              case 'e': // Эллипс
              {
                dTtypes->append(detailType{A, B});
                sumSquare += A * B * M_PI_4 * c;
                break;
              }

              case 't': // Треугольник
              {
                dTtypes->append(detailType{A, B});
                sumSquare += A * B / 2 * c;
                break;
              }

              case 'p': // Параллелограмм
              {
                h = s[4].toDouble();
                qreal addwidth = B / qTan(qDegreesToRadians(h));
                dTtypes->append(detailType{A + addwidth, B});
                sumSquare += A * B * c;
                break;
              }

              case 'z': // Трапеция
              {
                h = s[4].toDouble();
                dTtypes->append(detailType{A, B});
                sumSquare += (A + B) / 2 * h * c;
                break;
              }
            }
        }

      close();
    }
}
