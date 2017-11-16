#include <vtkSmartPointer.h>
//#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>

int main(int, char *[])
{
  // Create the polydata geometry
  #if 0
  vtkSmartPointer<vtkSphereSource> sphereSource =
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  #endif

  vtkSmartPointer<vtkCubeSource> CubeSource =
    vtkSmartPointer<vtkCubeSource>::New();
  CubeSource->Update();

  // Set up the actor to display the untransformed polydata
 
  vtkSmartPointer<vtkPolyDataMapper> originalMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  originalMapper->SetInputConnection(CubeSource->GetOutputPort());

  vtkSmartPointer<vtkActor> originalActor =
    vtkSmartPointer<vtkActor>::New();
  originalActor->SetMapper(originalMapper);
  originalActor->GetProperty()->SetColor(0,1,0); //original[green]

  // Set up the transform filter

  vtkSmartPointer<vtkTransform> translation =
    vtkSmartPointer<vtkTransform>::New();
  translation->Translate(1.0, 2.0, 3.0);

  vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
    vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transformFilter->SetInputConnection(CubeSource->GetOutputPort());
  transformFilter->SetTransform(translation);
  transformFilter->Update();

  // Set up the actor to display the transformed polydata

  vtkSmartPointer<vtkPolyDataMapper> transformedMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  transformedMapper->SetInputConnection(transformFilter->GetOutputPort());

  vtkSmartPointer<vtkActor> transformedActor =
    vtkSmartPointer<vtkActor>::New();
  transformedActor->SetMapper(transformedMapper);
  transformedActor->GetProperty()->SetColor(1,0,0); //target [red]

  //set up display framework
  //Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize( 1000, 800 );

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  //display axes
  vtkSmartPointer<vtkAxesActor> axes =
    vtkSmartPointer<vtkAxesActor>::New();

  vtkSmartPointer<vtkOrientationMarkerWidget> widget = 
    vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  widget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
  widget->SetOrientationMarker( axes );
  widget->SetInteractor( renderWindowInteractor );
  widget->SetViewport( 0.0, 0.0, 0.5, 0.5 );
  widget->SetEnabled( 1 );
  widget->InteractiveOn();

  // Set up the rest of the visualization pipeline

  renderer->AddActor(originalActor);
  renderer->AddActor(transformedActor);
  renderer->SetBackground(.3, .6, .3); // Set renderer's background color to green

  //start the pipeline
  renderWindowInteractor->Start();
  
  return EXIT_SUCCESS;
}
