/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QDebug>

// SlicerQt includes
#include "qSlicerPancreaticLUSModuleWidget.h"
#include "ui_qSlicerPancreaticLUSModuleWidget.h"

// CTK includes
#include "ctkButtonGroup.h"
#include "ctkPushButton.h"


//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPancreaticLUSModuleWidgetPrivate: public Ui_qSlicerPancreaticLUSModuleWidget
{
public:
  qSlicerPancreaticLUSModuleWidgetPrivate();
  //void setupUi(qSlicerWidget* widget);
};

//-----------------------------------------------------------------------------
// qSlicerPancreaticLUSModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerPancreaticLUSModuleWidgetPrivate::qSlicerPancreaticLUSModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
//void qSlicerPancreaticLUSModuleWidgetPrivate::setupUi(qSlicerWidget *widget)
//{
//}

//-----------------------------------------------------------------------------
// qSlicerPancreaticLUSModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerPancreaticLUSModuleWidget::qSlicerPancreaticLUSModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerPancreaticLUSModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerPancreaticLUSModuleWidget::~qSlicerPancreaticLUSModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerPancreaticLUSModuleWidget::setup()
{
  Q_D(qSlicerPancreaticLUSModuleWidget);
  d->setupUi(this);

  connect(d->pushButton,SIGNAL(clicked()),this,SLOT(generateVolume()));
  connect(d->openFileButton,SIGNAL(clicked()),this,SLOT(getFileName()));
  connect(d->openTSVFileButton,SIGNAL(clicked()),this,SLOT(getTSVFileName()));
  d->progressBar->setValue(0);
  d->nearestRadioButton->setChecked(true);
  this->imagesToProcess = 0;
  this->interpolationMethod = 0;
  this->currentImage = vtkSmartPointer<vtkImageData>::New();
  this->positions = vtkSmartPointer<vtkPoints>::New();
  this->directions = vtkSmartPointer<vtkPoints>::New();
  this->pngReader = vtkSmartPointer<vtkPNGReader>::New();
  for(int i = 0; i < 6; i++)
  {
      this->outputExtent[i] = 0;
  }
  this->ReconstructedVolume = vtkSmartPointer<vtkImageData>::New();
  this->ReconstructedVolume->SetSpacing(1.0,1.0,1.0);
  this->transformImageToReference = vtkSmartPointer<vtkMatrix4x4>::New();
  this->sliceAdder = new InsertSlice;

  this->settings = new QSettings("pancreaticLUSModule","Settings");

  loadSettings();

  this->Superclass::setup();
}

void qSlicerPancreaticLUSModuleWidget::generateVolume()
{
 //   clearData();
    Q_D(qSlicerPancreaticLUSModuleWidget);

    d->progressBar->setValue(0);
    this->blockSignals(true);

    trackerParser *parser = new trackerParser(this);
    parser->parseTSVFile(this->tsvFilePath,this->positions,this->directions);
    delete parser;
    qDebug() << "number of positions read" << this->positions->GetNumberOfPoints();

    if(this->positions->GetNumberOfPoints() < 1 || this->directions->GetNumberOfPoints() < 1)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","No valid positions in .tsv file");
        return;
    }

    this->imagesToProcess = d->numberOfImagesSpinBox->value();
    if(this->imagesToProcess > this->positions->GetNumberOfPoints() || this->imagesToProcess > this->directions->GetNumberOfPoints())
        this->imagesToProcess = this->positions->GetNumberOfPoints();

    if(d->nearestRadioButton->isChecked()) this->interpolationMethod = 0;
    else if(d->trilinearRadioButton->isChecked()) this->interpolationMethod = 1;

    setOutputExtent();
    d->progressBar->setValue(25);

    int progress = 25;
    for(int i = 0; i < this->imagesToProcess; i++)
    {
   //     qDebug() << "Processing image number" << i;
        this->currentImage = readPNGImages(this->folderPath, i);
        if(this->currentImage == 0) return;
        this->sliceAdder->pasteSlice(currentImage,transformImageToReference,interpolationMethod);
        progress = 25 + 75*i/(this->imagesToProcess);
   //     qDebug() << "progress" << progress;
        d->progressBar->setValue(progress);
    }
    d->progressBar->setValue(100);
    this->ReconstructedVolume = this->sliceAdder->getReconstructedVolume();
