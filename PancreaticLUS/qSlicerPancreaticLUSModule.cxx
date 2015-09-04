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
#include <QtPlugin>

// PancreaticLUS Logic includes
#include <vtkSlicerPancreaticLUSLogic.h>

// PancreaticLUS includes
#include "qSlicerPancreaticLUSModule.h"
#include "qSlicerPancreaticLUSModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerPancreaticLUSModule, qSlicerPancreaticLUSModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPancreaticLUSModulePrivate
{
public:
  qSlicerPancreaticLUSModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerPancreaticLUSModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerPancreaticLUSModulePrivate::qSlicerPancreaticLUSModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerPancreaticLUSModule methods

//-----------------------------------------------------------------------------
qSlicerPancreaticLUSModule::qSlicerPancreaticLUSModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerPancreaticLUSModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerPancreaticLUSModule::~qSlicerPancreaticLUSModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerPancreaticLUSModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerPancreaticLUSModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPancreaticLUSModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("Alberto Fernandez Pena");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerPancreaticLUSModule::icon() const
{
  return QIcon(":/Icons/PancreaticLUS.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerPancreaticLUSModule::categories() const
{
  return QStringList() << "NAVISurg";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPancreaticLUSModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerPancreaticLUSModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerPancreaticLUSModule
::createWidgetRepresentation()
{
  return new qSlicerPancreaticLUSModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerPancreaticLUSModule::createLogic()
{
  return vtkSlicerPancreaticLUSLogic::New();
}
