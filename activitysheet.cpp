#include "activitysheet.h"

#include <QTime>


QTime Intervention::duration() const
{
    const int hour = endTime.hour() - startTime.hour();
    const int minute = endTime.minute() - startTime.minute();
    const int second = endTime.second() - startTime.second();
    return QTime(hour, minute, second);
}

ActivitySheet::ActivitySheet() {}

int ActivitySheet::interventionCount(Intervention::InterventionType type, Intervention::SubSystem subSystem) const
{
    return std::count_if(interventions.begin(), interventions.end(), [&type, &subSystem](const Intervention &intervention) {
        return intervention.type == type && intervention.subSystem == subSystem;
    });
}

QTime ActivitySheet::workOrdersDuration() const
{
    QTime duration;

    for (const WorkOrder &order : workOrders)
        duration = duration.addSecs(order.duration.secsTo(QTime()));

    return duration;
}

int ActivitySheet::replacementCount(Intervention::SubSystem system1, Intervention::SubSystem system2) const
{
    return std::count_if(replacements.begin(), replacements.end(), [&system1, &system2](const HardwareReplacement &replacement) {
        return replacement.subSystem == system1 || replacement.subSystem == system2;
    });
}
