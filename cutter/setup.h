#ifndef SETUP_H
#define SETUP_H

#include <QDialog>

#include "descr.h"

namespace Ui {
  class setup;
}

class setup : public QDialog {
    Q_OBJECT

  public:
    explicit setup(QWidget *parent = 0);
    ~setup();
    QVector<detailType> *getTypes();
    qreal getWidth();
    qreal getHeight();
    QVector<quint32> *getCount();
    quint8 getMode();
    qreal getSquare();

  private slots:
    void on_pushButton_clicked();
    void on_setup_rejected();
    void on_tabWidget_currentChanged(int index);
    void on_fileButton_clicked();

  private:
    Ui::setup *ui;
};

#endif // SETUP_H
