#include "MainWindow.h"
#include <locale.h>

MainWindow::MainWindow(double ax, double bx, double ay, double by, QWidget *parent): QMainWindow(parent)
{
    setlocale(LC_ALL, "C");

	central = new QWidget(this);
	drawArea = new DrawArea (this, ax, bx, ay, by);

	QHBoxLayout *layout = new QHBoxLayout(central);

	layout->addWidget(drawArea);
	setCentralWidget (central);

	QMenu *fileMenu = menuBar()->addMenu("&File");
	QMenu *funcMenu = menuBar()->addMenu("&Function");

	QAction *scaleUpAction = funcMenu->addAction("Scale up", this, &MainWindow::scaleUp);
	scaleUpAction->setShortcut(QKeySequence("Ctrl+2"));

	QAction *scaleDownAction = funcMenu->addAction("Scale down", this, &MainWindow::scaleDown);
	scaleDownAction->setShortcut(QKeySequence("Ctrl+3"));

	QAction *exitAction = fileMenu->addAction("E&xit", this, &QWidget::close);
	exitAction->setShortcut(QKeySequence::Quit);

	setWindowTitle ("Graph");
}

MainWindow::~MainWindow()
{
}

void MainWindow::scaleUp()
{
	drawArea->scaleUp();
}

void MainWindow::scaleDown()
{
	drawArea->scaleDown();
}
