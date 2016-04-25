#ifndef INFO_H
#define INFO_H

#include <QDialog>

#include "drawfield.h"

namespace Ui {
  class info;
}

class info : public QDialog {
    Q_OBJECT

  public:
    explicit info(QWidget *parent = 0);
    ~info();

  public slots:
    void setFieldSize(qreal width, qreal height);
    void setRects(QVector<detail> *details, qreal w, qreal h, qreal freex, qreal freey);

  private:
    Ui::info *ui;
};

#endif // INFO_H
