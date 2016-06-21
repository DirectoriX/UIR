#include <QtMath>

#include "cutter.h"
#include "info.h"
#include "descr.h"
#include "setup.h"
#include "../rng.h"

info *w = NULL;
QVector<detailType> types; // Типы деталей
QVector<qint32> detailSet; // Список деталей после настройки пользователем
qint8 mode = 3;
bool hasfree = true; // Есть ли свободный прямоугольник в углу
quint32 typecount, detailCount;
qreal maxwidth, maxheight;
qreal square;

// Сортировка
bool cmpRange(place a, place b)
{
  switch (mode) // Способ укладки деталей на листе
    {
      case 0:
        return (a.y != b.y) ? a.y < b.y : a.x < b.x; // Строками

      case 1:
        return (a.x != b.x) ? a.x < b.x : a.y < b.y; // Столбцами

      case 2:
        return (qMin(a.x, a.y) != qMin(b.x, b.y)) ? qMin(a.x, a.y) < qMin(b.x, b.y) : ((a.y != b.y) ? a.y < b.y : a.x < b.x); // Строками и столбцами

      case 3:
        return (qMax(a.x, a.y) != qMax(b.x, b.y)) ? qMax(a.x, a.y) < qMax(b.x, b.y) : ((a.y != b.y) ? a.y < b.y : a.x < b.x); // Квадратом
    }

  return false;
}

cutter::cutter()
{
  if (!w) // Коренная особь
    { return; }

  details = QVector<qint32>(detailSet);
  quint32 m = details.count();
  quint32 i;

  // Перемешиваем порядок деталей
  while (m)
    {
      i = qFloor(RNG::getreal() * m--);
      qSwap(details[m], details[i]);
    }

  // Случайным образом поворачиваем часть из них
  for (i = 0; i < detailCount; i++)
    {
      details[i] *= (RNG::getreal() > 0.5) ? 1 : -1;
    }

  // Единственное место, куда можно разместить деталь
  places.append(place{ 0, 0, maxwidth, maxheight });
  totalwidth = totalheight = freex = freey = 0;
}

cutter::~cutter()
{
  disconnect(this);
}

quint32 cutter::paramCount() {return 0;}

QString cutter::name() {return "Раскрой";}

bool cutter::threadable() {return true;}

bool cutter::isSimpleMutation() {return false;}

void cutter::mutate(bool onlyOnce, qreal probability)
{
  if (onlyOnce)
    {
      if (RNG::getreal() < probability)
        {
          if (RNG::getreal() < 0.5) // Или поворачиваем деталь...
            { details[RNG::getint(0, detailCount)] *= -1; }
          else // ...или меняем две детали местами
            {
              qint32 c1 = RNG::getint(0, detailCount);
              qint32 c2 = RNG::getint(0, detailCount, c1);
              qSwap(c1, c2);
            }
        }
    }
  else
    {
      for (qint32 i = 0; i < detailCount ; i++)
        {
          if (RNG::getreal() < probability)
            {
              if (RNG::getreal() < 0.5) // Или поворачиваем деталь...
                { details[i] *= -1; }
              else // ...или меняем две детали местами
                {
                  qint32 c2 = RNG::getint(0, detailCount, i);
                  qSwap(i, c2);
                }
            }
        }
    }
}

bool cutter::isSimpleCrossover() {return false;}

void cutter::crossover(ICreature *p1, ICreature *p2)
{
  cutter *pT1, *pT2;
  pT1 = (cutter *)p1;
  pT2 = (cutter *)p2;
  details = QVector<qint32>(pT1->details);
  QVector<qint32> p2d = QVector<qint32>(pT2->details);
  quint32 i;

  // Поворачиваем все детали в исходное положение
  for (i = 0; i < detailCount; i++)
    {
      details[i] = qAbs(details[i]);
      p2d[i] = qAbs(p2d[i]);
    }

  // Удаляем из второго списка те детали, которые уже есть в первой половине первого списка
  // Также удаляем последнюю деталь первого списка
  for (i = 0; i < detailCount / 2; i++)
    {
      p2d.removeOne(details[i]);
      details.removeLast();
    }

  // Если нечётное количество, то надо удалить ещё одну
  if (detailCount % 2 == 1)
    {
      p2d.removeOne(details[detailCount / 2]);
    }

  // Объединяем списки
  details += p2d;
  p2d = QVector<qint32>(pT2->details);

  // Поворачиваем каждую деталь так, как она повёрнута у второго предка
  for (i = 0; i < detailCount; i++)
    {
      details[i] *= p2d[i] > 0 ? 1 : -1;
    }
}

ICreature *cutter::create()
{
  return new cutter();
}

void cutter::initDoubleParam(quint32 number) {}

void cutter::calculate()
{
  list.clear();
  places.clear();
  places.append(place{ 0, 0, maxwidth, maxheight });
  totalwidth = totalheight = freex = freey = 0;
  bool success = true;
  quint32 dc = 0;

  // Пытаемся добавить деталь и смотрим, получилось ли
  for (quint32 i = 0; i < detailCount; i++)
    {
      bool s = addRect(details[i]);
      success &= s;
      dc += s ? 1 : 0;
    }

  qreal totalsquare = totalwidth * totalheight;
  qreal freesquare;

  // Если есть пустой прямоугольник в углу
  if (hasfree)
    { freesquare = (totalwidth - freex) * (totalheight - freey); }
  else
    { freesquare = 0; }

  // Вычисляем эффективность
  fitness = square / (totalsquare - freesquare);

  // Если не удалось разместить хотя бы одну деталь
  if (!success)
    { fitness = 1.0 * dc - detailCount; }

  // Очищаем список потенциальных мест укладки за ненадобностью
  places.clear();
}

