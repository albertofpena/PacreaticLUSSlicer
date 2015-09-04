#include "trackerParser.h"
#include <QDebug>

using namespace std;

trackerParser::trackerParser(QObject *parent) :
    QObject(parent)
{
    this->isFirst = true;
    this->i = 0;
    this->tx = 0;
    this->ty = 0;
    this->tz = 0;
    this->rx = 0;
    this->ry = 0;
    this->rz = 0;
}

trackerParser::~trackerParser()
{
    this->isFirst = true;
    this->i = 0;
    this->tx = 0;
    this->ty = 0;
    this->tz = 0;
    this->rx = 0;
    this->ry = 0;
    this->rz = 0;
}

void trackerParser::parseTSVFile(QString inputFile, vtkSmartPointer<vtkPoints> positions, vtkSmartPointer<vtkPoints> directions)
{
    QByteArray byteArray = inputFile.toUtf8();
    const char* cString = byteArray.constData();

    std::cout << std::setprecision(9);

    std::ifstream input(cString);
    if (!input.is_open()) return;

    positions->SetDataTypeToDouble();

    // The file we are reading is a .tsv, we have to set \t (tab) as char separator
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\t");

    string line;
    while (getline(input, line))
    {
        if(this->isFirst == true)
        {
            this->isFirst = false;
            continue;
        }
        tokenizer tok(line,sep);
        for (tokenizer::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
        {
            string str = *tok_iter;
            double w = atof(str.c_str());

            if (this->i == 17)
            {
                // Reading Rz
                this->rz = w;
            }
            if (this->i == 18)
            {
                // Reading Ry
                this->ry = w;
            }
            if (this->i == 19)
            {
                // Reading Rz
                this->rx = w;
            }
            if (this->i == 20)
            {
                // Reading Tx
                this->tx = w;
            }
            if (this->i == 21)
            {
                // Reading Ty
                this->ty = w;
            }
            if (this->i == 22)
            {
                // Reading Tz
                this->tz = w;
            }
            this->i++;
        }
        double point[3] = {this->tx, this->ty, this->tz};
        double angle[3] = {this->rx, this->ry, this->rz};

        positions->InsertNextPoint(point);
        directions->InsertNextPoint(angle);

        this->i = 0;
    }
    input.close();
}

