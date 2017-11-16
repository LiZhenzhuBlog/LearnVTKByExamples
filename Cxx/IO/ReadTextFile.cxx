#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>

#include <sstream>

int main(int argc, char* argv[])
{
  #if 0
  // Verify input arguments
  if ( argc != 2 )
  {
    std::cout << "Usage: " << argv[0]
              << " Filename(.xyz)" << std::endl;
    return EXIT_FAILURE;
  }
  // Get all data from the file
  std::string filename = argv[1];
  #endif 

  std::string filename = "D:\\VTKTest\\THA-210-Trans-CD\\patient\\TSI50001_stem.PTL";
  //std::string filename = "D:\\VTKTest\\THA-210-Trans-CD\\patient\\TSI50001_cup.PTL";
  std::ifstream filestream(filename.c_str());

  //skip the header info part
  std::string line;
  while (std::getline(filestream, line)) {
	  if (line[0] == '*') break;
  }
  
  //skip the dummy line
  std::getline(filestream, line);

  //read total points number
  std::getline(filestream, line);
  double total_points, total_facets, total_vertices_on_facets;
  std::stringstream lineInfo;
  lineInfo << line;
  lineInfo >> total_points >> total_facets >> total_vertices_on_facets;

  cout << "total_points=[" << total_points << "], total_facets =[" << total_facets << "], total_vertices_on_facets=[" << total_vertices_on_facets << "]" << endl;
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();
  double pointCnt = 0;

  while(pointCnt < total_points && std::getline(filestream, line))
  {
	pointCnt++;
    double x, y, z;
    std::stringstream linestream;
    linestream << line;
    linestream >> x >> y >> z;

    points->InsertNextPoint(x, y, z);
    //cout << "[" << x << "," << y << "," << z << "]" << endl;
  }

  //read triangle data
  vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
  vtkSmartPointer<vtkTriangle> triangle   =
    vtkSmartPointer<vtkTriangle>::New();
  double facetCnt = 0;
  while (facetCnt < total_facets && std::getline(filestream, line))
  {
	  facetCnt++;
	  int vertexCount, Vertex1, Vertex2, Vertex3, Color;
	  std::stringstream linestream;
	  linestream << line;
	  linestream >> vertexCount >> Vertex1 >> Vertex2 >> Vertex3 >> Color;

	  triangle->GetPointIds()->SetId(0, Vertex1);
	  triangle->GetPointIds()->SetId(1, Vertex2);
	  triangle->GetPointIds()->SetId(2, Vertex3);
	  triangles->InsertNextCell(triangle);
  }

  //close the file stream
  filestream.close();

  // Create a polydata object
  vtkSmartPointer<vtkPolyData> polyData =
    vtkSmartPointer<vtkPolyData>::New();

  // Add the geometry and topology to the polydata
  polyData->SetPoints(points);
  polyData->SetPolys(triangles);

  std::cout << "There are " << polyData->GetNumberOfCells() << " cells." << std::endl;

  // Visualize

  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(polyData);

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);
  renderer->SetBackground(.3, .6, .3); // Background color green

  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
