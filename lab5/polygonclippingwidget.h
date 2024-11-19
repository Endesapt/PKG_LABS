#include <QWidget>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#ifndef POLYGONCLIPPINGWIDGET_H
#define POLYGONCLIPPINGWIDGET_H
class PolygonClippingWidget : public QWidget {
    QVector<QPointF> polygon;
    QRectF clipRect;

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        int gridSize = 20;
        painter.setPen(QColor(220, 220, 220));
        for (int x = 0; x < width(); x += gridSize) {
            painter.drawLine(x, 0, x, height());
        }
        for (int y = 0; y < height(); y += gridSize) {
            painter.drawLine(0, y, width(), y);
        }

        painter.setPen(Qt::red);
        QRectF scaledClipRect(clipRect.topLeft() * gridSize, clipRect.bottomRight() * gridSize);
        painter.drawRect(scaledClipRect);

        painter.setPen(Qt::blue);
        QPolygonF scaledPolygon;
        for (const auto &point : polygon) {
            scaledPolygon << point * gridSize;
        }
        painter.drawPolygon(scaledPolygon);

        painter.setPen(Qt::green);
        QPolygonF clippedPolygon = sutherlandHodgmanClip(polygon, clipRect);
        QPolygonF scaledClippedPolygon;
        for (const auto &point : clippedPolygon) {
            scaledClippedPolygon << point * gridSize;
        }
        painter.drawPolygon(scaledClippedPolygon);
    }

    QPolygonF sutherlandHodgmanClip(const QVector<QPointF> &subjectPolygon, const QRectF &clipRect) {
        QPolygonF outputList = subjectPolygon;
        QVector<QLineF> clipEdges = {
            QLineF(clipRect.topLeft(), clipRect.topRight()),
            QLineF(clipRect.topRight(), clipRect.bottomRight()),
            QLineF(clipRect.bottomRight(), clipRect.bottomLeft()),
            QLineF(clipRect.bottomLeft(), clipRect.topLeft())
        };

        for (const auto &clipEdge : clipEdges) {
            QPolygonF inputList = outputList;
            outputList.clear();

            for (int i = 0; i < inputList.size(); ++i) {
                QPointF current = inputList[i];
                QPointF prev = inputList[(i + inputList.size() - 1) % inputList.size()];

                bool currentInside = (clipEdge.p2().x() - clipEdge.p1().x()) * (current.y() - clipEdge.p1().y()) -
                                         (clipEdge.p2().y() - clipEdge.p1().y()) * (current.x() - clipEdge.p1().x()) >= 0;
                bool prevInside = (clipEdge.p2().x() - clipEdge.p1().x()) * (prev.y() - clipEdge.p1().y()) -
                                      (clipEdge.p2().y() - clipEdge.p1().y()) * (prev.x() - clipEdge.p1().x()) >= 0;

                if (currentInside) {
                    if (!prevInside) {
                        outputList << intersect(prev, current, clipEdge.p1(), clipEdge.p2());
                    }
                    outputList << current;
                } else if (prevInside) {
                    outputList << intersect(prev, current, clipEdge.p1(), clipEdge.p2());
                }
            }
        }

        return outputList;
    }

    QPointF intersect(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4) {
        float a1 = p2.y() - p1.y();
        float b1 = p1.x() - p2.x();
        float c1 = a1 * p1.x() + b1 * p1.y();

        float a2 = p4.y() - p3.y();
        float b2 = p3.x() - p4.x();
        float c2 = a2 * p3.x() + b2 * p3.y();

        float det = a1 * b2 - a2 * b1;
        if (det == 0) {
            return QPointF(0, 0); // Линии параллельны
        } else {
            float x = (b2 * c1 - b1 * c2) / det;
            float y = (a1 * c2 - a2 * c1) / det;
            return QPointF(x, y);
        }
    }

public:
    PolygonClippingWidget(const QString &fileName, QWidget *parent = nullptr) : QWidget(parent) {
        loadFromFile(fileName);
    }

    void loadFromFile(const QString &fileName) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Cannot open file:" << fileName;
            return;
        }

        QTextStream in(&file);
        int n;
        in >> n;
        for (int i = 0; i < n; ++i) {
            float x, y;
            in >> x >> y;
            polygon.append(QPointF(x, y));
        }

        float xmin, ymin, xmax, ymax;
        in >> xmin >> ymin >> xmax >> ymax;
        clipRect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));

        file.close();
    }
};
#endif // POLYGONCLIPPINGWIDGET_H
