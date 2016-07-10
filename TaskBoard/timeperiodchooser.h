#ifndef TIMEPERIODCHOOSER_H
#define TIMEPERIODCHOOSER_H

#include <QDate>
#include <QWidget>

namespace Ui {
class TimePeriodChooser;
}

class TimePeriodChooser : public QWidget
{
    Q_OBJECT

public:
    explicit TimePeriodChooser(QWidget *parent = 0);
    virtual ~TimePeriodChooser();

    virtual QDate beginDate() const;
    virtual QDateTime beginDateTime() const;
    virtual QDate endDate() const;
    virtual QDateTime endDateTime() const;

signals:
    void periodChanged();

public slots:
    virtual void showCurrentWeek(bool on = true);
    virtual void showCurrentMonth(bool on = true);

protected slots:
    virtual void nextPeriod();
    virtual void previousPeriod();
    void untoggleButtons();

protected:
    Ui::TimePeriodChooser *ui;

    void stepPeriod(bool negative);
    void stepMonth(bool negative);
    void stepWeek(bool negative);
    void stepArbitraryPeriod(bool negative);
    void setDates(const QDate &begin, const QDate &end);
};


#endif // TIMEPERIODCHOOSER_H
