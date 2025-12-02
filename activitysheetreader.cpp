#include "activitysheetreader.h"

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
    }

    return sheet;
}

Intervention ActivitySheetReader::readIntervention(Document &doc, int row)
{
    static const QRegularExpression ticketExpr(R"(^L\dISD-\d+$)");

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

    const QString subSystem = doc.read(row, 1).toString();
    if (subSystem == "Enrôlement")
        intervention.subSystem = Intervention::Enrolment;
    else if (subSystem == "Pré-enrôlement")
        intervention.subSystem = Intervention::PreEnrolment;
    else if (subSystem == "Retrait")
        intervention.subSystem = Intervention::Withdrawal;
    else if (subSystem == "Validation")
        intervention.subSystem = Intervention::Validation;
    else if (subSystem == "Production")
        intervention.subSystem = Intervention::Production;
    else if (subSystem == "Infrastructure")
        intervention.subSystem = Intervention::Infrastructure;

    const QStringList tags = doc.read(row, 8).toString().split(", ", Qt::SkipEmptyParts);
    for (const QString &tag : tags) {
        if (ticketExpr.matchView(tag).hasMatch())
            intervention.ticket = tag.toUpper();
        else if (tag == "logiciel")
            intervention.type = Intervention::Software;
        else if (tag == "matériel")
            intervention.type = Intervention::Hardware;
        else if (tag == "remplacement")
            intervention.type = Intervention::Replacement;
        else if (tag == "résolu")
            intervention.status = Intervention::Solved;
        else if (tag == "en cours de traitement")
            intervention.status = Intervention::Processing;
        else if (tag == "non résolu")
            intervention.status = Intervention::Unsolved;
    }

    return intervention;
}
