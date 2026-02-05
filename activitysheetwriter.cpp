#include "activitysheetwriter.h"

#include "utils.h"

#include <QFile>

#include <OpenXLSX.hpp>

using namespace OpenXLSX;

ActivitySheetWriter::ActivitySheetWriter() {}

bool ActivitySheetWriter::save(const QString &fileName, const ActivitySheet &sheet)
{
    if (QFile::exists(fileName))
        QFile::remove(fileName);

    QFile::copy(":/resources/IDCAM_Fiche_Activite_Journaliere.xlsx", fileName);

    XLDocument doc(fileName.toStdString());
    if (!doc.isOpen())
        return false;

    XLWorksheet excel = doc.workbook().worksheet(2);

    // Setting date
    excel.cell("B2").value().set<std::string>(sheet.date.toString("dd/MM/yyyy").toStdString());

    // Filling first table
    writeInterventionStats(excel, sheet);

    // Filling second table
    excel.cell("A13").value().set<std::string>(sheet.region.toStdString());
    excel.cell("B13").value().set<std::string>(sheet.technician.toStdString());
    excel.cell("C13").value().set<int>(sheet.workOrders.count());
    excel.cell("D13").value().set<std::string>(sheet.workOrdersDuration().toString().toStdString());

    // Filling third table, starting filling at row 18
    writeInterventions(excel, sheet.technician, 18, sheet.interventions);

    // We write replacements, starting at row 29
    writeReplacements(excel, sheet.date, 29, sheet.replacements);

    try {
        doc.saveAs(fileName.toStdString(), true);
        return true;
    } catch (const XLException &) {
        return false;
    }
}

void ActivitySheetWriter::writeInterventionStats(XLWorksheet &doc, const ActivitySheet &sheet)
{
    // Filling region
    doc.cell("A6").value().set(sheet.region.toStdString());

    auto fillFirst = [&doc, &sheet](const char *cell, Intervention::InterventionType type, Intervention::SubSystem subSystem, Intervention::SubSystem subSystem2 = Intervention::UnknownSystem) {
        const int value = sheet.interventionCount(type, subSystem) + sheet.interventionCount(type, subSystem2);
        doc.cell(cell).value().set<int>(value);
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
    doc.cell("C8").value().set(sheet.replacementCount(Intervention::PreEnrolment));
    doc.cell("D8").value().set(sheet.replacementCount(Intervention::Enrolment, Intervention::Withdrawal));
    doc.cell("F8").value().set(sheet.replacementCount(Intervention::Validation));
    doc.cell("G8").value().set(sheet.replacementCount(Intervention::Production));
    doc.cell("H8").value().set(sheet.replacementCount(Intervention::Infrastructure));
}

void ActivitySheetWriter::writeInterventions(XLWorksheet &doc, const QString &technician, int row, const QList<Intervention> &interventions)
{
    int count = 0;

    for (const Intervention &intervention : interventions) {
        doc.cell(row, 2).value().set(technician.toStdString());
        doc.cell(row, 3).value().set(Utils::interventionTypeString(intervention.type).toStdString());
        doc.cell(row, 4).value().set(Utils::subSystemString(intervention.subSystem).toStdString());
        doc.cell(row, 5).value().set(intervention.office.toStdString());
        doc.cell(row, 6).value().set(intervention.title.toStdString());
        doc.cell(row, 7).value().set(intervention.description.toStdString());
        doc.cell(row, 8).value().set(intervention.ticket.toStdString());
        doc.cell(row, 9).value().set(intervention.startTime.toString("hh:mm").toStdString());
        doc.cell(row, 10).value().set(Utils::interventionStatusString(intervention.status).toStdString());
        doc.cell(row, 11).value().set(intervention.endTime.toString("hh:mm").toStdString());

        // Increment counter
        ++count;

        // We allow a single record overflow
        if (count == 6) {
            doc.cell(row, 1).value().set(count);
            break;
        }

        // Increment current row
        ++row;
    }
}

void ActivitySheetWriter::writeReplacements(XLWorksheet &doc, const QDate &date, int row, const QList<HardwareReplacement> &replacements)
{
    // We write count
    doc.cell("C26").value().set(replacements.count());

    // We write them row by row
    for (const HardwareReplacement &replacement : replacements) {
        doc.cell(row, 1).value().set(replacement.office.toStdString());
        doc.cell(row, 2).value().set(Utils::hardwareTypeString(replacement.type).toStdString());
        doc.cell(row, 3).value().set(date.toString("dd/MM/yyyy").toStdString());
        doc.cell(row, 4).value().set(replacement.oldSerial.toStdString());
        doc.cell(row, 5).value().set(replacement.quantity);
        doc.cell(row, 6).value().set(replacement.location.toStdString());
        doc.cell(row, 7).value().set(replacement.reason.toStdString());
        doc.cell(row, 8).value().set(replacement.newSerial.toStdString());
        doc.cell(row, 9).value().set(Utils::hardwareStatusString(replacement.status).toStdString());
        ++row;
    }
}
