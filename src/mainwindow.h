#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QCursor>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTableView>
#include <QTimer>
#include <windows.h>
#include "controller.h"
#include <QFileDialog>
#include <QMessageBox>
typedef unsigned int UInt;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    controller *C;

private slots:
    //void predPCUpdata();
    void on_Next_clicked();

    void on_CLEAR_clicked();

    void on_Memory_GO_clicked();

    void on_RESET_clicked();

    void on_RUN_clicked();

    void slotOpenFileDialog();
    //void on_predPC_textChanged();
    //void show_line_num();

    //void on_hbtext_cursorPositionChanged();

    void slotSaveFileDialog();

    void on_SPEED_valueChanged(int value);

    void on_STOP_clicked();

private:
    Ui::MainWindow *ui;
    UInt memory_begin,memory_end,speed;
    QString inittxt;
    void updataMemory();
    void updata();

    QString PRINTFstat();
    QString PRINTDstat();
    QString PRINTEstat();
    QString PRINTMstat();
    QString PRINTWstat();
    QString PRINTCC();
    QString PRINTREG();
    QTimer *time;
};

#endif // MAINWINDOW_H
