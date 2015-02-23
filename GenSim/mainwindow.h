#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include <QFileDialog>
#include "../GenClasses.h"

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
    QVector<ICreature *> population;

    void SetProgress(quint32 value);

  private slots:
    void on_spin_PopulationMin_valueChanged(int value);

    void on_action_Run_triggered();

    void on_action_Stop_triggered();

    void on_action_Reset_triggered();

    void on_action_openCreature_triggered();

  private:
    Ui::MainWindow *ui;
    QString creature_library;
};

#endif // MAINWINDOW_H
