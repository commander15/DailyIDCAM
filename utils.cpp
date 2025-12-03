#include "utils.h"

#include <QRegularExpression>

Intervention::SubSystem Utils::subSystem(const QString &str)
{
    auto is = [&str](const char *s) { return Utils::is(s, str); };

    if (is("Pré-enrôlement"))      return Intervention::PreEnrolment;
    else if (is("Enrôlement"))     return Intervention::Enrolment;
    else if (is("Retrait"))        return Intervention::Withdrawal;
    else if (is("Validation"))     return Intervention::Validation;
    else if (is("Production"))     return Intervention::Production;
    else if (is("Infrastructure")) return Intervention::Infrastructure;
    else                           return Intervention::UnknownSystem;
}

QString Utils::subSystemString(Intervention::SubSystem system)
{
    switch (system) {
    case Intervention::PreEnrolment:    return "Pré-enrôlement";
    case Intervention::Enrolment:       return "Enrôlement";
    case Intervention::Withdrawal:      return "Retrait";
    case Intervention::Validation:      return "Validation";
    case Intervention::Production:      return "Production";
    case Intervention::Infrastructure:  return "Infrastructure";
    case Intervention::UnknownSystem:   return "Inconnu";
    default:                            return QString();
    }
}

Intervention::InterventionType Utils::interventionType(const QString &str)
{
    auto is = [&str](const char *s) { return Utils::is(s, str); };

    if (is("Logiciel"))          return Intervention::Software;
    else if (is("Matériel"))     return Intervention::Hardware;
    else if (is("Remplacement")) return Intervention::Replacement;
    else                         return Intervention::UnknownType;
}

QString Utils::interventionTypeString(Intervention::InterventionType type)
{
    switch (type) {
    case Intervention::Software:     return "Logiciel";
    case Intervention::Hardware:     return "Matériel";
    case Intervention::Replacement:  return "Remplacement";
    case Intervention::UnknownType:  return "Inconnu";
    default:                         return QString();
    }
}

Intervention::Status Utils::interventionStatus(const QString &str)
{
    auto is = [&str](const char *s) { return Utils::is(s, str); };

    if (is("En cours de traitement"))  return Intervention::Processing;
    else if (is("Résolu"))              return Intervention::Solved;
    else if (is("Non résolu"))          return Intervention::Unsolved;
    else                                return Intervention::UnknownStatus;
}

QString Utils::interventionStatusString(Intervention::Status status)
{
    switch (status) {
    case Intervention::Processing:     return "En cours de traitement";
    case Intervention::Solved:         return "Résolu";
    case Intervention::Unsolved:       return "Non résolu";
    case Intervention::UnknownStatus:  return "Inconnu";
    default:                           return QString();
    }
}

HardwareReplacement::Type Utils::hardwareType(const QString &str)
{
    auto is = [&str](const char *s) { return Utils::is(s, str); };

    if (is("Mini PC"))      return HardwareReplacement::TinyPC;
    else                    return HardwareReplacement::UnknownType;
}

QString Utils::hardwareTypeString(HardwareReplacement::Type type)
{
    switch (type) {
    case HardwareReplacement::TinyPC:       return "Mini PC";
    case HardwareReplacement::UnknownType:  return "Inconnu";
    default:                                return QString();
    }
}

HardwareReplacement::Status Utils::hardwareStatus(const QString &str)
{
    auto is = [&str](const char *s) { return Utils::is(s, str); };

    if (is("OK"))   return HardwareReplacement::Ok;
    else            return HardwareReplacement::Nok;
}

QString Utils::hardwareStatusString(HardwareReplacement::Status status)
{
    switch (status) {
    case HardwareReplacement::Ok:  return "OK";
    case HardwareReplacement::Nok:
    default:                       return "NOK";
    }
}

QString Utils::ticketNumber(const QString &str)
{
    static const QRegularExpression ticketExpr(R"(^L\dISD-\d+$)", QRegularExpression::CaseInsensitiveOption);
    const QRegularExpressionMatch match = ticketExpr.match(str.trimmed());

    return match.hasMatch() ? match.captured(0) : QString();
}

bool Utils::is(const char *expected, const QString &current)
{
    return current.trimmed().compare(QString::fromUtf8(expected), Qt::CaseInsensitive) == 0;
}
