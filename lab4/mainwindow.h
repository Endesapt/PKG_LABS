#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QButtonGroup>
#include<QVBoxLayout>
#include "rasterizationwidget.h"
class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow() {
        auto *layout = new QVBoxLayout(this);

        auto *inputLayout = new QHBoxLayout();
        x1Edit = new QLineEdit();
        y1Edit = new QLineEdit();
        x2Edit = new QLineEdit();
        y2Edit = new QLineEdit();
        radiusEdit = new QLineEdit();

        inputLayout->addWidget(new QLabel("X1:"));
        inputLayout->addWidget(x1Edit);
        inputLayout->addWidget(new QLabel("Y1:"));
        inputLayout->addWidget(y1Edit);
        inputLayout->addWidget(new QLabel("X2:"));
        inputLayout->addWidget(x2Edit);
        inputLayout->addWidget(new QLabel("Y2:"));
        inputLayout->addWidget(y2Edit);
        inputLayout->addWidget(new QLabel("Radius:"));
        inputLayout->addWidget(radiusEdit);

        auto *buttonLayout = new QHBoxLayout();
        auto *buttonGroup = new QButtonGroup(this);

        auto *stepButton = new QPushButton("Step by Step");
        auto *ddaButton = new QPushButton("DDA");
        auto *bresenhamLineButton = new QPushButton("Bresenham Line");
        auto *bresenhamCircleButton = new QPushButton("Bresenham Circle");

        buttonGroup->addButton(stepButton);
        buttonGroup->addButton(ddaButton);
        buttonGroup->addButton(bresenhamLineButton);
        buttonGroup->addButton(bresenhamCircleButton);

        buttonLayout->addWidget(stepButton);
        buttonLayout->addWidget(ddaButton);
        buttonLayout->addWidget(bresenhamLineButton);
        buttonLayout->addWidget(bresenhamCircleButton);

        rasterWidget = new RasterizationWidget;

        layout->addLayout(inputLayout);
        layout->addLayout(buttonLayout);
        layout->addWidget(rasterWidget);

        connect(stepButton, &QPushButton::clicked, this, &MainWindow::drawStepByStep);
        connect(ddaButton, &QPushButton::clicked, this, &MainWindow::drawDDA);
        connect(bresenhamLineButton, &QPushButton::clicked, this, &MainWindow::drawBresenhamLine);
        connect(bresenhamCircleButton, &QPushButton::clicked, this, &MainWindow::drawBresenhamCircle);
    }

private slots:
    void drawStepByStep() {
        rasterWidget->setParameters(getX1(), getY1(), getX2(), getY2(), 0, Algorithm::StepByStep);
    }

    void drawDDA() {
        rasterWidget->setParameters(getX1(), getY1(), getX2(), getY2(), 0, Algorithm::DDA);
    }

    void drawBresenhamLine() {
        rasterWidget->setParameters(getX1(), getY1(), getX2(), getY2(), 0, Algorithm::BresenhamLine);
    }

    void drawBresenhamCircle() {
        rasterWidget->setParameters(getX1(), getY1(), 0, 0, getRadius(), Algorithm::BresenhamCircle);
    }

private:
    QLineEdit *x1Edit, *y1Edit, *x2Edit, *y2Edit, *radiusEdit;
    RasterizationWidget *rasterWidget;

    int getX1() const { return x1Edit->text().toInt(); }
    int getY1() const { return y1Edit->text().toInt(); }
    int getX2() const { return x2Edit->text().toInt(); }
    int getY2() const { return y2Edit->text().toInt(); }
    int getRadius() const { return radiusEdit->text().toInt(); }
};
