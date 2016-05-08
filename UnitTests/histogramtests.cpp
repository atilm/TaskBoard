#include "gtest/gtest.h"
#include "histogram.h"
#include <QDebug>

class HistogramTests : public ::testing::Test{
protected:
    Histogram *hist;

    HistogramTests(){
        hist = new Histogram();
    }

    virtual ~HistogramTests(){
        delete hist;
    }
};

TEST_F(HistogramTests, maximum_determination){
    QVector<double> data;
    data << 1.2 << 7.3 << 0.2 << 4.2;

    double maxVal = *std::max_element(data.begin(), data.end());

    EXPECT_NEAR(7.3, maxVal, 1e-6);
}

TEST_F(HistogramTests, minimum_determination){
    QVector<double> data;
    data << 1.2 << 7.3 << 0.2 << 4.2;

    double maxVal = *std::min_element(data.begin(), data.end());

    EXPECT_NEAR(0.2, maxVal, 1e-6);
}

TEST_F(HistogramTests, builds_triangle_histogram){
    QVector<double> data;
    data << -1.5
         << -0.8 << -1.2
         << -0.4 << 0.2 << 0.5
         << 0.7  << 1.4
         << 2.1;

    hist->calculate(data, 1.0);

    QVector<double> bars = hist->getBars();

    EXPECT_EQ(5, bars.size());

    foreach(double val, bars)
        qDebug() << val;
}
