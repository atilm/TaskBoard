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

TEST_F(HistogramTests, builds_histogram_correctly){
    QVector<double> data;
    data << -1.5
         << -0.8 << -1.2
         << -0.4 << 0.2 << 0.5
         << 0.7  << 1.4
         << 2.1;

    hist->calculate(data, 1.0);

    QVector<double> bars = hist->getBars();

    EXPECT_EQ(6, bars.size());

    EXPECT_NEAR(1, bars[0], 0.001);
    EXPECT_NEAR(2, bars[1], 0.001);
    EXPECT_NEAR(3, bars[2], 0.001);
    EXPECT_NEAR(2, bars[3], 0.001);
    EXPECT_NEAR(1, bars[4], 0.001);
    EXPECT_NEAR(0, bars[5], 0.001);
}

TEST_F(HistogramTests, builds_ticks_correctly){
    QVector<double> data;
    data << -1.5
         << -0.8 << -1.2
         << -0.4 << 0.2 << 0.5
         << 0.7  << 1.4
         << 2.1;

    hist->calculate(data, 1.0);

    QVector<double> ticks = hist->getTicks();

    EXPECT_EQ(6, ticks.size());

    EXPECT_NEAR(-2, ticks[0], 0.001);
    EXPECT_NEAR(-1, ticks[1], 0.001);
    EXPECT_NEAR(0, ticks[2], 0.001);
    EXPECT_NEAR(1, ticks[3], 0.001);
    EXPECT_NEAR(2, ticks[4], 0.001);
    EXPECT_NEAR(3, ticks[5], 0.001);
}
