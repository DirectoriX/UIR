#ifndef MAINWINDOW_H
#define MAINWINDOW_H

///FIXME Commentaries!

#include <QMainWindow>
#include <QPluginLoader>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QList>
#include <QMetaType>
#include <qwt_plot_curve.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_grid.h>
#include <qwt_curve_fitter.h>

#if defined(Q_OS_WIN)
  #define extlib "*.dll"
#elif defined(Q_OS_LINUX)
  #define extlib "*.so"
#else
  #define extlib "*.!!"
#endif

#include "../icreature.h"
#include "tpopulation.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *ce);

    QDialog *iw;

  private slots:
    // \/ ToolButtons
    void on_toolButton_Open_clicked();
    void on_toolButton_Reopen_clicked();
    void on_toolButton_Run_clicked();
    void on_toolButton_Stop_clicked();
    void on_toolButton_Reset_clicked();
    void on_toolButton_Info_clicked(bool checked);
    // /\ ToolButtons

    // \/ Базовые настройки
    void on_spin_PopulationMin_valueChanged(qint32 value);
    void on_spin_PopulationMax_valueChanged(qint32 arg1);
    void on_spin_MutationChance_valueChanged(qreal arg1);
    void on_prob_2_clicked();
    void on_checkBox_Decrease_clicked(bool checked);
    // /\ Базовые настройки

    // \/ Расширенные настройки
    void on_checkBox_Extended_clicked(bool checked);
    void on_checkBox_SelectFromNew_clicked(bool checked);
    void on_checkBox_MutateOnce_clicked(bool checked);
    void on_checkBox_CreateNew_clicked(bool checked);
    void on_checkBox_Clear_clicked(bool checked);
    void on_checkBox_Threadable_clicked(bool checked);
    // /\ Расширенные настройки

    // \/ Условия останова
    void on_checkBox_MaxGeneration_clicked(bool checked);
    void on_spin_MaxGen_valueChanged(qint32 arg1);
    void on_checkBox_MaxTime_clicked(bool checked);
    void on_time_timeChanged(const QTime &time);
    void on_checkBox_BestResult_clicked(bool checked);
    void on_spin_f_valueChanged(qreal arg1);
    // /\ Условия останова

    // \/ Результаты
    void on_list_Creatures_itemSelectionChanged();
    // /\ Результаты

    // \/ Слоты TPopulation
    void updateResults(quint32 generation, quint32 time, const QList<double> &values);
    void addPoint(quint32 generation, qreal fitness);
    void stop(bool requested);
    // /\ Слоты TPopulation

  private:
    Ui::MainWindow *ui;
    QString creature_library;
    QPluginLoader *loader = NULL;
    TPopulation p;
    settings s;
    QwtPlotCurve curve, histo;
    QPolygonF poly, poly_m;
};

#endif // MAINWINDOW_H
