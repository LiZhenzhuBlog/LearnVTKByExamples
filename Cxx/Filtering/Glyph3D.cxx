#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkGlyph3D.h>
#include <vtkCellArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

int main(int, char *[])
{
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  points->InsertNextPoint(0,0,0);
  points->InsertNextPoint(10,10,10);
  points->InsertNextPoint(20,20,20);
  vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(points);
 
  #if 0
  vtkSmartPointer<vtkPolyData> glyph    = vtkSmartPointer<vtkPolyData>::New();
  #endif

  // Create anything you want here, we will use a cube for the demo.
  //-vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
  vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->SetRadius(1);
  vtkSmartPointer<vtkGlyph3D> glyph3D = vtkSmartPointer<vtkGlyph3D>::New();
  //glyph3D->SetSourceConnection(cubeSource->GetOutputPort());
  glyph3D->SetSourceConnection(sphereSource->GetOutputPort());
  glyph3D->SetInputData(polydata);
  glyph3D->Update();
    
  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(glyph3D->GetOutputPort());
 
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
 
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =  vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  renderer->AddActor(actor);
  renderer->SetBackground(.3, .6, .3); // Background color green
 
  renderWindow->Render();
  renderWindowInteractor->Start();
 
  return EXIT_SUCCESS;
}
