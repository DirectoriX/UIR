#ifndef DRAWFIELD_H
#define DRAWFIELD_H

#include <QPaintEvent>
#include <QWidget>
#include "descr.h"

class DrawField : public QWidget {
    Q_OBJECT
  public:
    explicit DrawField(QWidget *parent = 0);
    void paintEvent(QPaintEvent *pe);
    void clear();
    void setGeometry(const QRect &rect);
    void setGeometry(int x, int y, int w, int h);
    void setRealSize(qreal width, qreal height);
    void setRects(QVector<detail> *details);
    void setBorders(qreal width, qreal height);
    void setFreeSpace(qreal x, qreal y);

  private:
    qreal realwidth, realheight;
    qreal xcoef, ycoef;
    qreal maxwidth, maxheight;
    qreal freex, freey;
    QVector<place> rects;
};

#endif // DRAWFIELD_H
