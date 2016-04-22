#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H

#include <QLabel>

class TimeDisplay : public QLabel
{
    Q_OBJECT
public:
    explicit TimeDisplay(QWidget *parent = 0);
    virtual ~TimeDisplay();

signals:

public slots:
    virtual void setMinutes(int minutes);
};

#endif // TIMEDISPLAY_H
