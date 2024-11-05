#ifndef RASTERIZATIONWIDGET_H
#define RASTERIZATIONWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class RasterizationWidget;
}
QT_END_NAMESPACE

class RasterizationWidget : public QWidget {
public:
    RasterizationWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setMinimumSize(800, 600);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);

        // Пример вызова всех алгоритмов
        drawStepByStep(painter, 50, 50, 200, 200);
        drawDDA(painter, 250, 50, 400, 200);
        drawBresenhamLine(painter, 450, 50, 600, 200);
        drawBresenhamCircle(painter, 700, 150, 150);
    }

    void drawStepByStep(QPainter &painter, int x1, int y1, int x2, int y2) {
        painter.setPen(Qt::red);
        float dx = x2 - x1;
        float dy = y2 - y1;
        float steps = std::max(abs(dx), abs(dy));
        float xInc = dx / steps;
        float yInc = dy / steps;
        float x = x1;
        float y = y1;
        for (int i = 0; i <= steps; ++i) {
            painter.drawPoint(round(x), round(y));
            x += xInc;
            y += yInc;
        }
    }

    void drawDDA(QPainter &painter, int x1, double y1, int x2, double y2) {
        painter.setPen(Qt::green);
        double Dx=x2-x1,Dy=y2-y1;
        painter.drawPoint(x1,y1);
        while(x1<x2){
            x1=x1+1;
            y1=y1+Dy/Dx;
            painter.drawPoint(x1,y1);
        }
    }

    void drawBresenhamLine(QPainter &painter, int x1, int y1, int x2, int y2) {
        painter.setPen(Qt::blue);
        int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
        int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
        int err = (dx > dy ? dx : -dy) / 2, e2;

        while (true) {
            painter.drawPoint(x1, y1);
            if (x1 == x2 && y1 == y2) break;
            e2 = err;
            if (e2 > -dx) { err -= dy; x1 += sx; }
            if (e2 < dy) { err += dx; y1 += sy; }
        }
    }

    void drawBresenhamCircle(QPainter &painter, int xc, int yc, int r) {
        painter.setPen(Qt::black);
        int x = 0, y = r;
        int d = 3 - 2 * r;
        drawCirclePoints(painter, xc, yc, x, y);
        while (y >= x) {
            x++;
            if (d > 0) {
                y--;
                d = d + 4 * (x - y) + 10;
            } else {
                d = d + 4 * x + 6;
            }
            drawCirclePoints(painter, xc, yc, x, y);
        }
    }

    void drawCirclePoints(QPainter &painter, int xc, int yc, int x, int y) {
        painter.drawPoint(xc + x, yc + y);
        painter.drawPoint(xc - x, yc + y);
        painter.drawPoint(xc + x, yc - y);
        painter.drawPoint(xc - x, yc - y);
        painter.drawPoint(xc + y, yc + x);
        painter.drawPoint(xc - y, yc + x);
        painter.drawPoint(xc + y, yc - x);
        painter.drawPoint(xc - y, yc - x);
    }
};
#endif // RASTERIZATIONWIDGET_H
