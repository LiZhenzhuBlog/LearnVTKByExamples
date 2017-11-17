#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkCubeSource.h>
#include <vtkAxesActor.h>
#include <vtkTransform.h>
#include <vtkSmartPointer.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
 
int main (int, char*[])
{
  vtkSmartPointer<vtkCubeSource> cubeSource =
    vtkSmartPointer<vtkCubeSource>::New();
  //cubeSource->SetCenter(0.0, 0.0, 0.0);
  cubeSource->SetBounds(-1.0, 1.0,-1.0, 1.0,-1.0, 1.0);

  //create a mapper
  vtkSmartPointer<vtkPolyDataMapper> cubeMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  cubeMapper->SetInputConnection(cubeSource->GetOutputPort());
 
  // create an actor
  vtkSmartPointer<vtkActor> cubeActor =
    vtkSmartPointer<vtkActor>::New();
  cubeActor->SetMapper(cubeMapper);
 
  // a renderer and render window
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize( 1600, 968 );
 
  // an interactor
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  // add the actors to the scene
  renderer->AddActor(cubeActor);
  renderer->SetBackground(.1,.2,.3); // Background dark blue
 
  vtkSmartPointer<vtkTransform> transform =
    vtkSmartPointer<vtkTransform>::New();
  transform->Translate(1.0, 1.0, 1.0);

  vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();

  // The axes are positioned with a user transform
  //axes->SetUserTransform(transform);
  //axes->SetShaftTypeToLine();
  // properties of the axes labels can be set as follows
  // this sets the x axis label to red
  axes->SetTotalLength(3.0,3.0,3.0);

  vtkTextProperty * xTxtProperty = axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty();
  vtkTextProperty * yTxtProperty = axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty();
  vtkTextProperty * zTxtProperty = axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty();
  xTxtProperty->SetColor(1,0,0);
  yTxtProperty->SetColor(0,1,0);
  zTxtProperty->SetColor(0,0,1);
  xTxtProperty->SetFontSize(24); //why no effect at all?
  yTxtProperty->SetFontSize(12);
  zTxtProperty->SetFontFamilyToTimes(); //SetFontFamilyToCourier(); //SetFontFamilyToArial();
  zTxtProperty->SetFontSize(8);
  // the actual text of the axis label can be changed:
  // axes->SetXAxisLabelText("test");

  renderer->AddActor(axes);
 
  renderer->ResetCamera();
  renderWindow->Render();

  // begin mouse interaction
  renderWindowInteractor->Start();
 
  return EXIT_SUCCESS;
}
