#include "tpopulation.h"

ICreature *root; // Коренная особь
quint32 paramCount, stopGen, stopTime;
bool simpleMutation, simpleCrossover, increase, stopG, stopT, stopF;
qreal stopFt;
QDialog *wnd;

const qreal maxDifference = 0.0015;

// Два варианта сортировки
bool cmpi(ICreature *i, ICreature *j) { return (i->fitness > j->fitness); }

bool cmpd(ICreature *i, ICreature *j) { return (i->fitness < j->fitness); }

// Обёртка вызова функции вычисления приспособленности для многопоточного выполнения
void calc(ICreature *&c)
{
  if (qIsNaN(c->fitness))
    { c->calculate(); }
}

// Инициализация
TPopulation::TPopulation(QObject *parent): QThread(parent)
{
  QThreadPool::globalInstance()->reserveThread();
  qsrand(QTime::currentTime().msec());
  abort = false;
  reconfigure = stop = stopRequested = true;
  elapsedTime = generation = 0;
  this->w = wnd;
  root = NULL;
  QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount() - 1);
}

TPopulation::~TPopulation()
{
  mutex.lock();
  abort = true;
  values.clear();
  clear();
  condition.wakeOne();
  mutex.unlock();
  wait();
  QThreadPool::globalInstance()->releaseThread();
}

// Обновление настроек ГА
void TPopulation::updateSettings(const settings &s, bool isReset, bool resetTime)
{
  this->s = s;
  mutex.lock();
  reconfigure = true;

  // После сброса?
  if (isReset)
    {
      generation = 0;
      clear();
      divider = 1;

      if (resetTime)
        { elapsedTime = 0; }
    }

  mutex.unlock();
}

// Обновление информации окна
void TPopulation::requestUpdate()
{
  values.clear();
  mutex.lock();

  // Получаем приспособленность всех особей
  for (i = 0; i < creatures.count(); i++)
    {
      values.append(creatures[i]->fitness) ;
    }

  mutex.unlock();

  if (!creatures.empty() && w->isVisible())
    { creatures.first()->updateInfoWindow(); }

  emit updateView(generation, elapsedTime, values);
}

// Остановка поиска
void TPopulation::requestStop()
{
  stop = true;
  stopRequested = true;
}

// Возобновление поиска
void TPopulation::requestContinue()
{
  t.start();
  condition.wakeOne();
}

// Прерывание поиска, при выходе из программы
void TPopulation::requestAbort()
{
  abort = true;

  if (stop)
    { condition.wakeOne(); }
}

// Запрос на обновление окна с подробной информацией
void TPopulation::requestFullInfo(quint32 index)
{
  creatures[index]->updateInfoWindow();
}

// Установка условий останова
void TPopulation::setStopConditions(bool sgen, quint32 gen, bool stime, const QTime &time, bool sf, qreal f)
{
  stopG = sgen;
  stopT = stime;
  stopF = sf;
  stopGen = gen;
  stopTime = time.hour() * 60 * 60 * 1000;
  stopTime += time.minute() * 60 * 1000;
  stopTime += time.second() * 1000;
  stopFt = f;
}

// Установка коренной особи
QString TPopulation::setRoot(ICreature *newRoot)
{
  clear();
  root = newRoot;
  root->prepare();
  paramCount = root->paramCount();
  wnd = w = root->getInfoWindow();
  simpleCrossover = root->isSimpleCrossover();
  simpleMutation = root->isSimpleMutation();
  increase = true;
  return root->name();
}

// Определение степени различия особей
qreal TPopulation::difference(qint32 c1, qint32 c2)
{
  qreal result = 0;

  for (quint32 i = 0; i < paramCount; i++)
    {
      result += qPow((creatures[c1]->realParams[i] - creatures[c2]->realParams[i]), 2);
    }

  return result;
}

// Простое скрещивание
void TPopulation::crossover(quint32 parent1, quint32 parent2)
{
  ICreature *last, *p1, *p2;
  creatures.append(last = root->create());
  p1 = creatures[parent1];
  p2 = creatures[parent2];

  for (quint32 i = 0; i < paramCount; i++)
    { last->realParams[i] = (RNG::getreal() < 0.5) ? p1->realParams[i] : p2->realParams[i]; }

  last->fitness = NAN;
}

// Удаление всех особей
void TPopulation::clear()
{
  foreach (ICreature *c, creatures)
    {
      delete c;
    }

  creatures.clear();
}

