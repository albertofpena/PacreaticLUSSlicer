#ifndef TRACKERPARSER_H
#define TRACKERPARSER_H

#include <QObject>
#include <QFile>
#include <QStringList>

#include <vtkPoints.h>
#include <vtkSmartPointer.h>

class trackerParser : public QObject
{
    Q_OBJECT
public:
    explicit trackerParser(QObject *parent = 0);
    ~trackerParser();

private:
    bool isFirst;
signals:

public slots:
    void parseTSVFile(QString inputTSV, vtkSmartPointer<vtkPoints> positions, vtkSmartPointer<vtkPoints> directions);

};

#endif // TRACKERPARSER_H
