#include "drawfield.h"

#include <QPainter>

#include "info.h"

DrawField::DrawField(QWidget *parent) : QWidget(parent)
{
  rects = QVector<place>();
  realwidth = realheight = maxwidth = maxheight = 1;
}

void DrawField::paintEvent(QPaintEvent *pe)
{
  pe->accept();
  QPainter painter(this);
  // Очистка
  painter.setBrush(Qt::white);
  painter.drawRect(-100, -100, 1111, 1111);
  // Общее занятое поле
  QPen pen = QPen(Qt::yellow);
  painter.setPen(pen);
  painter.setBrush(Qt::yellow);
  painter.drawRect(0, 0, maxwidth * xcoef, maxheight * ycoef);
  // Свободный прямоугольник
  pen.setColor(Qt::cyan);
  painter.setPen(pen);
  painter.setBrush(Qt::cyan);
  painter.drawRect(freex * xcoef + 1, freey * ycoef + 1, (maxwidth - freex) * xcoef, (maxheight - freey) * ycoef);
  // Прямоугольники
  pen.setColor(Qt::gray);
  painter.setPen(pen);
  painter.setBrush(Qt::green);

  foreach (place p, rects)
    {
      painter.drawRect(p.x * xcoef, p.y * ycoef, p.maxx * xcoef, p.maxy * ycoef);
    }
}

void DrawField::clear()
{
  rects.clear();
}

void DrawField::setGeometry(const QRect &rect)
{
  QWidget::setGeometry(rect);
  xcoef = rect.width() / realwidth;
  ycoef = rect.height() / realheight;
}

void DrawField::setGeometry(int x, int y, int w, int h)
{
  QWidget::setGeometry(x, y, w, h);
  xcoef = w / realwidth;
  ycoef = h / realheight;
}

void DrawField::setRealSize(qreal width, qreal height)
{
  realwidth = width;
  realheight = height;
  xcoef = this->width() / realwidth;
  ycoef = this->height() / realheight;
}

void DrawField::setRects(QVector<detail> *details)
{
  rects.clear();

  foreach (detail d, *details)
    {
      rects.append(place{d.x, d.y, d.width, d.height});
    }
}

void DrawField::setBorders(qreal width, qreal height)
{
  maxheight = height;
  maxwidth = width;
}

void DrawField::setFreeSpace(qreal x, qreal y)
{
  freex = x;
  freey = y;
}
