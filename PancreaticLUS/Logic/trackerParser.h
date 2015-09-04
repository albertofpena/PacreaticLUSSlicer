#ifndef TRACKERPARSER_H
#define TRACKERPARSER_H

#include <QObject>
#include <iterator>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <iomanip>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

class trackerParser : public QObject
{
    Q_OBJECT
public:
    explicit trackerParser(QObject *parent = 0);
    ~trackerParser();

private:
    double tx;
    double ty;
    double tz;
    double rx;
    double ry;
    double rz;
    bool isFirst;
    int i;

signals:

public slots:
    void parseTSVFile(QString inputTSV, vtkSmartPointer<vtkPoints> positions, vtkSmartPointer<vtkPoints> directions);

};

#endif // TRACKERPARSER_H