/*
    vtkSmartPointer<vtkImageExtractComponents> extract = vtkSmartPointer<vtkImageExtractComponents>::New();
    extract->SetComponents(0);
    extract->SetInputData(this->ReconstructedVolume);
    extract->Update();
    vtkSmartPointer<vtkImageData> extractionData = vtkSmartPointer<vtkImageData>::New();
    extractionData->DeepCopy(extract->GetOutput());

    vtkNew<vtkMRMLScalarVolumeNode> volumeNode;
    volumeNode->SetAndObserveImageData(extractionData);
    volumeNode->SetOrigin(this->ReconstructedVolume->GetOrigin());
    qDebug() << this->ReconstructedVolume->GetOrigin()[0];
    volumeNode->SetSpacing(this->ReconstructedVolume->GetSpacing());

    //vtkSmartPointer<vtkMRMLScene> mrmlScene = vtkSmartPointer<vtkMRMLScene>::New();
    this->mrmlScene()->AddNode(volumeNode.GetPointer());

    vtkNew<vtkMRMLScalarVolumeDisplayNode> displayNode;
    this->mrmlScene()->AddNode(displayNode.GetPointer());
    volumeNode->SetAndObserveDisplayNodeID(displayNode->GetID());
 //   volumeNode->SetOrigin(this->ReconstructedVolume->GetOrigin());
*/

    this->positions->Initialize();
    this->directions->Initialize();

    vtkNew<vtkMRMLVectorVolumeNode> volumeNode;
    vtkSmartPointer<vtkMRMLColorTableNode> colorNode = vtkSmartPointer<vtkMRMLColorTableNode>::New();
    colorNode->SetTypeToGrey();
    this->mrmlScene()->AddNode(colorNode);
    volumeNode->SetAndObserveImageData(this->ReconstructedVolume);
    volumeNode->SetOrigin(this->ReconstructedVolume->GetOrigin());
    volumeNode->SetSpacing(this->ReconstructedVolume->GetSpacing());
    this->mrmlScene()->AddNode(volumeNode.GetPointer());
    vtkNew<vtkMRMLVectorVolumeDisplayNode> displayNode;
    displayNode->SetAndObserveColorNodeID(colorNode->GetID());
    this->mrmlScene()->AddNode(displayNode.GetPointer());
    this->mrmlScene()->UpdateNodeChangedIDs();
    volumeNode->SetAndObserveDisplayNodeID(displayNode->GetID());

    saveSettings();

    this->blockSignals(false);
}

