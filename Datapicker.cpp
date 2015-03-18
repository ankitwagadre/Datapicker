#include "Datapicker.h"
#include "ui_Datapicker.h"

#include <math.h>

#include <QFileDialog>
#include <QTableWidgetItem>
#include <QTableWidget>

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->x = event->scenePos().x();
    this->y = event->scenePos().y();
    emit mouse_Pressed();
}

Datapicker::Datapicker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Datapicker)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(100);
    ui->tableWidget->setColumnCount(2);
    connect(ui->setFirstPointButton,SIGNAL(clicked()),this,SLOT(setFirstPoint()));
    connect(ui->setSecondPointButton,SIGNAL(clicked()),this,SLOT(setSecondPoint()));
    connect(ui->setThirdPointButton,SIGNAL(clicked()),this,SLOT(setThirdPoint()));
    connect(ui->zoomInButton,SIGNAL(clicked()),this,SLOT(zoomIn()));
    connect(ui->zoomOutButton,SIGNAL(clicked()),this,SLOT(zoomOut()));
    connect(ui->setPointButton,SIGNAL(clicked()),this,SLOT(setAnswerPoint()));
    connect(ui->undoButton,SIGNAL(clicked()),this,SLOT(undo()));
    connect(ui->uploadImageButton,SIGNAL(clicked()),this,SLOT(uploadImage()));
}

Datapicker::~Datapicker()
{
    delete ui;
}

void Datapicker::uploadImage()
{
    scene = new CustomScene;
    ui->graphicsView->setScene(scene);
    ui->tableWidget->clearContents();
    pos = 1;
    rowNumber = 0;
    firstPointPos = 0;
    secondPointPos = 0;
    thirdPointPos = 0;
    answerPointPos = 0;
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.jpeg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Images"), "/home", tr("Image Files (*.png *.jpg *.bmp *jpeg)"));
    if (!fileName.isEmpty()){
        image.load(fileName);
        QPixmap pixmap = QPixmap::fromImage(image);
        item = new QGraphicsPixmapItem;
        item = scene->addPixmap(pixmap);
        scene->setSceneRect(-pixmap.width()/2, -pixmap.height()/2, pixmap.width(), pixmap.height());
        item->setPos(-pixmap.width()/2, -pixmap.height()/2);
        connect(scene,SIGNAL(mouse_Pressed()),this,SLOT(mousePressed()));
        ui->zoomInButton->setEnabled(true);
        ui->zoomOutButton->setEnabled(true);
    }
}

void Datapicker::setFirstPoint()
{
    if(firstPointPos){
        scene->removeItem(ellipse[firstPointPos]);
    }
    firstPointPos = pos;
}

void Datapicker::setSecondPoint()
{
    if(secondPointPos){
        scene->removeItem(ellipse[secondPointPos]);
    }
    secondPointPos = pos;
}

void Datapicker::setThirdPoint()
{
    if(thirdPointPos){
        scene->removeItem(ellipse[thirdPointPos]);
    }
    thirdPointPos = pos;
}

void Datapicker::setAnswerPoint()
{
    if(firstPointPos && secondPointPos && thirdPointPos){
        answerPointPos = pos;
    }
}

void Datapicker::undo()
{
    if(firstPointPos == pos){
        firstPointPos = 0;
    }else if(secondPointPos == pos){
        secondPointPos = 0;
    }else if(thirdPointPos == pos){
        thirdPointPos = 0;
    }
    if(pos > 1){
        scene->removeItem(ellipse[--pos]);
        ui->tableWidget->removeRow(--rowNumber);
    }
}

void Datapicker::mousePressed()
{
    QBrush redBrush(Qt::red);
    QBrush blackBrush(Qt::black);
    QPen blackpen(Qt::black);
    blackpen.setWidth(1);
    if(firstPointPos == pos){
        ellipse[pos++] = scene->addEllipse(scene->x,scene->y,5.0,5.0,blackpen,blackBrush);
        x1 = scene->x;
        y1 = scene->y;
    }else if(secondPointPos == pos){
        ellipse[pos++] = scene->addEllipse(scene->x,scene->y,5.0,5.0,blackpen,blackBrush);
        x2 = scene->x;
        y2 = scene->y;
    }else if(thirdPointPos == pos){
        ellipse[pos++] = scene->addEllipse(scene->x,scene->y,5.0,5.0,blackpen,blackBrush);
        x3 = scene->x;
        y3 = scene->y;
    }else if(answerPointPos == pos){
        ellipse[pos++] = scene->addEllipse(scene->x,scene->y,5.0,5.0,blackpen,redBrush);
        x4 = scene->x;
        y4 = scene->y;
        coordinatesOfPoint(x4,y4);
    }
}

void Datapicker::zoomIn()
{
    ui->graphicsView->scale(1.25, 1.25);
}

void Datapicker::zoomOut()
{
    ui->graphicsView->scale(1/1.25, 1/1.25);
}

void Datapicker::coordinatesOfPoint(double x, double y)
{
    /*references "http://en.wikipedia.org/wiki/Transformation_matrix" */
    double X1 = ui->valueBoxOfX1->value();
    double Y1 = ui->valueBoxOfY1->value();
    double X2 = ui->valueBoxOfX2->value();
    double Y2 = ui->valueBoxOfY2->value();
    double X3 = ui->valueBoxOfX3->value();
    double Y3 = ui->valueBoxOfY3->value();
    double tan;
    double sin;
    double cos;
    double scaleOfX;
    double scaleOfY;
    if(((y2 - y1)*(X3 - X1) - (y3 - y1)*(X2 - X1))!=0){
        tan = ((x2 - x1)*(X3 - X1) - (x3 - x1)*(X2 - X1))/((y2 - y1)*(X3 - X1) - (y3 - y1)*(X2 - X1));
        sin = tan/sqrt(1 + tan*tan);
        cos = sqrt(1 - sin*sin);
    }else{
        sin=1;
        cos=0;
    }
    if((X2 - X1)!=0){
        scaleOfX = (X2 - X1)/((x2 - x1)*cos - (y2 - y1)*sin);
    }else{
        scaleOfX = (X3 - X1)/((x3 - x1)*cos - (y3 - y1)*sin);
    }
    if((Y2-Y1)!=0){
        scaleOfY = (Y2 - Y1)/((x2 - x1)*sin + (y2 - y1)*cos);
    }else{
        scaleOfY = (Y3 - Y1)/((x3 - x1)*sin + (y3 - y1)*cos);
    }
    double X4 = X1 + (((x - x1)*cos - (y - y1)*sin)*scaleOfX);
    double Y4 = Y1 + (((x - x1)*sin + (y - y1)*cos)*scaleOfY);
    fillTable(X4,Y4);
}

/*temporary widget will be replaced by spreadsheet of labplot*/
void Datapicker::fillTable(double x, double y)
{
    QTableWidgetItem *valueOfX = new QTableWidgetItem;
    QTableWidgetItem *valueOfY = new QTableWidgetItem;
    valueOfX->setText(QString::number(x));
    valueOfY->setText(QString::number(y));
    ui->tableWidget->setItem(rowNumber,0,valueOfX);
    ui->tableWidget->setItem(rowNumber,1,valueOfY);
    rowNumber++;
}
