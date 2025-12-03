#include "activitysheetreader.h"

#include "utils.h"

#include <QFile>
#include <QRegularExpression>

#include <xlsxdocument.h>

using namespace QXlsx;

ActivitySheetReader::ActivitySheetReader() {}

ActivitySheet ActivitySheetReader::read(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
        return read(&file);
    return ActivitySheet();
}

ActivitySheet ActivitySheetReader::read(QIODevice *device)
{
    Document doc(device);
    if (!doc.load())
        return ActivitySheet();

    ActivitySheet sheet;
    sheet.date = QDate::currentDate();

    int count = doc.dimension().bottomLeft().row();
    for (int row(2); row <= count; ++row) {
        const QString task = doc.read(row, 4).toString();
        if (task == "Intervention")
            sheet.interventions.append(readIntervention(doc, row));
        else if (task == "Remplacement")
            sheet.replacements.append(readReplacement(doc, row));
    }

    return sheet;
}

Intervention ActivitySheetReader::readIntervention(Document &doc, int row)
{
    const QString fullDescription = doc.read(row, 3).toString();
    const int frontier = fullDescription.indexOf(":");
    const QString title = fullDescription.left(frontier);
    const QString description = fullDescription.right(fullDescription.size() - frontier -1);

    Intervention intervention;
    intervention.title = title.trimmed();
    intervention.description = description.trimmed();
    intervention.office = doc.read(row, 2).toString();
    intervention.startTime = doc.read(row, 10).toTime();
    intervention.endTime = doc.read(row, 12).toTime();
    intervention.subSystem = Utils::subSystem(doc.read(row, 1).toString());

    const QStringList tags = doc.read(row, 8).toString().split(", ", Qt::SkipEmptyParts);
    for (const QString &tag : tags) {
        // First we check if it's not a ticket number
        const QString ticket = Utils::ticketNumber(tag);
        if (!ticket.isEmpty()) {
            intervention.ticket = ticket;
        }

        // Next, we check if it's not an intervention type
        Intervention::InterventionType type = Utils::interventionType(tag);
        if (type != Intervention::UnknownType) {
            intervention.type = type;
            continue;
        }

        // Later, we check if it's not an intervention status
        Intervention::Status status = Utils::interventionStatus(tag);
        if (status != Intervention::UnknownStatus) {
            intervention.status = status;
            continue;
        }
    }

    return intervention;
}

HardwareReplacement ActivitySheetReader::readReplacement(QXlsx::Document &doc, int row)
{
    HardwareReplacement replacement;
    return replacement;
}