// Основной цикл
void TPopulation::run()
{
  // Куча переменных для настроек
  quint32 populationMax, populationMin;
  qreal probabilityMutation;
  bool selectFromNew, mutateOnce, createNew, clear, threadable;
  populationMax = 50;
  populationMin = 10;
  probabilityMutation = 0.1;
  selectFromNew = false;
  mutateOnce = false;
  createNew = true;
  clear = true;
  threadable = true;
  // Узнаём время начала
  t.start();

  forever
    {
      // Чтобы всё поколение считалось одинаково и полностью - блокируем мьютекс
      mutex.lock();

      // Нужно остановить
      if (stop)
        {
          if (!creatures.isEmpty())
            {
              // Обязательно добавляем точку на график!
              emit addPoint(generation, creatures[0]->fitness);
            }

          emit stopped(stopRequested);
          condition.wait(&mutex);
          stop = false;
        }

      // Прерываем, ибо закрывают программу
      if (abort)
        {
          mutex.unlock();
          return;
        }

      // Перенастройка
      if (reconfigure)
        {
          populationMax = s.populationMax;
          populationMin = s.populationMin;
          probabilityMutation = s.probabilityMutation;
          selectFromNew = s.selectFromNew;
          mutateOnce = s.mutateOnce;
          createNew = s.createNew;
          clear = s.clear;
          threadable = s.threadable && root->threadable();
          reconfigure = false;
          increase = s.increase;
        }

      // Если надо удалять похожие существа...
      if (clear && ((generation % 10) == 1))
        {
          quint32 c = creatures.count();
          {
            for (ui = 0; ui < c; ui++)
              {
                if (qIsNaN(creatures[ui]->fitness))
                  { continue; }

                for (quint32 j = ui + 1; j < c; j++)
                  {
                    if (qIsNaN(creatures[j]->fitness))
                      { continue; }

                    if (
                      qFuzzyCompare(creatures[ui]->fitness, creatures[j]->fitness) ||
                      (difference(ui, j) < maxDifference))
                      { creatures[j]->fitness = NAN; }
                  }
              }

            // ...удаляем
            for (ui = c - 1; ui > 0; ui--)
              {
                if (qIsNaN(creatures[ui]->fitness))
                  {
                    delete creatures[ui];
                    creatures.removeAt(ui);
                  }
              }
          }
        }

      generation++; // Номер поколения

      // Если вдруг популяция меньше минимального размера - дополняем новыми
      for (ui = creatures.count(); ui < populationMin; ui++)
        {
          creatures.append(root->create());
          creatures.last()->fitness = NAN;
        }

      // Добавляем новое существо если пользователь хочет
      if (createNew)
        {
          creatures.append(root->create());
          creatures.last()->fitness = NAN;
        }

      quint32 c1, c2;

      // Селекция
      if (selectFromNew) // Если выбирать из новых
        {
          quint32 count;

          for (ui = creatures.count(); ui < populationMax; ui++)
            {
              count = creatures.count();
              c1 = RNG::getint(0, count);
              c2 = RNG::getint(0, count, c1);

              if (simpleCrossover)
                { crossover(c1, c2); }
              else
                {
                  creatures.append(root->create());
                  creatures.last()->crossover(creatures[c1], creatures[c2]);
                  creatures.last()->fitness = NAN;
                }
            }
        }
      else // Если не выбирать из новых
        {
          quint32 upper = creatures.count();

          for (ui = creatures.count(); ui < populationMax; ui++)
            {
              c1 = RNG::getint(0, upper);
              c2 = RNG::getint(0, upper, c1);

              if (simpleCrossover)
                { crossover(c1, c2); }
              else
                {
                  creatures.append(root->create());
                  creatures.last()->crossover(creatures[c1], creatures[c2]);
                  creatures.last()->fitness = NAN;
                }
            }
        }

      // Мутации
      for (i = 1; i < creatures.count(); i++)
        {
          if (simpleMutation) // Если обычная мутация
            if (mutateOnce) // Только одна мутация на особь
              {
                if (RNG::getreal() < probabilityMutation)
                  {
                    creatures[i]->initDoubleParam(RNG::getint(0, paramCount - 1));
                    creatures[i]->fitness = NAN;
                  }
              }
            else // Несколько мутаций на особь
              {
                for (quint32 j = 0; j < paramCount; j++)
                  {
                    if (RNG::getreal() < probabilityMutation)
                      {
                        creatures[i]->initDoubleParam(j);
                        creatures[i]->fitness = NAN;
                      }
                  }
              }
          else // Если мутация реализоана в плагине
            {
              creatures[i]->mutate(mutateOnce, probabilityMutation);
              creatures[i]->fitness = NAN;
            }
        }

      // Вычисление
      if (threadable) // В несколько потоков
        {
          QtConcurrent::blockingMap(creatures, calc);
        }
      else // В один поток
        {
          for (ui = 0; ui < populationMax; ui++)
            {
              if (qIsNaN(creatures[ui]->fitness))
                { creatures[ui]->calculate(); }
            }
        }

      // Сортировка, порядок задаётся пользователем, поэтому условие
      if (increase)
        { std::sort(creatures.begin(), creatures.end(), cmpi); }
      else
        { std::sort(creatures.begin(), creatures.end(), cmpd); }

      // Удаляем последние существа пока размер популяции не станет минимальным
      for (ui = populationMax - 1; ui >= populationMin; ui--)
        {
          delete creatures[ui];
          creatures.removeLast();
        }

      // Если номер поколения кратен делителю...
      // делитель нужен чтобы не ставить огромное число точек на график
      if (generation % divider == 0)
        {
          if (generation % (divider * 10) == 0)
            { divider *= 10; }

          emit addPoint(generation, creatures[0]->fitness);
        }

      mutex.unlock();

      // Условия останова
      if ((stopG && generation >= stopGen) || // Номер поколения
          (stopT && stopTime <= elapsedTime) || // Время работы
          (stopF && // По достигнутому значению
           ((increase && (creatures.first()->fitness >= stopFt)) ||
            (!increase && (creatures.first()->fitness <= stopFt)))
          ))
        {
          stop = true;
          stopRequested = false;
        }

      // Если прошло хотя бы полсекунды - обновляем информацию!
      if ((et = t.elapsed()) > 500)
        {
          elapsedTime += et;
          requestUpdate();
          t.start();
        }
    }
}

