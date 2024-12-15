// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QColor>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void drawLine();
    void clearScene();

private:
    void setupUI();
    void stepByStepLine(int x1, int y1, int x2, int y2);
    void DDALine(int x1, int y1, int x2, int y2);
    void bresenhamLine(int x1, int y1, int x2, int y2);
    void bresenhamCircle(int xc, int yc, int radius);
    void putPixel(int x, int y, const QColor& color);
    void drawGridNumbers();
    QColor getCurrentAlgorithmColor();

    QGraphicsScene *scene;
    QGraphicsView *view;
    QComboBox *algorithmSelect;
    QSpinBox *x1Spin, *y1Spin, *x2Spin, *y2Spin, *radiusSpin;
    QPushButton *drawButton, *clearButton;
    int gridSize;

    // Цвета для разных алгоритмов
    const QColor STEP_BY_STEP_COLOR = QColor(255, 0, 0);    // Красный
    const QColor DDA_COLOR = QColor(0, 255, 0);             // Зеленый
    const QColor BRESENHAM_LINE_COLOR = QColor(0, 0, 255);  // Синий
    const QColor BRESENHAM_CIRCLE_COLOR = QColor(128, 0, 128); // Фиолетовый
};

#endif
