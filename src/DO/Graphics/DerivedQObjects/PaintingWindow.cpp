// ========================================================================== //
// This file is part of DO++, a basic set of libraries in C++ for computer 
// vision.
//
// Copyright (C) 2013 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public 
// License v. 2.0. If a copy of the MPL was not distributed with this file, 
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

#include "PaintingWindow.hpp"
#include <QtWidgets>

namespace DO {

  // ====================================================================== //
  // ScrollArea
  ScrollArea::ScrollArea(QWidget* parent) : QScrollArea(parent)
  {
    setAlignment(Qt::AlignCenter);
    setAttribute(Qt::WA_DeleteOnClose);
  }

  void ScrollArea::closeEvent(QCloseEvent *event)
  {
    if(event->spontaneous())
    {
      qWarning() << "\n\nWarning: you closed a window unexpectedly!\n\n";
      qWarning() << "Graphical application is terminating...";
      qApp->exit(0);
    }
  }

  // ====================================================================== //
  // PaintingWindow
  PaintingWindow::PaintingWindow(int width, int height,
                                 const QString& windowTitle, int x, int y,
                                 QWidget* parent)
    : QWidget(parent)
    , scroll_area_(new ScrollArea(parent))
    , pixmap_(width, height)
    , painter_(&pixmap_)
  {
    setFocusPolicy(Qt::WheelFocus);
    
    // Set event listener.
    event_listening_timer_.setSingleShot(true);
    connect(&event_listening_timer_, SIGNAL(timeout()),
            this, SLOT(eventListeningTimerStopped()));
    
    // Move widget.
    if (x != -1 && y != -1)
      scroll_area_->move(x,y);
    scroll_area_->setWindowTitle(windowTitle);
    scroll_area_->setWidget(this);
    scroll_area_->setFocusProxy(this);
    
    // Maximize if necessary.
    if ( width >= qApp->desktop()->width()   ||
         height >= qApp->desktop()->height() )
      scroll_area_->showMaximized();
    // Resize the scroll area with the size plus a two-pixel offset.
    else
      scroll_area_->resize(width+2, height+2);
    resize(width, height);

    // Initialize the pixmap.
    pixmap_.fill();
    update();
    
    // Show the widget.
    scroll_area_->show();
  }

  QString PaintingWindow::windowTitle() const
  {
    return scroll_area_->windowTitle();
  }

  int PaintingWindow::x() const
  {
    return scroll_area_->pos().x();
  }

  int PaintingWindow::y() const
  {
    return scroll_area_->pos().y();
  }

  void PaintingWindow::drawPoint(int x, int y, const QColor& c)
  {
    painter_.setPen(c);
    painter_.drawPoint(x, y);
    update();
  }

  void PaintingWindow::drawPoint(const QPointF& p, const QColor& c)
  {
    painter_.setPen(c);
    painter_.drawPoint(p);
    update();
  }

  void PaintingWindow::drawLine(int x1, int y1, int x2, int y2,
                                const QColor& c, int penWidth)
  {
    painter_.setPen(QPen(c, penWidth));
    painter_.drawLine(x1, y1, x2, y2);
    update();
  }

  void PaintingWindow::drawLine(const QPointF& p1, const QPointF& p2, 
                                const QColor& c, int penWidth)
  {
    painter_.setPen(QPen(c, penWidth));
    painter_.drawLine(p1, p2);
    update();
  }

  void PaintingWindow::drawCircle(int xc, int yc, int r, const QColor& c,
                                  int penWidth)
  {
    painter_.setPen(QPen(c, penWidth));
    painter_.drawEllipse(QPoint(xc, yc), r, r);
    update();
  }

  void PaintingWindow::drawCircle(const QPointF& center, float r,
                                  const QColor& c, int penWidth)
  {
    painter_.setPen(QPen(c, penWidth));
    painter_.drawEllipse(QPointF(center.x(), center.y()), r, r);
    update();
  }

  void PaintingWindow::drawEllipse(int x, int y, int w, int h,
                                   const QColor& c, int penWidth)
  {
    painter_.setPen(QPen(c, penWidth));
    painter_.drawEllipse(x, y, w, h);
    update();
  }

  void PaintingWindow::drawEllipse(const QPointF& center, qreal r1, qreal r2,
                                   qreal degree, const QColor& c, int penWidth)
  {
    painter_.save();
    painter_.setPen(QPen(c, penWidth));
    painter_.translate(center);
    painter_.rotate(degree);
    painter_.translate(-r1, -r2);
    painter_.drawEllipse(QRectF(0, 0, 2*r1, 2*r2));
    painter_.restore();
    update();
  }

  void PaintingWindow::drawPoly(const QPolygonF& polygon, const QColor& c,
                                int width)
  {
    painter_.setPen(QPen(c, width));
    painter_.drawPolygon(polygon);
    update();
  }

