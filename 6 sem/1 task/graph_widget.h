#ifndef GRAPH_WIDGET_H
#define GRAPH_WIDGET_H

#include <QWidget>

typedef double (*func_t) (double);

class GraphWidget : public QWidget
{
  Q_OBJECT

private:
  int m_func_id = 0;
  const char *m_func_name = nullptr;
  double m_min_x = 0.0;
  double m_max_x = 0.0;
  double m_min_y = 0.0;
  double m_max_y = 0.0;
  int m_n = 0;
  func_t m_f = nullptr;

public:
  GraphWidget (QWidget *parent, double min_x, double max_x, int n);

  QSize minimumSizeHint () const;
  QSize sizeHint () const;

  QPointF graph_to_window (double x_graph, double y_graph);
  void change_func ();

protected:
  void paintEvent (QPaintEvent *event);

private:
  void recompute_y_bounds ();
};

#endif // GRAPH_WIDGET_H
