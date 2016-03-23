#ifndef PROJECTENTRY_H
#define PROJECTENTRY_H

#include <QString>

class ProjectEntry
{
public:
    ProjectEntry();
    virtual ~ProjectEntry();

    int id;
    QString title;
    QString shortSign;
    QString description;
};

#endif // PROJECTENTRY_H