vtkSmartPointer<vtkImageData> qSlicerPancreaticLUSModuleWidget::readPNGImages(QString directory, int index)
{
    Q_D(qSlicerPancreaticLUSModuleWidget);
    vtkSmartPointer<vtkImageData> outImage = vtkSmartPointer<vtkImageData>::New();
    QString fileName = QString("%1/crop%2.png").arg(directory).arg(QString::number(index+1));
    QByteArray byteArray = fileName.toUtf8();
    const char* cString = byteArray.constData();

    if(this->pngReader->CanReadFile(cString) == 0)
    {
        QMessageBox messageBox;
        messageBox.critical(0,tr("Error"),tr("Invalid PNG image"));
        return 0;
    }
    this->pngReader->SetFileName(cString);
    this->pngReader->Update();
   // qDebug() << "Reading" << this->pngReader->GetFileName();
    outImage = this->pngReader->GetOutput();
    //this->transformImageToReference[index] = vtkSmartPointer<vtkMatrix4x4>::New();


    double *pos = {this->positions->GetPoint(index)};
    double *rot = {this->directions->GetPoint(index)};

    int offset0, offset1, offset2 = 0;
    offset0 = d->offset0SpinBox->value();
    offset1 = d->offset1SpinBox->value();
    offset2 = d->offset2SpinBox->value();
    vtkTransform *transform = vtkTransform::New();
    transform->Identity();
//    transform->PostMultiply();
//    transform->Translate(pos[1]+ offset0,pos[2] + offset1 ,pos[0] + offset2);
    transform->PreMultiply();
    transform->RotateZ(rot[0]);
    transform->RotateX(rot[1]);
    transform->RotateY(rot[2]);
    transform->Scale(0.1,0.1,0.1);
    transform->PostMultiply();
    transform->Translate(pos[1]+ offset0,pos[2] + offset1 ,pos[0] + offset2);
    transform->Update();
    transform->GetMatrix(this->transformImageToReference);
//    qDebug() << this->transformImageToReference->GetElement(0,0) << this->transformImageToReference->GetElement(0,1) << this->transformImageToReference->GetElement(0,2) << this->transformImageToReference->GetElement(0,3) << "\n"
//             << this->transformImageToReference->GetElement(1,0) << this->transformImageToReference->GetElement(1,1) << this->transformImageToReference->GetElement(1,2) << this->transformImageToReference->GetElement(1,3) << "\n"
//             << this->transformImageToReference->GetElement(2,0) << this->transformImageToReference->GetElement(2,1) << this->transformImageToReference->GetElement(2,2) << this->transformImageToReference->GetElement(2,3) << "\n"
//             << this->transformImageToReference->GetElement(3,0) << this->transformImageToReference->GetElement(3,1) << this->transformImageToReference->GetElement(3,2) << this->transformImageToReference->GetElement(3,3);
    transform->Delete();
    return outImage;
}

void qSlicerPancreaticLUSModuleWidget::setOutputExtent()
{
    double extent_Ref[6] =
    {
        VTK_DOUBLE_MAX, VTK_DOUBLE_MIN,
        VTK_DOUBLE_MAX, VTK_DOUBLE_MIN,
        VTK_DOUBLE_MAX, VTK_DOUBLE_MIN
    };


    for (int i = 0; i < this->imagesToProcess; ++i)
    {
        this->currentImage = readPNGImages(this->folderPath, i);
        if(this->currentImage == 0) return;
        int *frameExtent = currentImage->GetExtent();
        std::vector<double*> corners_ImagePix;
        double minX = frameExtent[0];
        double maxX = frameExtent[1];
        double minY = frameExtent[2];
        double maxY = frameExtent[3];

        double c0[4] = {minX,minY,0,1};
        double c1[4] = {minX,maxY,0,1};
        double c2[4] = {maxX,minY,0,1};
        double c3[4] = {maxX,maxY,0,1};
        corners_ImagePix.push_back(c0);
        corners_ImagePix.push_back(c1);
        corners_ImagePix.push_back(c2);
        corners_ImagePix.push_back(c3);

        // transform the corners to Reference and expand the extent if needed
        for (unsigned int corner = 0; corner < corners_ImagePix.size(); ++corner)
        {
            double corner_Ref[ 4 ] = { 0, 0, 0, 1 }; // position of the corner in the Reference coordinate system
            this->transformImageToReference->MultiplyPoint(corners_ImagePix[corner], corner_Ref);

            for ( int axis = 0; axis < 3; axis ++ )
            {
                if ( corner_Ref[axis] < extent_Ref[axis*2] )
                {
                    // min extent along this coord axis has to be decreased
                    extent_Ref[axis*2]=corner_Ref[axis];
                }
                if ( corner_Ref[axis] > extent_Ref[axis*2+1] )
                {
                    // max extent along this coord axis has to be increased
                    extent_Ref[axis*2+1]=corner_Ref[axis];
                }
            }
        }
    }

    double* outputSpacing = this->ReconstructedVolume->GetSpacing();
    this->outputExtent[1] = int((extent_Ref[1] - extent_Ref[0]) / outputSpacing[0]);
    this->outputExtent[3] = int((extent_Ref[3] - extent_Ref[2]) / outputSpacing[1]);
    this->outputExtent[5] = int((extent_Ref[5] - extent_Ref[4]) / outputSpacing[2]);

    this->ReconstructedVolume->SetOrigin(extent_Ref[0], extent_Ref[2], extent_Ref[4]);
    double* outputOrigin = this->ReconstructedVolume->GetOrigin();

    this->sliceAdder->setOutputExtent(this->outputExtent);
    this->sliceAdder->setOutputOrigin(outputOrigin);
    this->sliceAdder->setOutputSpacing(outputSpacing);
    qDebug() << "Volume origin:" << extent_Ref[0] << extent_Ref[2] << extent_Ref[4];
    qDebug() << "Extent size: " << outputExtent[0] << outputExtent[1] << outputExtent[2] << outputExtent[3] << outputExtent[4] << outputExtent[5];
    qDebug() << "Extent size: " << extent_Ref[0] << extent_Ref[1] << extent_Ref[2] << extent_Ref[3] << extent_Ref[4] << extent_Ref[5];

    if (this->sliceAdder->resetOutput() != 1)
    {
        qDebug() << "Error, failed to initialize output volume";
    }
}