  void PaintingWindow::drawRect(int x, int y, int w, int h, const QColor& c,
                                int penWidth)
  {
    painter_.setPen(QPen(c, penWidth));
    painter_.drawRect(x, y, w, h);
    update();
  }

  void PaintingWindow::drawText(int x, int y, const QString& text,
                                const QColor& color, int fontSize,
                                double orientation, bool italic, bool bold,
                                bool underline)
  {
    QFont font;
    font.setPointSize(fontSize);
    font.setItalic(italic);
    font.setBold(bold);
    font.setUnderline(underline);

    painter_.save();
    painter_.setPen(color);
    painter_.setFont(font);

    painter_.translate(x, y);
    painter_.rotate(qreal(orientation));
    painter_.drawText(0, 0, text);
    painter_.restore();
    update();
  }

  void PaintingWindow::drawArrow(int x1, int y1, int x2, int y2,
                                 const QColor& col,
                                 int arrowWidth, int arrowHeight, int style,
                                 int width)
  {
    double sl;
    double dx = x2-x1;
    double dy = y2-y1;
    double norm= qSqrt(dx*dx+dy*dy);
    if (norm < 0.999) // null vector
    {
      painter_.setPen(QPen(col, width));
      painter_.drawPoint(x1, y1);
      update();
      return;
    }

    QPainterPath path;
    QPolygonF pts;
    
    qreal dx_norm = dx / norm;
    qreal dy_norm = dy / norm;
    qreal p1x = x1 + dx_norm*(norm-arrowWidth) + arrowHeight/2.*dy_norm;
    qreal p1y = y1 + dy_norm*(norm-arrowWidth) - arrowHeight/2.*dx_norm;
    qreal p2x = x1 + dx_norm*(norm-arrowWidth) - arrowHeight/2.*dy_norm;
    qreal p2y = y1 + dy_norm*(norm-arrowWidth) + arrowHeight/2.*dx_norm;
    switch(style) {
      case 0:
        painter_.setPen(QPen(col, width));
        painter_.drawLine(x1, y1, x2, y2);
        painter_.drawLine(x2, y2, int(p1x), int(p1y));
        painter_.drawLine(x2, y2, int(p2x), int(p2y));
        break;
      case 1:
        pts << QPointF(p2x, p2y);
        pts << QPointF(x2, y2);
        pts << QPointF(p1x, p1y);
        sl = norm-(arrowWidth*.7);
        pts << QPointF(x1 + dx_norm*sl + dy_norm*width, 
                       y1 + dy_norm*sl - dx_norm*width);
        pts << QPointF(x1 + dy_norm*width, y1 - dx_norm*width);
        pts << QPointF(x1 - dy_norm*width, y1 + dx_norm*width);
        pts << QPointF(x1 + dx_norm*sl - dy_norm*width,
                       y1 + dy_norm*sl + dx_norm*width);
        path.addPolygon(pts);
        painter_.fillPath(path, col);
        break;
      case 2:
        pts << QPointF(p2x, p2y);
        pts << QPointF(x2, y2);
        pts << QPointF(p1x, p1y);
        sl = norm-arrowWidth;
        pts << QPointF(x1 + dx_norm*sl + dy_norm*width, 
                       y1 + dy_norm*sl - dx_norm*width);
        pts << QPointF(x1 + dy_norm*width, y1-dx_norm*width);
        pts << QPointF(x1 - dy_norm*width, y1+dx_norm*width);
        pts << QPointF(x1 + dx_norm*sl - dy_norm*width,
                       y1 + dy_norm*sl + dx_norm*width);
        path.addPolygon(pts);
        painter_.fillPath(path, col);
        break;
      default:
        break;
    }
    
    update();
  }

  void PaintingWindow::display(const QImage& image, int xoff, int yoff,
                               double fact)
  {
    painter_.translate(xoff, yoff);
    painter_.scale(qreal(fact), qreal(fact));
    painter_.drawImage(0, 0, image);
    painter_.scale(qreal(1./fact), qreal(1./fact));
    painter_.translate(-xoff, -yoff);
    update();
  }

  void PaintingWindow::fillCircle(int x, int y, int r, const QColor& c)
  {
    QPainterPath path;
    path.addEllipse(qreal(x)-r/2., qreal(y)-r/2., qreal(r), qreal(r));
    painter_.fillPath(path, c);
    update();
  }

  void PaintingWindow::fillCircle(const QPointF& p, qreal r, const QColor& c)
  {
    QPainterPath path;
    path.addEllipse(p, r, r);
    painter_.fillPath(path, c);
    update();
  }

  void PaintingWindow::fillEllipse(int x, int y, int w, int h, 
                                   const QColor& c)
  {
    QPainterPath path;
    path.addEllipse(qreal(x), qreal(y), qreal(w), qreal(h));
    painter_.fillPath(path, c);
    update();
  }

