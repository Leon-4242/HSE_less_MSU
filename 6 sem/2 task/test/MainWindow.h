#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QHBoxLayout>

#include "CommonDefs.h"
#include "DrawArea.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        explicit MainWindow(double, double, double, double, QWidget *parent = nullptr);
        ~MainWindow();

    private:
		QWidget *central;
		DrawArea *drawArea;

    private slots:
		void scaleUp();
		void scaleDown();
};

#endif // MAINWINDOW_H
