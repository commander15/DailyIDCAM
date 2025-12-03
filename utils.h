#ifndef UTILS_H
#define UTILS_H

#include "activitysheet.h"

class Utils
{
public:
    static Intervention::SubSystem subSystem(const QString &str);
    static QString subSystemString(Intervention::SubSystem system);

    static Intervention::InterventionType interventionType(const QString &str);
    static QString interventionTypeString(Intervention::InterventionType type);

    static Intervention::Status interventionStatus(const QString &str);
    static QString interventionStatusString(Intervention::Status status);

    static HardwareReplacement::Type hardwareType(const QString &str);
    static QString hardwareTypeString(HardwareReplacement::Type type);

    static HardwareReplacement::Status hardwareStatus(const QString &str);
    static QString hardwareStatusString(HardwareReplacement::Status status);

    static QString ticketNumber(const QString &str);

private:
    static bool is(const char *expected, const QString &current);
};

#endif // UTILS_H
