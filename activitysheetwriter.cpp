#include "activitysheetwriter.h"

#include <QFile>

#include <xlsxdocument.h>

using namespace QXlsx;

ActivitySheetWriter::ActivitySheetWriter() {}

bool ActivitySheetWriter::save(const QString &fileName, const ActivitySheet &sheet)
{
    Document doc(":/resources/IDCAM_Fiche_Activité_Journaliere..xlsx");
    if (!doc.load())
        return false;

    // Setting date
    doc.write("B2", sheet.date.toString("dd/MM/yyyy"));

    // Filling first table
    doc.write("A6", sheet.region);
    auto fillFirst = [&doc, &sheet](const char *cell, Intervention::InterventionType type, Intervention::SubSystem subSystem, Intervention::SubSystem subSystem2 = Intervention::UnknownSystem) {
        doc.write(cell, sheet.interventionCount(type, subSystem) + sheet.interventionCount(type, subSystem2));
    };
    fillFirst("C6", Intervention::Software, Intervention::PreEnrolment);
    fillFirst("D6", Intervention::Software, Intervention::Enrolment, Intervention::Withdrawal);
    fillFirst("E6", Intervention::Software, Intervention::Validation);
    fillFirst("F6", Intervention::Software, Intervention::Production);
    fillFirst("G6", Intervention::Software, Intervention::Infrastructure);

    // Filling second table
    doc.write("A13", sheet.region);
    doc.write("B13", sheet.technician);
    doc.write("C13", sheet.workOrders.count());
    doc.write("D13", sheet.workOrdersDuration());

    // Filling third table
    const int interventionFirstRow = 18;
    int row = interventionFirstRow;
    for (const Intervention &intervention : sheet.interventions) {
        QString status;
        QString subSys;
        QString type;

        switch (intervention.status) {
        case Intervention::Processing:
            status = "En cours de traitement";
            break;

        case Intervention::Solved:
            status = "Résolu";
            break;

        case Intervention::Unsolved:
            status = "Non résolu";
            break;

        case Intervention::UnknownStatus:
            break;
        }

        switch (intervention.type) {
        case Intervention::Software:
            type = "Logiciel";
            break;

        case Intervention::Hardware:
            type = "Matériel";
            break;

        case Intervention::Replacement:
            type = "Remplacement";
            break;

        case Intervention::UnknownType:
            break;
        }

        switch (intervention.subSystem) {
        case Intervention::PreEnrolment:
            subSys = "Pré-enrolement";
            break;

        case Intervention::Enrolment:
            subSys = "Enrolement";
            break;

        case Intervention::Withdrawal:
            subSys = "Retrait";
            break;

        case Intervention::Validation:
            subSys = "Validation";
            break;

        case Intervention::Production:
            subSys = "Production";
            break;

        case Intervention::Infrastructure:
            subSys = "Infrastructure";
            break;

        case Intervention::UnknownSystem:
            break;
        }

        doc.write(row, 2, sheet.technician);
        doc.write(row, 3, type);
        doc.write(row, 4, subSys);
        doc.write(row, 5, intervention.office);
        doc.write(row, 6, intervention.title);
        doc.write(row, 7, intervention.description);
        doc.write(row, 8, intervention.ticket);
        doc.write(row, 9, intervention.startTime);
        doc.write(row, 10, status);
        doc.write(row, 11, intervention.endTime);

        // We allow a single record overflow
        if (row >= interventionFirstRow + 5) {
            doc.write(row, 1, row - interventionFirstRow + 1);
            break;
        }

        ++row;
    }

    return doc.saveAs(fileName);
}
