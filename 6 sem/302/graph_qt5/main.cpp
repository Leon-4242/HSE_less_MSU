#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>

#include "common_defs.h"
#include "graph_widget.h"

#define DEFAULT_A (-5.0)
#define DEFAULT_B (5.0)
#define DEFAULT_N (10)


static
int parse_command_line (int argc, char **argv, double *pa, double *pb, int *pn)
{
    if (argc == 1)
        return 0;

    if (argc != 4)
    {
        fprintf (stderr, "Wrong number of arguments!\n");
        return -1;
    }

    double a, b;
    int n;

    if (sscanf (argv[1], "%lf", &a) != 1)
    {
        fprintf (stderr, "Invalid value for a!\n");
        return -1;
    }

    if (sscanf (argv[2], "%lf", &b) != 1)
    {
        fprintf (stderr, "Invalid value for b!\n");
        return -1;
    }

    if (sscanf (argv[3], "%d", &n) != 1)
    {
        fprintf (stderr, "Invalid value for n!\n");
        return -1;
    }

    double segment_length = b - a;
    if (segment_length < EPSILON_FOR_COMPARE)
    {
        fprintf (stderr, "Too short segment: %lf\n", segment_length);
        return -1;
    }

    if (n <= 0)
    {
        fprintf (stderr, "Non-positive number of points!\n");
        return -1;
    }

    *pa = a;
    *pb = b;
    *pn = b;

    return 0;
}

int main (int argc, char **argv)
{
  double a = DEFAULT_A;
  double b = DEFAULT_B;
  int n = DEFAULT_N;

  int err = parse_command_line (argc, argv, &a, &b, &n);
  if (err < 0)
    return -1;

  QApplication app (argc, argv);

  QMainWindow *main_window = new QMainWindow;
  QMenuBar *menu_bar = new QMenuBar (main_window);
  QMenu *file_menu = new QMenu ("&File", menu_bar);
  menu_bar->addMenu(file_menu);
  QMenu *function_menu = new QMenu ("F&unction", menu_bar);
  menu_bar->addMenu(function_menu);
  GraphWidget *graph_widget = new GraphWidget (main_window, a, b, n);

  QAction *change_function_action = function_menu->addAction ("&Change", graph_widget, &GraphWidget::change_func);
  change_function_action->setShortcut (QString ("Ctrl+C"));

  QAction *exit_action = file_menu->addAction ("E&xit", main_window, &GraphWidget::close);
  exit_action->setShortcut (QString ("Ctrl+X"));

  main_window->setMenuBar (menu_bar);
  main_window->setCentralWidget (graph_widget);
  main_window->setWindowTitle ("Graph");

  main_window->show ();
  app.exec ();
  delete main_window;
  return 0;
}
