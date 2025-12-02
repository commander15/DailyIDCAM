#ifndef ACTIVITYSHEET_H
#define ACTIVITYSHEET_H

#include <QString>
#include <QDate>

class Intervention
{
public:
    enum SubSystem {
        PreEnrolment,
        Enrolment,
        Withdrawal,
        Validation,
        Production,
        Infrastructure,
        UnknownSystem
    };

    enum InterventionType {
        Software,
        Hardware,
        Replacement,
        UnknownType
    };

    enum Status {
        Processing,
        Solved,
        Unsolved,
        UnknownStatus
    };

    QString title;
    QString description;
    QString office;
    QString ticket;
    QTime startTime;
    QTime endTime;
    SubSystem subSystem = UnknownSystem;
    InterventionType type = UnknownType;
    Status status = UnknownStatus;
};

class WorkOrder
{
public:
    int count = 0;
    QTime duration;
    QString region;
};

class ActivitySheet
{
public:
    ActivitySheet();

    int interventionCount(Intervention::InterventionType type, Intervention::SubSystem subSystem) const;
    QTime workOrdersDuration() const;

    QDate date;
    QString technician;
    QString region;
    QList<WorkOrder> workOrders;
    QList<Intervention> interventions;
};

#endif // ACTIVITYSHEET_H
