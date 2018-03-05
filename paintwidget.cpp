#include "paintwidget.h"
#include <QPainter>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>


paintWidget::paintWidget(QOpenGLWidget *parent) :
    QOpenGLWidget(parent)
{
    //setAutoFillBackground(true);
    //this->pix = new QPixmap(1920,1080);
    this->pix = new QPixmap(1280,720);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void paintWidget::read(unsigned short x, unsigned short y, QPixmap* pix) {
    static QPainter p;
    p.begin(this->pix);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawPixmap(x,y,*pix);
    p.end();
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
    if (!pix->isNull()) {
        p.drawPixmap(0,0, this->pix->scaled(this->width(),this->height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    }
    p.end();
}
