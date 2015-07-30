#ifndef TPOPULATION_H
#define TPOPULATION_H

///FIXME Commentaries!

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTime>
#include <QtMath>
#include <QtConcurrent>

#include "../icreature.h"
#include "../rng.h"

struct settings
{
  quint32 populationMax;
  quint32 populationMin;
  qreal probabilityMutation;
  bool selectFromNew;
  bool mutateOnce;
  bool createNew;
  bool clear;
  bool threadable;
  bool increase;
};

class TPopulation : public QThread {

    Q_OBJECT

  public:
    TPopulation(QObject *parent = 0);
    ~TPopulation();

    void updateSettings(const settings &s, bool isReset, bool resetTime = true);
    void requestUpdate(void);
    void requestStop(void);
    void requestContinue(void);
    void requestAbort(void);
    void requestFullInfo(quint32 index);
    void setStopConditions(bool sgen, quint32 gen, bool stime, const QTime &time, bool sf, qreal f);
    QString setRoot(ICreature *newRoot);
    void merge(const QVector<ICreature *> &other);
    qreal difference(qint32 c1, qint32 c2);
    void crossover(quint32 parent1, quint32 parent2);
    void clear();

    QDialog *w;
    QVector<ICreature *> creatures;
    bool stop;

  signals:
    void updateView(quint32 generation, quint32 time, const QList<double> &values);
    void addPoint(quint32 generation, qreal fitness);
    void stopped(bool requested);

  protected:
    void run() Q_DECL_OVERRIDE;

  private:
    QMutex mutex;
    QWaitCondition condition;
    bool reconfigure, abort, stopRequested;
    quint32 generation, ui, divider, elapsedTime;
    qint32 et, i;
    QList<double> values;
    settings s;
    QTime t;
};

#endif // TPOPULATION_H
