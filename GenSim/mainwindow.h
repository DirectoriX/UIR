#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent/QtConcurrent>
#include <QPluginLoader>
#include <QMessageBox>
#include <QFileDialog>
#include "../GenClasses.h"
#include <QStringList>

namespace Ui {
  class MainWindow;
  void run(MainWindow *window, quint32 generation, int minpop, int maxpop, qreal mutation_probability);
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool work;
    IPopulation *population;
    qreal mutation_probability, new_probability;

    void SetProgressInfo(quint32 value, bool updateinfo, int count);

  private slots:
    void on_spin_PopulationMin_valueChanged(int value);
    void on_action_Run_triggered();
    void on_action_Stop_triggered();
    void on_action_Reset_triggered();
    void on_action_openCreature_triggered();
    void on_list_Creatures_doubleClicked(const QModelIndex &index);
    void on_Spin_MutationChance_valueChanged(double arg1);
    void on_Spin_NewChance_valueChanged(double arg1);

  private:
    Ui::MainWindow *ui;
    QString creature_library;
    QPluginLoader *loader = NULL;
};

#endif // MAINWINDOW_H