  void PaintingWindow::fillEllipse(const QPointF& p, qreal rx, qreal ry,
                                   qreal degree, const QColor& c)
  {
    painter_.save();
    painter_.translate(p);
    painter_.rotate(degree);
    painter_.translate(-rx, -ry);
    QPainterPath path;
    path.addEllipse(0., 0., 2*rx, 2*ry);
    painter_.fillPath(path, c);
    painter_.restore();
    update();
  }

  void PaintingWindow::fillPoly(const QPolygonF& polygon, const QColor& c)
  {
    QPainterPath path;
    path.addPolygon(polygon);
    painter_.fillPath(path, c);
    update();
  }

  void PaintingWindow::fillRect(int x, int y, int w, int h, 
                                const QColor& c)
  {
    painter_.setPen(c);
    painter_.fillRect(x, y, w, h, c);
    update();
  }

  void PaintingWindow::clear()
  {
    pixmap_.fill();
    update();
  }

  void PaintingWindow::setAntialiasing(bool on)
  { painter_.setRenderHints(QPainter::Antialiasing, on); }

  void PaintingWindow::setTransparency(bool on)
  {
    if (on)
      painter_.setCompositionMode(QPainter::CompositionMode_Multiply);
    else
      painter_.setCompositionMode(QPainter::CompositionMode_Source);
  }

  void PaintingWindow::saveScreen(const QString& filename)
  {
    pixmap_.save(filename);
  }

  void PaintingWindow::resizeScreen(int width, int height)
  {
    if (pixmap_.width() == width && pixmap_.height() == height)
      return;
    /*
       The following internal changes are critical to prevent Qt from crashing.
       1. Tell QPainter 'painter_' to stop using using QPixmap 'pixmap_'.
       2. Reinitialize the QPixmap with the new size.
       3. Now we can re-allow QPainter 'painter_' to re-use QPixmap 'pixmap_'.
     */
    painter_.end();
    pixmap_ = QPixmap(width, height);
    pixmap_.fill();
    painter_.begin(&pixmap_);
    
    // Resize the window and the scroll area as follows.
    resize(width, height);
    if (width > qApp->desktop()->width() || height > qApp->desktop()->height())
    {
      width = 800;
      height = 600;
    }
    scroll_area_->resize(width+2, height+2);
  }

  void PaintingWindow::waitForEvent(int ms)
  {
    event_listening_timer_.setInterval(ms);
    event_listening_timer_.start();
  }

  void PaintingWindow::eventListeningTimerStopped()
  {
    emit sendEvent(noEvent());
  }

  void PaintingWindow::mouseMoveEvent(QMouseEvent *event)
  {
    emit movedMouse(event->x(), event->y(), event->buttons());

    if (event_listening_timer_.isActive())
    {
      event_listening_timer_.stop();
      emit sendEvent(mouseMoved(event->x(), event->y(), event->buttons(),
                     event->modifiers()));
    }
  }

  void PaintingWindow::mousePressEvent(QMouseEvent *event)
  {
#ifdef Q_OS_MAC
    Qt::MouseButtons buttons = (event->modifiers() == Qt::ControlModifier &&
                  event->buttons() == Qt::LeftButton) ? 
    Qt::MiddleButton : event->buttons();
    emit pressedMouseButtons(event->x(), event->y(), buttons);
#else
    emit pressedMouseButtons(event->x(), event->y(), event->buttons());
#endif
    if (event_listening_timer_.isActive())
    {
      event_listening_timer_.stop();
      emit sendEvent(mousePressed(event->x(), event->y(), event->buttons(),
                     event->modifiers()));
    }
  }

  void PaintingWindow::mouseReleaseEvent(QMouseEvent *event)
  {
#ifdef Q_OS_MAC
    Qt::MouseButtons buttons = (event->modifiers() == Qt::ControlModifier &&
      event->buttons() == Qt::LeftButton) ? 
      Qt::MiddleButton : event->buttons();
    emit releasedMouseButtons(event->x(), event->y(), buttons);
#else
    emit releasedMouseButtons(event->x(), event->y(), event->buttons());
#endif
    if (event_listening_timer_.isActive())
    {
      event_listening_timer_.stop();
      emit sendEvent(mouseReleased(event->x(), event->y(),
                                   event->buttons(), event->modifiers()));
    }
  }

  void PaintingWindow::keyPressEvent(QKeyEvent *event)
  {
    emit pressedKey(event->key());
    if (event_listening_timer_.isActive())
    {
      event_listening_timer_.stop();
      emit sendEvent(keyPressed(event->key(), event->modifiers()));
    }
  }

  void PaintingWindow::keyReleaseEvent(QKeyEvent *event)
  {
    emit releasedKey(event->key());
    if (event_listening_timer_.isActive())
    {
      event_listening_timer_.stop();
      emit sendEvent(keyReleased(event->key(), event->modifiers()));
    }
  }

  void PaintingWindow::paintEvent(QPaintEvent *event)
  {    
    QPainter p(this);
    p.drawPixmap(0, 0, pixmap_);
  }

} /* namespace DO */
