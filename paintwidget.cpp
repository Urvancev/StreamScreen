#include "paintwidget.h"
#include <QPainter>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>

paintWidget::paintWidget(QOpenGLWidget *parent) :
    QOpenGLWidget(parent)
{
    //setAutoFillBackground(true);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void paintWidget::read(QPixmap* pix) {
    this->pix = *pix;
}

void paintWidget::paintEvent(QPaintEvent *) {
    static QPainter p;
    //static QScreen* screen = QApplication::primaryScreen();

    //static WId wid= QApplication::desktop()->winId();
    //static QPixmap pix = screen->grabWindow(wid,1920);


    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing);
    //pix = screen->grabWindow(wid);
    //pix = screen->grabWindow(wid,1920);
    if (!pix.isNull()) {
        this->pix = this->pix.scaled(this->width(),this->height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        p.drawPixmap(0,0,this->pix);
    }
    p.end();
}
