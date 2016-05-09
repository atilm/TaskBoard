#include "histogram.h"
#include <QDebug>

Histogram::Histogram()
{
}

Histogram::~Histogram()
{
}

void Histogram::calculate(const QVector<double> &data, double binWidth)
{
    this->data = data;
    this->binWidth = binWidth;

    findBinRange();
    buildTicks();
    buildBars(data);
}

QVector<double> Histogram::getTicks()
{
    return ticks;
}

QVector<double> Histogram::getBars()
{
    return bars;
}

void Histogram::findBinRange()
{
    double minVal = *std::min_element(data.begin(), data.end());
    double maxVal = *std::max_element(data.begin(), data.end());

    minBinPos = static_cast<int>(floor(minVal / binWidth));
    maxBinPos = static_cast<int>(ceil(maxVal / binWidth));
}

void Histogram::buildTicks()
{
    ticks.clear();

    for(int pos=minBinPos; pos<=maxBinPos; pos++)
        ticks.append(pos*binWidth);
}

void Histogram::buildBars(const QVector<double> &data)
{
    bars.clear();
    bars.resize(maxBinPos - minBinPos + 1);

    int index = 0;

    for(int pos=minBinPos; pos <= maxBinPos; pos++){
        double lower = pos * binWidth - binWidth / 2;
        double upper = pos * binWidth + binWidth / 2;

        foreach(double d, data){
            if(d > lower && d <= upper)
                bars[index] += 1;
        }

        index++;
    }
}

