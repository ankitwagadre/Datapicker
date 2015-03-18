#ifndef DATAPICKER_H
#define DATAPICKER_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>

namespace Ui {
class Datapicker;
}

class CustomScene : public QGraphicsScene
{
    Q_OBJECT

public:
    double x;
    double y;

signals:
    void mouse_Pressed();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

class Datapicker : public QDialog
{
    Q_OBJECT

public:
    explicit Datapicker(QWidget *parent = 0);
    ~Datapicker();


private slots:
    void uploadImage();
    void setFirstPoint();
    void setSecondPoint();
    void setThirdPoint();
    void setAnswerPoint();
    void undo();
    void mousePressed();
    void zoomIn();
    void zoomOut();

private:
    void coordinatesOfPoint(double x,double y);
    void fillTable(double x, double y);

    Ui::Datapicker *ui;
    CustomScene *scene;
    QGraphicsEllipseItem *ellipse[100];
    QGraphicsPixmapItem *item;
    QImage image;
    int pos;
    int firstPointPos;
    int secondPointPos;
    int thirdPointPos;
    int answerPointPos;
    int rowNumber;
    /*coordinates in qgraphicsscene plane*/
    double x1;
    double y1;
    double x2;
    double y2;
    double x3;
    double y3;
    double x4;
    double y4;
};

#endif // DATAPICKER_H
