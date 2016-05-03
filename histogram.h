#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QVector>

class Histogram
{
public:
    Histogram();
    virtual ~Histogram();

    void calculate(const QVector<double> &data, double binWidth);

    QVector<double> getTicks();
    QVector<double> getBars();

private:
    int minBinPos;
    int maxBinPos;
    double binWidth;
    QVector<double> data;
    QVector<double> ticks;
    QVector<double> bars;

    void findBinRange();
    void buildTicks();
    void buildBars(const QVector<double> &data);
};

#endif // HISTOGRAM_H
