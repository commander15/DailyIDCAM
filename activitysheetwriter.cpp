#include "activitysheetwriter.h"

#include "utils.h"

#include <QFile>

#include <xlsxdocument.h>

using namespace QXlsx;

ActivitySheetWriter::ActivitySheetWriter() {}

bool ActivitySheetWriter::save(const QString &fileName, const ActivitySheet &sheet)
{
    Document doc(":/resources/IDCAM_Fiche_Activite_Journaliere.xlsx");
    if (!doc.load())
        return false;

    // Setting date
    doc.write("B2", sheet.date.toString("dd/MM/yyyy"));

    // Filling first table
    writeInterventionStats(doc, sheet);

    // Filling second table
    doc.write("A13", sheet.region);
    doc.write("B13", sheet.technician);
    doc.write("C13", sheet.workOrders.count());
    doc.write("D13", sheet.workOrdersDuration());

    // Filling third table, starting filling at row 18
    writeInterventions(doc, sheet.technician, 18, sheet.interventions);

    // We write replacements, starting at row 29
    writeReplacements(doc, sheet.date, 29, sheet.replacements);

    return doc.saveAs(fileName);
}

void ActivitySheetWriter::writeInterventionStats(QXlsx::Document &doc, const ActivitySheet &sheet)
{
    // Filling region
    doc.write("A6", sheet.region);

    auto fillFirst = [&doc, &sheet](const char *cell, Intervention::InterventionType type, Intervention::SubSystem subSystem, Intervention::SubSystem subSystem2 = Intervention::UnknownSystem) {
        doc.write(cell, sheet.interventionCount(type, subSystem) + sheet.interventionCount(type, subSystem2));
    };

    fillFirst("C6", Intervention::Software, Intervention::PreEnrolment);
    fillFirst("D6", Intervention::Software, Intervention::Enrolment, Intervention::Withdrawal);
    fillFirst("F6", Intervention::Software, Intervention::Validation);
    fillFirst("G6", Intervention::Software, Intervention::Production);
    fillFirst("H6", Intervention::Software, Intervention::Infrastructure);
    fillFirst("C7", Intervention::Hardware, Intervention::PreEnrolment);
    fillFirst("D7", Intervention::Hardware, Intervention::Enrolment, Intervention::Withdrawal);
    fillFirst("F7", Intervention::Hardware, Intervention::Validation);
    fillFirst("G7", Intervention::Hardware, Intervention::Production);
    fillFirst("H7", Intervention::Hardware, Intervention::Infrastructure);
    doc.write("C8", sheet.replacementCount(Intervention::PreEnrolment));
    doc.write("D8", sheet.replacementCount(Intervention::Enrolment, Intervention::Withdrawal));
    doc.write("F8", sheet.replacementCount(Intervention::Validation));
    doc.write("G8", sheet.replacementCount(Intervention::Production));
    doc.write("H8", sheet.replacementCount(Intervention::Infrastructure));
}

void ActivitySheetWriter::writeInterventions(QXlsx::Document &doc, const QString &technician, int row, const QList<Intervention> &interventions)
{
    int count = 0;

    for (const Intervention &intervention : interventions) {
        doc.write(row, 2, technician);
        doc.write(row, 3, Utils::interventionTypeString(intervention.type));
        doc.write(row, 4, Utils::subSystemString(intervention.subSystem));
        doc.write(row, 5, intervention.office);
        doc.write(row, 6, intervention.title);
        doc.write(row, 7, intervention.description);
        doc.write(row, 8, intervention.ticket);
        doc.write(row, 9, intervention.startTime.toString("hh:mm"));
        doc.write(row, 10, Utils::interventionStatusString(intervention.status));
        doc.write(row, 11, intervention.endTime.toString("hh:mm"));

        // Increment counter
        ++count;

        // We allow a single record overflow
        if (count == 6) {
            doc.write(row, 1, count);
            break;
        }

        // Increment current row
        ++row;
    }
}

void ActivitySheetWriter::writeReplacements(QXlsx::Document &doc, const QDate &date, int row, const QList<HardwareReplacement> &replacements)
{
    // We write count
    doc.write("C26", replacements.count());

    // We write them row by row
    for (const HardwareReplacement &replacement : replacements) {
        doc.write(row, 1, replacement.office);
        doc.write(row, 2, Utils::hardwareTypeString(replacement.type));
        doc.write(row, 3, date);
        doc.write(row, 4, replacement.oldSerial);
        doc.write(row, 5, replacement.quantity);
        doc.write(row, 6, replacement.location);
        doc.write(row, 7, replacement.reason);
        doc.write(row, 8, replacement.newSerial);
        doc.write(row, 9, Utils::hardwareStatusString(replacement.status));
        ++row;
    }
}
