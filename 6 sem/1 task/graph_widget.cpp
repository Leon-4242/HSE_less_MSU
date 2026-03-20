#include "graph_widget.h"

#include "common_defs.h"
#include <QPainter>

static
double f_0 (double x)
{
  return x;
}

static
double f_1 (double x)
{
  return x * x * x;
}

GraphWidget::GraphWidget (QWidget *parent, double min_x, double max_x, int n) : QWidget (parent)
{
  m_min_x = min_x;
  m_max_x = max_x;
  m_n = n;

  m_func_id = 0;

  change_func ();
}

QSize GraphWidget::minimumSizeHint () const
{
  return QSize (100, 100);
}

QSize GraphWidget::sizeHint () const
{
  return QSize (1000, 1000);
}

void GraphWidget::change_func ()
{
  m_func_id = (m_func_id + 1) % 2;

  switch (m_func_id)
    {
      case 0:
        m_func_name = "f (x) = x";
        m_f = f_0;
        break;
      case 1:
        m_func_name = "f (x) = x * x * x";
        m_f = f_1;
        break;
    }
  update ();
}

QPointF GraphWidget::graph_to_window (double x_graph, double y_graph)
{
  double x_win = (x_graph - m_min_x) / (m_max_x - m_min_x) * width ();
  double y_win = (m_max_y - y_graph) / (m_max_y - m_min_y) * height ();
  return QPointF (x_win, y_win);
}


void GraphWidget::recompute_y_bounds ()
{
  m_max_y = m_min_y = 0;
  double delta_x = (m_max_x - m_min_x) / m_n;
  for (double x = m_min_x; x - m_max_x < EPSILON_FOR_COMPARE; x += delta_x)
    {
      double y = m_f (x);
      if (y < m_min_y)
        m_min_y = y;
      if (y > m_max_y)
        m_max_y = y;
    }

  double delta_y = 0.01 * (m_max_y - m_min_y);
  m_min_y -= delta_y;
  m_max_y += delta_y;
}

void GraphWidget::paintEvent (QPaintEvent * /* event */)
{
  QPainter painter (this);
  QPen pen_black(Qt::black, 0, Qt::SolidLine); 
  QPen pen_red(Qt::red, 0, Qt::SolidLine); 
  painter.setPen (pen_black);

  recompute_y_bounds();

  double delta_x = (m_max_x - m_min_x) / m_n;
  double x1, x2, y1, y2;
  x1 = m_min_x;
  y1 = m_f (x1);
  for (x2 = x1 + delta_x; x2 - m_max_x < EPSILON_FOR_COMPARE; x2 += delta_x)
    {
      y2 = m_f (x2);
      painter.drawLine (graph_to_window (x1, y1), graph_to_window (x2, y2));
      x1 = x2;
      y1 = y2;
    }
  x2 = m_max_x;
  y2 = m_f (x2);
  painter.drawLine (graph_to_window (x1, y1), graph_to_window (x2, y2));

  painter.setPen (pen_red);
  painter.drawLine (graph_to_window (m_min_x, 0), graph_to_window (m_max_x, 0));
  painter.drawLine (graph_to_window (0, m_min_y), graph_to_window (0, m_max_y));

  painter.setPen ("blue");
  painter.drawText (0, 20, m_func_name);
}
