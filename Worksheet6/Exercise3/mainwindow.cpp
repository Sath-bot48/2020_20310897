#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkPolyData.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QFileDialog>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkLight.h>
#include <vtkPlane.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

        // standard call to setup Qt UI (same as previously)
	ui->setupUi(this);

	connect(ui->ResetCamera, &QPushButton::released, this, &MainWindow::handleResetCamera);

	// Now need to create a VTK render window and link it to the QtVTK widget
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	ui->qvtkWidget->SetRenderWindow(renderWindow);   // note that vtkWidget is the name I gave to my QtVTKOpenGLWidget in Qt creator

	// Now use the VTK libraries to render something. To start with you can copy-paste the cube example code, there are comments to show where the code must be modified.
	//--------------------------------------------- Code From Example 1 --------------------------------------------------------------------------

        // Open File
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open STL File"), "./", tr("STL Files(*.stl)"));
        std::string str = fileName.toUtf8().constData();
        reader = vtkSmartPointer<vtkSTLReader>::New();
        reader->SetFileName(str.data());
        reader->Update();

        mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        mapper->SetInputConnection(reader->GetOutputPort());

	// Create an actor that is used to set the cube's properties for rendering and place it in the window
	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOn();

	colors = vtkSmartPointer<vtkNamedColors>::New();
	actor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());

	// Create a renderer, and render window
	renderer = vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();  // ###### We've already created the renderWindow this time ######
        ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);                                   // ###### ask the QtVTKOpenGLWidget for its renderWindow ######

	// Link a renderWindowInteractor to the renderer (this allows you to capture mouse movements etc)  ###### Not needed with Qt ######
	//vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//renderWindowInteractor->SetRenderWindow( ui->vtkWidget->GetRenderWindow() );    

        // Render Light
	light = vtkSmartPointer<vtkLight>::New();
	light->SetLightTypeToSceneLight();
	light->SetPosition(5, 5, 15);
	light->SetPositional(true);
	light->SetConeAngle(10);
	light->SetFocalPoint(0, 0, 0);
	light->SetDiffuseColor(1, 1, 1);
	light->SetAmbientColor(1, 1, 1);
	light->SetSpecularColor(1, 1, 1);
	light->SetIntensity(0.5);

	// Add the actor to the scene
	renderer->AddActor(actor);
	renderer->AddLight(light);
	renderer->SetBackground(colors->GetColor3d("Silver").GetData());

	// Setup the renderers's camera
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCameraClippingRange();

	// Render and interact
        // renderWindow->Render();            // ###### Not needed with Qt ######
        // renderWindowInteractor->Start();   // ###### Not needed with Qt ######
	//--------------------------------------------- /Code From Example 1 -------------------------------------------------------------------------*/
}

void MainWindow::on_actionFileOpen_triggered() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open STL File"), "./", tr("STL Files(*.stl)"));
	std::string str = fileName.toUtf8().constData();
	reader->SetFileName(str.data());
	reader->Update();
	renderer->ResetCamera();
	ui->qvtkWidget->GetRenderWindow()->Render();
}

// Clip Filter --------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_clipfilter_clicked(bool checked) {
        if (checked) {

                planeLeft->SetOrigin(0.0, 0.0, 0.0);
		planeLeft->SetNormal(-1.0, 0.0, 0.0);
		clipfilter->SetInputConnection(reader->GetOutputPort());
		clipfilter->SetClipFunction(planeLeft.Get());
                mapper->SetInputConnection(clipfilter->GetOutputPort());
                filter_number(1);
        }

	ui->qvtkWidget->GetRenderWindow()->Render();
}

// Shrink filter --------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_shrinkfilter_clicked(bool checked) {
	if (checked) {

		shrinkfilter->SetInputConnection(reader->GetOutputPort());
		shrinkfilter->SetShrinkFactor(.8);
		shrinkfilter->Update();
		mapper->SetInputConnection(shrinkfilter->GetOutputPort());
		filter_number(2);
	}

	ui->qvtkWidget->GetRenderWindow()->Render();
}

// Reset -----------------------------------------------------------------------------------------------------------------------------------
void MainWindow::handleResetCamera() {

        renderer->ResetCamera();
	mapper->SetInputConnection(reader->GetOutputPort());
	filter_number(3);
	//light->SetIntensity(0.5);

	ui->qvtkWidget->GetRenderWindow()->Render();
}

// If click one box cant click another box -------------------------------------------------------------------------------------------------
void MainWindow::filter_number(int numeberCheckbox) {
        switch (numeberCheckbox) {

        case 1: ui->shrinkfilter->setChecked(false);
		break;
	case 2: ui->clipfilter->setChecked(false);
		break;
	case 3: ui->clipfilter->setChecked(false);
		ui->shrinkfilter->setChecked(false);
		light->SetIntensity(0.5);
		break;
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}
