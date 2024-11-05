#include "clippingwidget.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ClippingWidget widget("lines.txt");
    widget.setWindowTitle("Line Clipping");
    widget.resize(800, 600);
    widget.show();

    return app.exec();
}
