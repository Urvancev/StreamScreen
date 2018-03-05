#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QOpenGLWidget>

class paintWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit paintWidget(QOpenGLWidget *parent = 0);
    QPixmap* pix;

signals:

public slots:
    void paintEvent(QPaintEvent *);
    void read(unsigned short x, unsigned short y, QPixmap* pix);
};

#endif // PAINTWIDGET_H
