#include "prg.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

const double STERAK[] = {0,
                         12.7062, 4.3027, 3.1824, 2.7764, 2.5706, 2.4469, 2.3646, 2.3060, 2.2622, 2.2281,
                         2.2010, 2.1788, 2.1604, 2.1448, 2.1314, 2.1199, 2.1098, 2.1009, 2.0930, 2.0860,
                         2.0796, 2.0739, 2.0687, 2.0639, 2.0595, 2.0555, 2.0518, 2.0484, 2.0452, 2.0423,
                         2.0395, 2.0369, 2.0345, 2.0322, 2.0301, 2.0281, 2.0262, 2.0244, 2.0227, 2.0211,
                         2.0195, 2.0181, 2.0167, 2.0154, 2.0141, 2.0129, 2.0117, 2.0106, 2.0096, 2.0086,
                        };

QVector<double> VAmin, VSA;
QVector<int> Vt;
int totalcount;

www *wnd = NULL;

prg::prg()
{
  if (wnd != NULL)
    {
      realParams = new qreal[30];

      for (int i = 0; i < 30; i++)
        { initDoubleParam(i);}
    }
}

prg::~prg()
{
  delete realParams;
  disconnect(this);
}


prg *prg::create()
{
  return new prg();
}

void prg::initDoubleParam(quint32 number)
{
  realParams[number] = RNG::getint(0, totalcount);
}

void prg::calculate()
{
  QVector<double> a;

  for (int i = 0; i < 30; i++)
    {
      a += realParams[i];
    }

  qSort(a);

  for (int i = 0; i < 29; i++)
    {
      if (a[i] == a[i + 1])
        {
          fitness = -999999999;
          return;
        }
    }

  QVector<int> time = QVector<int>();
  QVector<double> AminM, SAM;
  AminM = QVector<double>();
  SAM = QVector<double>();
  //////////////////////////////////
  QVector<int> timeAll = QVector<int>();
  QVector<double> AminMAll, SAMAll;
  AminMAll = QVector<double>();
  SAMAll = QVector<double>();

  for (int i = 0; i < 30; i++)
    {
      timeAll += Vt[realParams[i]];
      AminMAll += VAmin[realParams[i]];
      SAMAll += VSA[realParams[i]];
    }

  for (int i = 0; i < timeAll.count(); i++)
    {
      time += timeAll[i];
      AminM += AminMAll[i];
      SAM += SAMAll[i];
    }

  int n = time.count();

  for (int i = 0; i < n - 1; i++)
    {
      bool swapped = false;

      for (int j = 0; j < n - i - 1; j++)
        {
          if (time[j] > time[j + 1])
            {
              qSwap(time[j], time[j + 1]);
              qSwap(AminM[j], AminM[j + 1]);
              qSwap(SAM[j], SAM[j + 1]);
              swapped = true;
            }
        }

      if (!swapped)
        { break; }
    }

  for (int i = 0; i < 29; i++)
    {
      if (time[i] == time[i + 1])
        {
          fitness = -999999999;
          return;
        }
    }

  int count;
  double AminSum, SASum, TimeSum, TimeSum2, TimeAminSum, TimeSASum, AminSum2, SASum2;
  count = 30;
  AminSum = SASum = AminSum2 = SASum2 = TimeSum = TimeSum2 = TimeAminSum = TimeSASum = 0;

  for (int i = 0; i < count; i++)
    {
      AminSum += 1.0 * AminM[i];
      SASum += 1.0 * SAM[i];
      AminSum2 += 1.0 * AminM[i] * AminM[i];
      SASum2 += 1.0 * SAM[i] * SAM[i];
      TimeSum += 1.0 * time[i];
      TimeSum2 += 1.0 * time[i] * time[i];
      TimeAminSum += 1.0 * time[i] * AminM[i];
      TimeSASum += 1.0 * time[i] * SAM[i];
    }

  double b0Amin, b0SA, b1Amin, b1SA, rAmin, rSA;
  b1Amin = (TimeAminSum / count - TimeSum / count * AminSum / count) /
           (TimeSum2 / count - TimeSum / count * TimeSum / count);
  b1SA = (TimeSASum / count - TimeSum / count * SASum / count) /
         (TimeSum2 / count - TimeSum / count * TimeSum / count);
  b0Amin = AminSum / count - b1Amin * TimeSum / count;
  b0SA = SASum / count - b1SA * TimeSum / count;
  double AbaSum, cabasum, esum, e2Sum, S;
  //Интервал Amin
  AbaSum = cabasum = esum = e2Sum = 0;
  int inside = 0;

  for (int i = 0; i < count; i++)
    {
      AbaSum += qPow((time[i] - TimeSum / count), 2);
      cabasum += b0Amin + time[i] * b1Amin;
      esum += (b0Amin + time[i] * b1Amin) - AminM[i];
      e2Sum += qPow((b0Amin + time[i] * b1Amin) - AminM[i], 2);
    }

  for (int i = 0; i < count; i++)
    {
      S = qSqrt((e2Sum / (count - 2)) *
                (1.0 / count + qPow((time[i] - TimeSum / count), 2) / AbaSum));
      S *= STERAK[count - 2];

      if ((AminM[i] > b0Amin + time[i] * b1Amin - S) && (AminM[i] < b0Amin + time[i] * b1Amin + S))
        { inside++; }
    }

  //Интервал SA
  AbaSum = cabasum = esum = e2Sum = 0;

  for (int i = 0; i < count; i++)
    {
      AbaSum += qPow((time[i] - TimeSum / count), 2);
      cabasum += b0SA + time[i] * b1SA;
      esum += (b0SA + time[i] * b1SA) - SAM[i];
      e2Sum += qPow((b0SA + time[i] * b1SA) - SAM[i], 2);
    }

  for (int i = 0; i < count; i++)
    {
      S = qSqrt((e2Sum / (count - 2)) *
                (1.0 / count + qPow((time[i] - TimeSum / count), 2) / AbaSum));
      S *= STERAK[count - 2];

      if ((SAM[i] > b0SA + time[i] * b1SA - S) && (SAM[i] < b0SA + time[i] * b1SA + S))
        { inside++; }
    }

  //Остальное
  rSA = (count * TimeSASum - TimeSum * SASum) /
        (qSqrt(count * SASum2 - SASum * SASum) * qSqrt(count * TimeSum2 - TimeSum * TimeSum));
  rAmin = (count * TimeAminSum - TimeSum * AminSum) /
          (qSqrt(count * AminSum2 - AminSum * AminSum) * qSqrt(count * TimeSum2 - TimeSum * TimeSum));
  double rrr = 0;

  for (int i = 0; i < 29; i++)
    {
      rrr += qPow(time[i] - time[i + 1], 2) + qPow(AminM[i] - AminM[i + 1], 2) + qPow(SAM[i] - SAM[i + 1], 2);
    }

  rrr /= 29;
  double zzz = 0;

  for (int i = 0; i < 29; i++)
    {
      zzz += qSqrt(qAbs(qPow(time[i] - time[i + 1], 2) - rrr));
    }

  fitness = (qAbs(rSA) * 100000 + qAbs(rAmin) * 100000) * inside - zzz;
}

