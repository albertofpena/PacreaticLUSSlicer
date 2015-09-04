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

#ifndef __qSlicerPancreaticLUSModuleWidget_h
#define __qSlicerPancreaticLUSModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"
#include "qSlicerCoreApplication.h"
#include "qSlicerPancreaticLUSModuleExport.h"

// Qt includes
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

// VTK includes
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkImageData.h>
#include <vtkPNGReader.h>
#include <vtkMatrix4x4.h>
#include <vtkInformation.h>
#include <vtkTransform.h>
#include <vtkMRMLScene.h>

// TODO remove unused
#include <vtkMRMLVectorVolumeNode.h>
#include <vtkMRMLVectorVolumeDisplayNode.h>
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLScalarVolumeDisplayNode.h>
#include <vtkMRMLColorTableNode.h>
#include <vtkImageExtractComponents.h>
#include <vtkMetaImageWriter.h>
#include <vtkNew.h>
#include <vtkSlicerApplicationLogic.h>
#include <vtkMRMLSelectionNode.h>
#include <vtkImageReslice.h>

// Project includes
#include "Logic/trackerParser.h"
#include "Logic/imageProcessor.h"
#include "Logic/insertSlice.h"


struct pancreaticImage
{
    vtkSmartPointer<vtkImageData> image;
    vtkSmartPointer<vtkMatrix4x4> transformation;
};

class qSlicerPancreaticLUSModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_PANCREATICLUS_EXPORT qSlicerPancreaticLUSModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerPancreaticLUSModuleWidget(QWidget *parent=0);
  virtual ~qSlicerPancreaticLUSModuleWidget();

public slots:
    void generateVolume();
private slots:
    void getFileName();
    void getTSVFileName();
protected:
  QScopedPointer<qSlicerPancreaticLUSModuleWidgetPrivate> d_ptr;

  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicerPancreaticLUSModuleWidget);
  Q_DISABLE_COPY(qSlicerPancreaticLUSModuleWidget);

  int firstImageToProcess;
  int lastImageToProcess;
  int interpolationMethod;
  int outputExtent[6];
  double *origin;
  vtkSmartPointer<vtkImageData> ReconstructedVolume;
  vtkSmartPointer<vtkPoints> positions;
  vtkSmartPointer<vtkPoints> directions;
  vtkSmartPointer<vtkPNGReader> pngReader;
  vtkSmartPointer<vtkImageData> imageData;
  vtkSmartPointer<vtkMatrix4x4> transformImageToReference;
  vtkSmartPointer<vtkMatrix4x4> finalTransform;
  vtkSmartPointer<vtkImageData> currentImage;

  QString folderPath;
  QString tsvFilePath;

  InsertSlice *sliceAdder;
  pancreaticImage imageDataSet[1700];

  QSettings *settings;

  vtkSmartPointer<vtkImageData> readPNGImages(QString directory, int index);

  void setOutputExtent();
  void loadSettings();
  void saveSettings();
};

#endif
