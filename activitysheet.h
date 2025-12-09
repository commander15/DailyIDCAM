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

    QTime duration() const;
};

class WorkOrder
{
public:
    int count = 0;
    QTime duration;
    QString region;
};

class HardwareReplacement
{
public:
    enum Type {
        UnknownType,
        TinyPC
    };

    enum Status { Ok, Nok };

    QString oldSerial;
    QString newSerial;
    int quantity = 0;
    QString reason;
    QString location;
    QString office;
    Intervention::SubSystem subSystem = Intervention::UnknownSystem;
    Type type = UnknownType;
    Status status = Nok;
};

class ActivitySheet
{
public:
    ActivitySheet();

    int interventionCount(Intervention::InterventionType type, Intervention::SubSystem subSystem) const;
    QTime workOrdersDuration() const;
    int replacementCount(Intervention::SubSystem system1, Intervention::SubSystem system2 = Intervention::UnknownSystem) const;

    QDate date;
    QString technician;
    QString region;
    QList<WorkOrder> workOrders;
    QList<Intervention> interventions;
    QList<HardwareReplacement> replacements;
};

#endif // ACTIVITYSHEET_H
