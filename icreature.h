#ifndef GENINTERFACES
#define GENINTERFACES

#include <QString>
#include <QVector>
#include <QStringList>
#include <QtPlugin>
#include <QDialog>

class ICreature {
  public:
    // Служебная информация
    virtual quint32 paramCount() = 0;
    virtual QString name() = 0;
    virtual bool threadable() = 0;

    // Для определения типа мутации
    virtual bool isSimpleMutation() = 0;
    virtual void mutate(bool onlyOnce, qreal probability) = 0;

    // Для определения типа скрещивания
    virtual bool isSimpleCrossover() = 0;
    virtual void crossover(ICreature *p1, ICreature *p2) = 0;

    qreal fitness;
    qreal *realParams;

    virtual ~ICreature() {}

    // Создать новый объект класса-потомка
    virtual ICreature *create() = 0;

    // Инициализировать значение с плавающей запятой по номеру
    virtual void initDoubleParam(quint32 number) = 0;

    // Подсчёт приспособленности
    virtual void calculate() = 0;

    // Для настройки задачи, вызывается 1 раз при загрузке
    virtual void prepare() = 0;

    // Для очистки задачи, вызывается 1 раз при закрытии
    virtual void clean() = 0;

    // Вернуть окно с информацией
    virtual QDialog *getInfoWindow() = 0;

    // Обновить окно с информацией
    virtual void updateInfoWindow() = 0;
};

#define Creature_iid "DirectoriX.UIR.Creature"

Q_DECLARE_INTERFACE(ICreature, Creature_iid)

#endif // GENINTERFACES

