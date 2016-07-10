#include "timeperiodchooser.h"
#include "ui_timeperiodchooser.h"

TimePeriodChooser::TimePeriodChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimePeriodChooser)
{
    ui->setupUi(this);

    connect(ui->weekButton, SIGNAL(toggled(bool)),
            this, SLOT(showCurrentWeek(bool)));
    connect(ui->monthButton, SIGNAL(toggled(bool)),
            this, SLOT(showCurrentMonth(bool)));
    connect(ui->previousButton, SIGNAL(clicked(bool)),
            this, SLOT(previousPeriod()));
    connect(ui->nextButton, SIGNAL(clicked(bool)),
            this, SLOT(nextPeriod()));
    connect(ui->beginDateChooser, SIGNAL(dateChanged(QDate)),
            this, SIGNAL(periodChanged()));
    connect(ui->endDateChooser, SIGNAL(dateChanged(QDate)),
            this, SIGNAL(periodChanged()));
    connect(ui->beginDateChooser, SIGNAL(dateChanged(QDate)),
            this, SLOT(untoggleButtons()));
    connect(ui->endDateChooser, SIGNAL(dateChanged(QDate)),
            this, SLOT(untoggleButtons()));
}

TimePeriodChooser::~TimePeriodChooser()
{
    delete ui;
}

QDate TimePeriodChooser::beginDate() const
{
    return ui->beginDateChooser->date();
}

QDateTime TimePeriodChooser::beginDateTime() const
{
    return ui->beginDateChooser->dateTime();
}

QDate TimePeriodChooser::endDate() const
{
    return ui->endDateChooser->date();
}

QDateTime TimePeriodChooser::endDateTime() const
{
    return ui->endDateChooser->dateTime();
}

void TimePeriodChooser::toggleCurrentWeek(bool on)
{
    ui->weekButton->setChecked(on);
}

void TimePeriodChooser::toggleCurrentMonth(bool on)
{
    ui->monthButton->setChecked(on);
}

void TimePeriodChooser::showCurrentWeek(bool on)
{
    if(!on)
        return;

    ui->monthButton->setChecked(false);
    QDate today = QDate::currentDate();
    QDate monday = today.addDays(-1 * (today.dayOfWeek() - 1));
    QDate sunday = monday.addDays(6);

    setDates(monday, sunday);
}

void TimePeriodChooser::showCurrentMonth(bool on)
{
    if(!on)
        return;

    ui->weekButton->setChecked(false);
    QDate today = QDate::currentDate();
    QDate first = today.addDays(-1 * (today.day() - 1));
    QDate last = first.addDays(today.daysInMonth() - 1);

    setDates(first, last);
}

void TimePeriodChooser::nextPeriod()
{
    stepPeriod(false);
}

void TimePeriodChooser::previousPeriod()
{
    stepPeriod(true);
}

void TimePeriodChooser::stepPeriod(bool negative)
{
    if(ui->weekButton->isChecked())
        stepWeek(negative);
    else if(ui->monthButton->isChecked())
        stepMonth(negative);
    else
        stepArbitraryPeriod(negative);
}

void TimePeriodChooser::stepMonth(bool negative)
{
    int direction = negative ? -1 : 1;

    QDate begin = ui->beginDateChooser->date();
    QDate end = ui->endDateChooser->date();

    begin = begin.addMonths(direction);
    end = end.addMonths(direction);

    int endDateCorrection = end.daysInMonth() - end.day();
    end = end.addDays(endDateCorrection);

    setDates(begin, end);
}

void TimePeriodChooser::stepWeek(bool negative)
{
    stepArbitraryPeriod(negative);
}

void TimePeriodChooser::stepArbitraryPeriod(bool negative)
{
    int direction = negative ? -1 : 1;

    QDate begin = ui->beginDateChooser->date();
    QDate end = ui->endDateChooser->date();

    int days = direction * (begin.daysTo(end) + 1);

    begin = begin.addDays(days);
    end = end.addDays(days);

    setDates(begin, end);
}

void TimePeriodChooser::setDates(const QDate &begin, const QDate &end)
{
    ui->beginDateChooser->blockSignals(true);
    ui->endDateChooser->blockSignals(true);

    ui->beginDateChooser->setDate(begin);
    ui->endDateChooser->setDate(end);

    ui->beginDateChooser->blockSignals(false);
    ui->endDateChooser->blockSignals(false);

    emit periodChanged();
}

void TimePeriodChooser::untoggleButtons()
{
    ui->weekButton->setChecked(false);
    ui->monthButton->setChecked(false);
}
