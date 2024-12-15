#include "mainwindow.h"
#include <cmath>
#include<QGraphicsTextItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gridSize(3)
{
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);
    view->setMinimumSize(800, 800);
    view->setRenderHint(QPainter::Antialiasing, false);

    algorithmSelect = new QComboBox();
    algorithmSelect->addItem("Пошаговый алгоритм");
    algorithmSelect->addItem("Алгоритм ЦДА");
    algorithmSelect->addItem("Алгоритм Брезенхема (линия)");
    algorithmSelect->addItem("Алгоритм Брезенхема (окружность)");

    x1Spin = new QSpinBox();
    y1Spin = new QSpinBox();
    x2Spin = new QSpinBox();
    y2Spin = new QSpinBox();
    radiusSpin = new QSpinBox();

    x1Spin->setRange(-200, 200);
    y1Spin->setRange(-200, 200);
    x2Spin->setRange(-200, 200);
    y2Spin->setRange(-200, 200);
    radiusSpin->setRange(1, 200);

    x1Spin->setValue(-50);
    y1Spin->setValue(-50);
    x2Spin->setValue(50);
    y2Spin->setValue(50);
    radiusSpin->setValue(50);

    drawButton = new QPushButton("Нарисовать");
    clearButton = new QPushButton("Очистить");

    QVBoxLayout *controlLayout = new QVBoxLayout();

    QHBoxLayout *coordLayout = new QHBoxLayout();
    coordLayout->addWidget(new QLabel("X1:"));
    coordLayout->addWidget(x1Spin);
    coordLayout->addWidget(new QLabel("Y1:"));
    coordLayout->addWidget(y1Spin);
    coordLayout->addWidget(new QLabel("X2:"));
    coordLayout->addWidget(x2Spin);
    coordLayout->addWidget(new QLabel("Y2:"));
    coordLayout->addWidget(y2Spin);
    coordLayout->addWidget(new QLabel("Радиус:"));
    coordLayout->addWidget(radiusSpin);

    controlLayout->addWidget(algorithmSelect);
    controlLayout->addLayout(coordLayout);
    controlLayout->addWidget(drawButton);
    controlLayout->addWidget(clearButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(view);
    mainLayout->addLayout(controlLayout);

    connect(drawButton, &QPushButton::clicked, this, &MainWindow::drawLine);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearScene);

    clearScene();
}

void MainWindow::drawGridNumbers()
{
    for(int i = -200; i <= 200; i += 10) {
        if(i != 0) {
            QGraphicsTextItem *textX = scene->addText(QString::number(i));
            textX->setPos(i * gridSize - 10, 10);
            QGraphicsTextItem *textY = scene->addText(QString::number(i));
            textY->setPos(10, -i * gridSize - 10);
        }
    }
}

void MainWindow::clearScene()
{
    scene->clear();

    QPen gridPen(Qt::lightGray);
    for(int i = -200; i <= 200; i += 10) {
        scene->addLine(i * gridSize, -600, i * gridSize, 600, gridPen);
        scene->addLine(-600, i * gridSize, 600, i * gridSize, gridPen);
    }

    QPen axisPen(Qt::black);
    axisPen.setWidth(2);
    scene->addLine(-600, 0, 600, 0, axisPen);
    scene->addLine(0, -600, 0, 600, axisPen);

    drawGridNumbers();
}

void MainWindow::putPixel(int x, int y, const QColor& color)
{
    QBrush brush(color);
    scene->addRect(x * gridSize, -y * gridSize - gridSize,
                   gridSize, gridSize,
                   QPen(color.darker()), brush);
}

void MainWindow::stepByStepLine(int x1, int y1, int x2, int y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    double steps = std::max(std::abs(dx), std::abs(dy));

    if(steps == 0) {
        putPixel(x1, y1, STEP_BY_STEP_COLOR);
        return;
    }

    double xIncrement = dx / steps;
    double yIncrement = dy / steps;

    double x = x1;
    double y = y1;

    for(int i = 0; i <= steps; i++) {
        putPixel(round(x), round(y), STEP_BY_STEP_COLOR);
        x += xIncrement;
        y += yIncrement;
    }
}

void MainWindow::DDALine(int x1, int y1, int x2, int y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;

    double steps = std::max(std::abs(dx), std::abs(dy));

    if(steps == 0) {
        putPixel(x1, y1, DDA_COLOR);
        return;
    }

    float xIncrement = dx / steps;
    float yIncrement = dy / steps;

    float x = x1;
    float y = y1;

    for(int i = 0; i <= steps; i++) {
        putPixel(round(x), round(y), DDA_COLOR);
        x += xIncrement;
        y += yIncrement;
    }
}

void MainWindow::bresenhamLine(int x1, int y1, int x2, int y2)
{
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while(true) {
        putPixel(x1, y1, BRESENHAM_LINE_COLOR);

        if(x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if(e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if(e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void MainWindow::bresenhamCircle(int xc, int yc, int radius)
{
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while(y >= x) {
        putPixel(xc + x, yc + y, BRESENHAM_CIRCLE_COLOR);
        putPixel(xc - x, yc + y, BRESENHAM_CIRCLE_COLOR);
        putPixel(xc + x, yc - y, BRESENHAM_CIRCLE_COLOR);
        putPixel(xc - x, yc - y, BRESENHAM_CIRCLE_COLOR);
        putPixel(xc + y, yc + x, BRESENHAM_CIRCLE_COLOR);
        putPixel(xc - y, yc + x, BRESENHAM_CIRCLE_COLOR);
        putPixel(xc + y, yc - x, BRESENHAM_CIRCLE_COLOR);
        putPixel(xc - y, yc - x, BRESENHAM_CIRCLE_COLOR);

        if(d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void MainWindow::drawLine()
{
    int algorithm = algorithmSelect->currentIndex();
    int x1 = x1Spin->value();
    int y1 = y1Spin->value();
    int x2 = x2Spin->value();
    int y2 = y2Spin->value();
    int radius = radiusSpin->value();

    switch(algorithm) {
    case 0:
        stepByStepLine(x1, y1, x2, y2);
        break;
    case 1:
        DDALine(x1, y1, x2, y2);
        break;
    case 2:
        bresenhamLine(x1, y1, x2, y2);
        break;
    case 3:
        bresenhamCircle(x1, y1, radius);
        break;
    }
}