void cutter::prepare()
{
  // Регистрируем для работы сигналов, типичная заморочка Qt
  qRegisterMetaType<QVector<detailType>/**/>();
  qRegisterMetaType<QVector<detail>/**/>();
  // Окно с информацией
  w = new info();
  // Окно настройки
  setup *s = new setup();
  s->exec();
  // Получаем параметры
  maxwidth = s->getWidth();
  maxheight = s->getHeight();
  types = QVector<detailType>(*s->getTypes());
  typecount = types.count();
  detailSet = QVector<qint32>();
  QVector<quint32> c = *s->getCount();

  for (qint32 i = 0, n = c.count(); i < n; i++)
    {
      for (quint32 j = 0; j < c[i]; j++)
        { detailSet.append(i + 1); }
    }

  mode = s->getMode();
  hasfree = (mode == 2 || mode == 5);
  square = s->getSquare();
  delete s;
  // Сохраняем количество деталей в отдельную переменную
  detailCount = detailSet.count();
  connect(this, SIGNAL(setFieldSize(qreal, qreal)), w, SLOT(setFieldSize(qreal, qreal)));
  emit setFieldSize(maxwidth, maxheight);
}

void cutter::clean()
{
  delete w;
}

QDialog *cutter::getInfoWindow()
{
  return w;
}

void cutter::updateInfoWindow()
{
  connect(this, SIGNAL(setRects(QVector<detail> *, qreal, qreal, qreal, qreal)),
          w, SLOT(setRects(QVector<detail> *, qreal, qreal, qreal, qreal)));
  emit setRects(&list, totalwidth, totalheight, freex, freey);
}

// Добавление одной детали на лист, основная мякотка
bool cutter::addRect(qint32 type)
{
  qreal x = 0.0, y = 0.0, width, height;
  bool swap = type < 0; // Надо ли менять размеры местами (деталь повёрнута?)
  type = qAbs(type) - 1;
  width = types[type].width;
  height = types[type].height;

  if (swap) { qSwap(width, height); } // Если надо - меняем!

  qint32 i;

  // Ищем место, куда можно поместить деталь
  for (i = 0; i < places.count(); i++)
    {
      place p = places[i];

      if (p.maxx >= width && p.maxy >= height) // Нашли
        {
          x = p.x;
          y = p.y;
          break;
        }
    }

  // Если не нашли - возвращаем ложь. Грустно
  if (i == places.count())
    {
      return false;
    }

  places.remove(i);

  // Увеличиваем общий занимаемый размер если надо
  if (x + width > totalwidth)
    { totalwidth = x + width; }

  if (y + height > totalheight)
    { totalheight = y + height; }

  // Если есть свободный прямоугольник в углу - возможно, придётся менять его размеры
  if (hasfree)
    {
      if (x + width > y + height) // Меняем
        { freey = qMax(freey, y + height); }
      else
        { freex = qMax(freex, x + width); }
    }
  else // Делаем размер прямоугольника нулевым
    {
      freex = totalwidth;
      freey = totalheight;
    }

  // Добавляем информацию о детальке  список
  list.append(detail{ x, y, width, height, (quint32)type, swap });
  places.append(place{ 0, totalheight, maxwidth, maxheight - totalheight });
  places.append(place{ totalwidth, 0, maxwidth - totalwidth, maxheight });
  places.append(place{ x + width, y, maxwidth - x - width, maxheight - y });
  places.append(place{ x, y + height, maxwidth - x, maxheight - y - height });

  // Проверяем все места-кандидаты и удаляем те, в которые детали больше не влезут
  for (i = places.count() - 1; i >= 0; i--)
    {
      for (qint32 j = list.count() - 1; j >= 0; j--)
        {
          detail d = list[j];

          if (places[i].x <= d.x && places[i].y >= d.y && places[i].y < d.y + d.height)
            { places[i].maxx = qMin(places[i].maxx, d.x - places[i].x); }

          if (places[i].y <= d.y && places[i].x >= d.x && places[i].x < d.x + d.width)
            { places[i].maxy = qMin(places[i].maxy, d.y - places[i].y); }

          if (places[i].x >= d.x && places[i].x < d.x + d.width &&
              places[i].y >= d.y && places[i].y < d.y + d.height)
            {
              places[i].maxx = places[i].maxy = 0;
            }
        }

      bool keep = false;

      for (quint32 j = 0; j < typecount; j++)
        {
          keep |= (places[i].maxx >= types[j].width && places[i].maxy >= types[j].height) ||
                  (places[i].maxx >= types[j].height && places[i].maxy >= types[j].width);
        }

      // Таки надо удалить
      if (!keep)
        {
          places.remove(i);
          continue;
        }
    }

  // В этих режимах нет сортировки мест-кандидатов, поэтому просто выходим
  if (mode == 4 || mode == 5)
    { return true; }

  // А в остальных - надо сортировать
  std::sort(places.begin(), places.end(), cmpRange);
  return true;
}
