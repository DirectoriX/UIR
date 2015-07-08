#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent/QtConcurrent>
#include <QPluginLoader>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QMovie>

#include "../icreature.h"
#include "../rng.h"
#include "population.h"

namespace Ui {
  class MainWindow;
  void run(MainWindow *window, quint32 generation, qint32 minpop, qint32 maxpop, qreal mutation_probability);
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    mutable bool work;
    mutable qreal mutation_probability, new_probability;
    mutable qint32 minpop, maxpop;

    void SetProgressInfo(quint32 value);

    void closeEvent(QCloseEvent *ce);

  private slots:
    void on_spin_PopulationMin_valueChanged(qint32 value);
    void on_action_Run_triggered();
    void on_action_Stop_triggered();
    void on_action_Reset_triggered();
    void on_action_openCreature_triggered();
    void on_list_Creatures_doubleClicked(const QModelIndex &index);
    void on_Spin_MutationChance_valueChanged(qreal arg1);
    void on_Spin_NewChance_valueChanged(qreal arg1);
    void on_spin_PopulationMax_valueChanged(qint32 arg1);

private:
    mutable Ui::MainWindow *ui;
    QString creature_library;
    QPluginLoader *loader = NULL;
};

#endif // MAINWINDOW_H
