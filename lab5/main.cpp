#include "clippingwidget.h"
#include "polygonclippingwidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ClippingWidget widget("lines.txt");
    widget.setWindowTitle("Line Clipping");
    widget.resize(800, 600);
    widget.show();

    PolygonClippingWidget polygonWidget("polygon.txt");
    polygonWidget.setWindowTitle("Polygon Clipping");
    polygonWidget.resize(800, 600);
    polygonWidget.show();

    return app.exec();
}
