#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>

#include <sstream>

int main(int argc, char* argv[])
{
  #if 0
  // Verify input arguments
  if ( argc != 2 )
  {
    std::cout << "Usage: " << argv[0]
              << " Filename(.PTL)" << std::endl;
    return EXIT_FAILURE;
  }
  // Get all data from the file
  std::string filename = argv[1];
  #endif 

  //std::string filename = "D:\\VTKTest\\THA-210-Trans-CD\\patient\\TSI50001_stem.PTL";
  //std::string filename = "D:\\VTKTest\\THA-210-Trans-CD\\patient\\TSI50001_cup.PTL";
  //std::string filename = "/home/pzhang/Downloads/SouthKoreaCentum/20171018_Centum_DAD00072/patientDat/ng6014RP.CTL"; //implant
  //std::string filename = "/home/pzhang/Downloads/SouthKoreaCentum/20171018_Centum_DAD00072/patientDat/n802702RL.CTL";
  std::string filename = "/home/pzhang/Downloads/SouthKoreaCentum/20171018_Centum_DAD00072/patientDat/DAD00072-Tibia.TTL"; //tiboa bone
  

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

  #if 0
  //add the band registration points manually
  //  -20.2565    -20.7121   1359.1774
  //  -19.7589    -16.4266   1347.8543
  //   -5.2737    -19.5377   1356.6453
  //  -10.4610    -16.6470   1344.2318
  double x, y, z;
  x= -20.2565, y= -20.7121,     z = 1359.1774;
  points->InsertNextPoint(x, y, z);

  x= -19.7589,  y =   -16.4266, z = 1347.8543;
  points->InsertNextPoint(x, y, z);

  x= -5.2737,   y = -19.5377,   z = 1356.6453;
  points->InsertNextPoint(x, y, z);

  x= -10.4610,  y = -16.6470,   z = 1344.2318;
  points->InsertNextPoint(x, y, z);

  {
	  facetCnt++;
	  int vertexCount;
      int  Vertex1=total_points;
      int  Vertex2=total_points + 1;
      int  Vertex3=total_points + 2;
	  triangle->GetPointIds()->SetId(0, Vertex1);
	  triangle->GetPointIds()->SetId(1, Vertex2);
	  triangle->GetPointIds()->SetId(2, Vertex3);
	lan  triangles->InsertNextCell(triangle);
  }

  {
      facetCnt++;
      int vertexCount;
      int  Vertex1=total_points;
      int  Vertex2=total_points + 2;
      int  Vertex3=total_points + 3;
      triangle->GetPointIds()->SetId(0, Vertex1);
      triangle->GetPointIds()->SetId(1, Vertex2);
      triangle->GetPointIds()->SetId(2, Vertex3);
      triangles->InsertNextCell(triangle);
  }
  #endif

  //add registration data
  //  -20.2565,    -20.7121,   1359.1774
  //  -19.7589,    -16.4266,   1347.8543
  //   -5.2737,    -19.5377,   1356.6453
  //  -10.4610,    -16.6470,   1344.2318
  vtkSmartPointer<vtkPoints> targetPoints = vtkSmartPointer<vtkPoints>::New();
  double targetPoint1[3] = { -20.2565,    -20.7121,   1359.1774};
  targetPoints->InsertNextPoint(targetPoint1);
  double targetPoint2[3] = {-19.7589,    -16.4266,   1347.8543};
  targetPoints->InsertNextPoint(targetPoint2);
  double targetPoint3[3] = {-5.2737,    -19.5377,   1356.6453};
  targetPoints->InsertNextPoint(targetPoint3);
  double targetPoint4[3] = {-10.4610,    -16.6470,   1344.2318};
  targetPoints->InsertNextPoint(targetPoint4);

  vtkSmartPointer<vtkPolyData> target = vtkSmartPointer<vtkPolyData>::New();
  target->SetPoints(targetPoints);

  vtkSmartPointer<vtkVertexGlyphFilter> targetGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
  targetGlyphFilter->SetInputData(target);
  targetGlyphFilter->Update();

  vtkSmartPointer<vtkPolyDataMapper> targetMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  targetMapper->SetInputConnection(targetGlyphFilter->GetOutputPort());
  
  // Create a polydata object
  vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

  // Add the geometry and topology to the polydata
  polyData->SetPoints(points);
  polyData->SetPolys(triangles);

  std::cout << "There are " << polyData->GetNumberOfCells() << " cells." << std::endl;

  // Visualize

  vtkSmartPointer<vtkPolyDataMapper> mapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(polyData);

  vtkSmartPointer<vtkActor> actor =  vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  //---------------------------------------------------------------------
  vtkSmartPointer<vtkActor> targetActor =  vtkSmartPointer<vtkActor>::New();
  targetActor->SetMapper(targetMapper);
  targetActor->GetProperty()->SetColor(1,0,0);
  targetActor->GetProperty()->SetPointSize(4);
  //-----------------------------------------------------------------------

  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);
  renderer->AddActor(targetActor);
  renderer->SetBackground(.3, .6, .3); // Background color green

  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
