//==================================================
/*!
  \file   TlToVtk.cpp
  \author Saurabh Jain <sjain@thinksurgical.com>
  \date   Sat Nov 07 07:35:02 2015

  \brief  surface converter tl --> vtk

*/
//==================================================
#include <iostream>
#include <string>
#include <vtkPolyDataWriter.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>

//--------------------------------------------------
int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " -a|-b <input> <output> " << std::endl << std::endl;
        std::cerr << "       -a, --ascii  : write in ASCII format " << std::endl;
        std::cerr << "       -b, --binary : write in binary format " << std::endl;
        return 1;
    }

    char pcLine[256];
    pcLine[0] = 0; //needed for initialization

    std::ifstream fip;
    int nPoints=0, nPolys=0, temp;

    fip.open(argv[2]);
    if(!fip.is_open())
    {
        std::cout << "Error: Cannot Open Input File!\n";
        return 1;
    }

    // Read past to the header.
    while (pcLine[0] != '*')
    {
        fip.getline(pcLine, 256);

        if (fip.eof())
            return 1;
    } // end while


    // Skip one more line that is after * or **.
    fip.getline(pcLine, 256);

    fip >> nPoints >> nPolys >> temp;
    if(nPoints==0 || nPolys==0)
        return 1;

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints(nPoints);

    // Read points
    for (vtkIdType i = 0; i < nPoints; i++)
    {
        double x, y, z;
        fip >> x >> y >> z;

        if(fip.eof())
            return 1;

        points->SetPoint(i, x, y, z);
    }

    // Read triangles
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    for (vtkIdType i = 0; i < nPolys; i++)
    {
        fip >> temp; // read number of vertices (always 3 so ignored)
        vtkIdType a, b, c;
        fip >> a >> b >> c;
        polys->InsertNextCell(3);
        polys->InsertCellPoint(a);
        polys->InsertCellPoint(b);
        polys->InsertCellPoint(c);
        fip >> temp; // read color (ignored)

        if(fip.eof())//should not end, return false
            return 1;
    }

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);
    polydata->SetPolys(polys);
    vtkSmartPointer<vtkPolyDataWriter> writer = vtkPolyDataWriter::New();
#if VTK_MAJOR_VERSION <= 5
    writer->SetInput(polydata);
#else
    writer->SetInputData(polydata);
#endif
    writer->SetFileName(argv[3]);
    writer->SetFileType((strncmp(argv[1], "-b", 2) == 0 || strncmp(argv[1], "--binary", 8) == 0) ? VTK_BINARY : VTK_ASCII);
    writer->Write();


    return 0;
}
