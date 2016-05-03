#include "trackerParser.h"
#include <QDebug>

using namespace std;

trackerParser::trackerParser(QObject *parent) :
    QObject(parent)
{
    this->isFirst = true;
}

trackerParser::~trackerParser()
{
    this->isFirst = true;
}

void trackerParser::parseTSVFile(QString inputFile, vtkSmartPointer<vtkPoints> positions, vtkSmartPointer<vtkPoints> directions)
{   
    QFile file(inputFile);

    if(!file.open(QFile::ReadOnly))
    {
        return;
    }

    positions->SetDataTypeToDouble();
    directions->SetDataTypeToDouble();

    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QList<QByteArray> splitLine;
        line.replace(',','.');
        if(isFirst == true)
        {
            isFirst = false;
            continue;
        }
        splitLine = line.split('\t');
        positions->InsertNextPoint(splitLine.at(20).toDouble(),splitLine.at(21).toDouble(),splitLine.at(22).toDouble());
        directions->InsertNextPoint(splitLine.at(17).toDouble(),splitLine.at(18).toDouble(),splitLine.at(19).toDouble());
    }

    file.close();
	isFirst = true;
}

