#include "rasterizationwidget.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *label = new QLabel("Растровые алгоритмы");
    label->setAlignment(Qt::AlignCenter);

    RasterizationWidget *rasterWidget = new RasterizationWidget;

    layout->addWidget(label);
    layout->addWidget(rasterWidget);

    window.setLayout(layout);
    window.setWindowTitle("Растровые алгоритмы");
    window.show();

    return app.exec();
}
