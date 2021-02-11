#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkCamera.h>
#include <vtkPolyData.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLight.h>
#include <vtkPlane.h>
#include <vtkShrinkFilter.h>
#include <vtkClipDataSet.h>
#include <vtkSTLReader.h>

namespace Ui {
     class MainWindow;
}

     class MainWindow : public QMainWindow {
         Q_OBJECT

     public:
         explicit MainWindow(QWidget *parent = 0);
         ~MainWindow();

         vtkSmartPointer<vtkDataSetMapper> mapper;
         vtkSmartPointer<vtkActor> actor;
         vtkSmartPointer<vtkNamedColors> colors;
         vtkSmartPointer<vtkRenderer> renderer;
         vtkSmartPointer<vtkLight> light;
         vtkSmartPointer<vtkSTLReader>reader;

     private slots:
         void on_actionFileOpen_triggered();
         void on_clipfilter_clicked(bool checked);
         void on_shrinkfilter_clicked(bool checked);
         void filter_number(int CheckBoxname);
         void handleResetCamera();

     private:
         Ui::MainWindow *ui;
         vtkSmartPointer<vtkClipDataSet> clipfilter = vtkSmartPointer<vtkClipDataSet>::New();
         vtkSmartPointer<vtkShrinkFilter> shrinkfilter = vtkSmartPointer<vtkShrinkFilter>::New();
         vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
};

#endif // MAINWINDOW_H