void prg::prepare()
{
  wnd = new www();
  QString fileName;

  if ((fileName = QFileDialog::getOpenFileName(NULL, "Открыть", NULL, "Таблица(*.csv)")) != "")
    {
      QString data;
      QFile importedCSV(fileName);
      QStringList rowOfData, rowData;
      data.clear();
      rowOfData.clear();
      rowData.clear();

      if (importedCSV.open(QFile::ReadOnly))
        {
          data = importedCSV.readAll();
          data.remove('"');
          rowOfData = data.split("\r\n");
          importedCSV.close();
        }
      else
        {
          return;
        }

      int count = rowOfData.size() - 2;

      for (int i = 1; i < count + 1; i++)
        {
          rowData = rowOfData[i].split(QRegExp("[;, \t]"));

          if (rowData.count() != 4)
            {
              QMessageBox(QMessageBox::Critical, "Ошибка", "Неверный формат файла").exec();
              return;
            }

          Vt.append(rowData[1].toInt());
          VAmin.append(rowData[2].toDouble());
          VSA.append(rowData[3].toDouble());
        }
    }

  totalcount = Vt.count();
}

void prg::clean()
{
  delete wnd;
  VAmin.clear();
  Vt.clear();
  VSA.clear();
}

quint32 prg::paramCount()
{
  return 30;
}

QString prg::name()
{
  return "Pereguda!";
}

bool prg::threadable()
{
  return true;
}

QDialog *prg::getInfoWindow()
{
  return wnd;
}

void prg::updateInfoWindow()
{
  connect(this, SIGNAL(updateInfo(double *)), wnd, SLOT(updateL(double *)));
  emit updateInfo(realParams);
  QFile output("result.csv");

  if (output.open(QFile::WriteOnly))
    {
      output.write("NUM,HOURS,AMIN,ST\r\n");

      for (int i = 0; i < 30; i++)
        { output.write(QByteArray(QString("1," + QString::number(Vt[realParams[i]]) + "," + QString::number(VAmin[realParams[i]]) + "," + QString::number(VSA[realParams[i]]) + "\r\n").toLatin1())); }

      output.close();
    }
}

bool prg::isSimpleMutation()
{
  return true;
}

void prg::mutate(bool onlyOnce, qreal probability)
{
  return;
}

bool prg::isSimpleCrossover()
{
  return true;
}

void prg::crossover(ICreature *p1, ICreature *p2)
{
  return;
}
