#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkLandmarkTransform.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkProperty.h>
#include <vtkAxesActor.h>

namespace {
    void CreatePolyData(vtkSmartPointer<vtkPolyData> polydata);
    void PerturbPolyData(vtkSmartPointer<vtkPolyData> polydata);
    void TranslatePolyData(vtkSmartPointer<vtkPolyData> polydata);
}

int main(int argc, char *argv[])
{
    vtkSmartPointer<vtkPolyData> source =
    vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyData> target =
    vtkSmartPointer<vtkPolyData>::New();

    if (argc == 3) {
        std::cout << "Reading data..." << std::endl;
        std::string strSource = argv[1];
        std::string strTarget = argv[2];
        vtkSmartPointer<vtkXMLPolyDataReader> sourceReader =
        vtkSmartPointer<vtkXMLPolyDataReader>::New();
        sourceReader->SetFileName(strSource.c_str());
        sourceReader->Update();
        source->ShallowCopy(sourceReader->GetOutput());

        vtkSmartPointer<vtkXMLPolyDataReader> targetReader =
        vtkSmartPointer<vtkXMLPolyDataReader>::New();
        targetReader->SetFileName(strTarget.c_str());
        targetReader->Update();
        target->ShallowCopy(targetReader->GetOutput());
    } else {
        std::cout << "Creating data..." << std::endl;
        CreatePolyData(source);
        target->ShallowCopy(source);
        TranslatePolyData(target);
        PerturbPolyData(target);
    }

    // Setup ICP transform
    vtkSmartPointer<vtkIterativeClosestPointTransform> icp = 
    vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
    icp->SetSource(source);
    icp->SetTarget(target);
    icp->GetLandmarkTransform()->SetModeToRigidBody();
    icp->SetMaximumNumberOfIterations(20);
    //icp->StartByMatchingCentroidsOn();
    icp->Modified();
    icp->Update();

    // Get the resulting transformation matrix (this matrix takes the source points to the target points)
    vtkSmartPointer<vtkMatrix4x4> m = icp->GetMatrix();
    std::cout << "The resulting matrix is: " << *m << std::endl;

    // Transform the source points by the ICP solution
    vtkSmartPointer<vtkTransformPolyDataFilter> icpTransformFilter =
    vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    icpTransformFilter->SetInputData(source);
    icpTransformFilter->SetTransform(icp);
    icpTransformFilter->Update();

    /*
    // If you need to take the target points to the source points, the matrix is:
    icp->Inverse();
    vtkSmartPointer<vtkMatrix4x4> minv = icp->GetMatrix();
    std::cout << "The resulting inverse matrix is: " << *minv << std::cout;
    */

    // Visualize
    vtkSmartPointer<vtkPolyDataMapper> sourceMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    sourceMapper->SetInputData(source);

    vtkSmartPointer<vtkActor> sourceActor =
    vtkSmartPointer<vtkActor>::New();
    sourceActor->SetMapper(sourceMapper);
    sourceActor->GetProperty()->SetColor(0,1,0);//source is green
    sourceActor->GetProperty()->SetPointSize(4);

    vtkSmartPointer<vtkPolyDataMapper> targetMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    targetMapper->SetInputData(target);

    vtkSmartPointer<vtkActor> targetActor =
    vtkSmartPointer<vtkActor>::New();
    targetActor->SetMapper(targetMapper);
    targetActor->GetProperty()->SetColor(1,0,0); //target is red
    targetActor->GetProperty()->SetPointSize(4);

    vtkSmartPointer<vtkPolyDataMapper> solutionMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    solutionMapper->SetInputConnection(icpTransformFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> solutionActor =
    vtkSmartPointer<vtkActor>::New();
    solutionActor->SetMapper(solutionMapper);
    solutionActor->GetProperty()->SetColor(0,0,1); //solution is blue
    solutionActor->GetProperty()->SetPointSize(3);

    // Create a renderer, render window, and interactor
    vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetSize( 1600, 968 );

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // Add the actor to the scene
    renderer->AddActor(sourceActor);
    renderer->AddActor(targetActor);
    renderer->AddActor(solutionActor);
    renderer->SetBackground(.3, .6, .3); // Background color green

    //add axis
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Translate(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();

    // The axes are positioned with a user transform
    //axes->SetUserTransform(transform);
    //axes->SetShaftTypeToLine();
    // properties of the axes labels can be set as follows
    // this sets the x axis label to red
    axes->SetTotalLength(3.0,3.0,3.0);

    #if 0
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
    #endif

    renderer->AddActor(axes);

    // Render and interact
    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}

namespace { // anonymous

    #if dataPreparation
    source  (4 points = origin + unit distance on xyz axis) [green]
    ====>
    target =  source * [0, .3, 0] [red]
    ===> add noise [0, 0.1] to target
    source + target == ICP ==> tranformation Matrix
    source + icp->matrix   ==> solution (should pretty close to target points)
    #endif

    void CreatePolyData(vtkSmartPointer<vtkPolyData> polydata)
    {
        // This function creates a set of 4 points (the origin and a point unit distance along each axis)

        vtkSmartPointer<vtkPoints> points =
        vtkSmartPointer<vtkPoints>::New();

        // Create points
        double origin[3] = {0.0, 0.0, 0.0};
        points->InsertNextPoint(origin);
        double p1[3] = {1.0, 0.0, 0.0};
        points->InsertNextPoint(p1);
        double p2[3] = {0.0, 1.0, 0.0};
        points->InsertNextPoint(p2);
        double p3[3] = {0.0, 0.0, 1.0};
        points->InsertNextPoint(p3);

        vtkSmartPointer<vtkPolyData> temp = 
        vtkSmartPointer<vtkPolyData>::New();
        temp->SetPoints(points);

        //why need this filter. point != vertexFillter output?
        vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
        vertexFilter->SetInputData(temp);
        vertexFilter->Update();

        polydata->ShallowCopy(vertexFilter->GetOutput());
    }

    void PerturbPolyData(vtkSmartPointer<vtkPolyData> polydata)
    {
        vtkSmartPointer<vtkPoints> points =
        vtkSmartPointer<vtkPoints>::New();
        points->ShallowCopy(polydata->GetPoints());

        for (vtkIdType i = 0; i < points->GetNumberOfPoints(); i++) {
            double p[3];
            points->GetPoint(i, p);
            double perturb[3];
            if (i%3 == 0) {
                perturb[0] = .1; perturb[1] = 0; perturb[2] = 0;
            } else if (i%3 == 1) {
                perturb[0] = 0; perturb[1] = .1; perturb[2] = 0;
            } else {
                perturb[0] = 0; perturb[1] = 0; perturb[2] = .1;
            }

            for (unsigned int j = 0; j < 3; j++) {
                p[j] += perturb[j];
            }
            points->SetPoint(i, p);
        }

        polydata->SetPoints(points);

    }

    void TranslatePolyData(vtkSmartPointer<vtkPolyData> polydata)
    {
        vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
        transform->Translate(0,.3,0);

        vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
        vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        transformFilter->SetInputData(polydata);
        transformFilter->SetTransform(transform);
        transformFilter->Update();

        polydata->ShallowCopy(transformFilter->GetOutput());
    }

} // end anonymous namespace
