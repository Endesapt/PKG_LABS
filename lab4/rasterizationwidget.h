#ifndef RASTERIZATIONWIDGET_H
#define RASTERIZATIONWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>

enum Algorithm { None, StepByStep, DDA, BresenhamLine, BresenhamCircle };
class RasterizationWidget : public QWidget {
    Q_OBJECT

public:
    RasterizationWidget(QWidget *parent = nullptr) : QWidget(parent), currentAlgorithm(None) {
        setMinimumSize(800, 800);
    }

    void setParameters(int x1, int y1, int x2, int y2, int r = 0, Algorithm algo = None) {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
        this->radius = r;
        this->currentAlgorithm = algo;
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        drawGrid(painter);

        switch (currentAlgorithm) {
        case StepByStep:
            drawStepByStep(painter, x1, y1, x2, y2);
            break;
        case DDA:
            drawDDA(painter, x1, y1, x2, y2);
            break;
        case BresenhamLine:
            drawBresenhamLine(painter, x1, y1, x2, y2);
            break;
        case BresenhamCircle:
            drawBresenhamCircle(painter, x1, y1, radius);
            break;
        default:
            break;
        }
    }

private:
    Algorithm  currentAlgorithm;
    int x1, y1, x2, y2, radius;
    const int gridSize = 30;

    void drawGrid(QPainter &painter) {
        painter.setPen(Qt::lightGray);
        int step = width() / gridSize;
        for (int i = 0; i <= gridSize; ++i) {
            painter.drawLine(i * step, 0, i * step, height());
            painter.drawLine(0, i * step, width(), i * step);
        }
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
            painter.drawEllipse(QPoint(round(x) * width() / gridSize, round(y) * height() / gridSize), 2, 2);
            x += xInc;
            y += yInc;
        }
    }

    void drawDDA(QPainter &painter, int x1, int y1, int x2, int y2) {
        painter.setPen(Qt::green);
        int dx = x2 - x1;
        int dy = y2 - y1;
        int steps = std::max(abs(dx), abs(dy));
        float xInc = dx / (float)steps;
        float yInc = dy / (float)steps;
        float x = x1;
        float y = y1;
        for (int i = 0; i <= steps; ++i) {
            painter.drawEllipse(QPoint(round(x) * width() / gridSize, round(y) * height() / gridSize), 2, 2);
            x += xInc;
            y += yInc;
        }
    }

    void drawBresenhamLine(QPainter &painter, int x1, int y1, int x2, int y2) {
        painter.setPen(Qt::blue);
        int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
        int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
        int err = (dx > dy ? dx : -dy) / 2, e2;

        while (true) {
            painter.drawEllipse(QPoint(x1 * width() / gridSize, y1 * height() / gridSize), 2, 2);
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
        int step = width() / gridSize;
        painter.drawEllipse(QPoint((xc + x) * step, (yc + y) * step), 2, 2);
        painter.drawEllipse(QPoint((xc - x) * step, (yc + y) * step), 2, 2);
        painter.drawEllipse(QPoint((xc + x) * step, (yc - y) * step), 2, 2);
        painter.drawEllipse(QPoint((xc - x) * step, (yc - y) * step), 2, 2);
        painter.drawEllipse(QPoint((xc + y) * step, (yc + x) * step), 2, 2);
        painter.drawEllipse(QPoint((xc - y) * step, (yc + x) * step), 2, 2);
        painter.drawEllipse(QPoint((xc + y) * step, (yc - x) * step), 2, 2);
        painter.drawEllipse(QPoint((xc - y) * step, (yc - x) * step), 2, 2);
    }
};


#endif // RASTERIZATIONWIDGET_H
