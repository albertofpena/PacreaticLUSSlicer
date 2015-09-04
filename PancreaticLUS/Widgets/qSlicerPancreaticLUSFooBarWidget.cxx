/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerPancreaticLUSFooBarWidget.h"
#include "ui_qSlicerPancreaticLUSFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_PancreaticLUS
class qSlicerPancreaticLUSFooBarWidgetPrivate
  : public Ui_qSlicerPancreaticLUSFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerPancreaticLUSFooBarWidget);
protected:
  qSlicerPancreaticLUSFooBarWidget* const q_ptr;

public:
  qSlicerPancreaticLUSFooBarWidgetPrivate(
    qSlicerPancreaticLUSFooBarWidget& object);
  virtual void setupUi(qSlicerPancreaticLUSFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerPancreaticLUSFooBarWidgetPrivate
::qSlicerPancreaticLUSFooBarWidgetPrivate(
  qSlicerPancreaticLUSFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerPancreaticLUSFooBarWidgetPrivate
::setupUi(qSlicerPancreaticLUSFooBarWidget* widget)
{
  this->Ui_qSlicerPancreaticLUSFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerPancreaticLUSFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerPancreaticLUSFooBarWidget
::qSlicerPancreaticLUSFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerPancreaticLUSFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerPancreaticLUSFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerPancreaticLUSFooBarWidget
::~qSlicerPancreaticLUSFooBarWidget()
{
}