void qSlicerPancreaticLUSModuleWidget::getFileName()
{
    Q_D(qSlicerPancreaticLUSModuleWidget);

    this->folderPath = QFileDialog::getExistingDirectory(this,tr("Open Directory"),"/media/datos/ETSIT/PFC/pseudotumor/VTS_01_1",QFileDialog::ShowDirsOnly);
}

void qSlicerPancreaticLUSModuleWidget::getTSVFileName()
{
    Q_D(qSlicerPancreaticLUSModuleWidget);

    this->tsvFilePath = QFileDialog::getOpenFileName(this,tr("Choose TSV file"),"/media/datos/ETSIT/PFC/pseudotumor/datos_tracker/Pancreas_Rec1.tsv",tr("TSV File (*.tsv)"));
}

void qSlicerPancreaticLUSModuleWidget::loadSettings()
{
    Q_D(qSlicerPancreaticLUSModuleWidget);

    d->offset0SpinBox->setValue(this->settings->value("offset0",0).toInt());
    d->offset1SpinBox->setValue(this->settings->value("offset1",0).toInt());
    d->offset2SpinBox->setValue(this->settings->value("offset2",0).toInt());
    d->numberOfImagesSpinBox->setValue(this->settings->value("numberOfImages",1700).toInt());

    int interpolation = this->settings->value("interpolationMethod",0).toInt();
    //FIXME magic numbers
    if(interpolation == 0) d->nearestRadioButton->setChecked(true);
    else if(interpolation == 1)  d->trilinearRadioButton->setChecked(true);
    this->tsvFilePath = this->settings->value("tsvFile","").toString();
    this->folderPath = this->settings->value("imagesFolder","").toString();
}


void qSlicerPancreaticLUSModuleWidget::saveSettings()
{
    Q_D(qSlicerPancreaticLUSModuleWidget);

    this->settings->setValue("tsvFile",this->tsvFilePath);
    this->settings->setValue("imagesFolder",this->folderPath);
    this->settings->setValue("offset0",d->offset0SpinBox->value());
    this->settings->setValue("offset1",d->offset1SpinBox->value());
    this->settings->setValue("offset2",d->offset2SpinBox->value());
    this->settings->setValue("numberOfImages",d->numberOfImagesSpinBox->value());
    this->settings->setValue("interpolationMethod",this->interpolationMethod);
}
