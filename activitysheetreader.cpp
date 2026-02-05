#include "activitysheetreader.h"

#include "utils.h"

#include <QFile>
#include <QTemporaryFile>
#include <QRegularExpression>

#include <OpenXLSX.hpp>

using namespace OpenXLSX;

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
    if (!device->isReadable())
        return ActivitySheet();

    QString fileName;

    // If it's a QFile, we just get the fileName
    if (device->inherits("QFile")) {
        QFile *file = qobject_cast<QFile *>(device);
        fileName = file->fileName();
    }

    if (fileName.isEmpty() || fileName.startsWith(":")) {
        QTemporaryFile *temp = new QTemporaryFile(device);
        if (!temp->open())
            return ActivitySheet();

        temp->write(device->readAll());
        temp->flush();
        temp->reset();
        fileName = temp->fileName();
    }

    XLDocument doc(fileName.toStdString());
    if (!doc.isOpen())
        return ActivitySheet();

    ActivitySheet sheet;
    sheet.date = QDate::currentDate();

    XLWorkbook book = doc.workbook();
    XLWorksheet excel = book.worksheet(1);

    int count = excel.rowCount();
    for (int row(2); row <= count; ++row) {
        const std::string task = excel.cell(row, 4).value().getString();
        if (task == "Intervention")
            sheet.interventions.append(readIntervention(excel, row));
        else if (task == "Remplacement")
            sheet.replacements.append(readReplacement(excel, row));
    }

    return sheet;
}

Intervention ActivitySheetReader::readIntervention(XLWorksheet &sheet, int row)
{
    const QString fullDescription = QString::fromStdString(sheet.cell(row, 3).value().getString());
    const int frontier = fullDescription.indexOf(":");
    const QString title = fullDescription.left(frontier);
    const QString description = fullDescription.right(fullDescription.size() - frontier -1);

    auto time = [&sheet](int row, int col) -> QTime {
        const QString value = QString::fromStdString(sheet.cell(row, col).getString());
        return QTime::fromString(value, "hh:mm:ss");
    };

    Intervention intervention;
    intervention.title = title.trimmed();
    intervention.description = description.trimmed();
    intervention.office = QString::fromStdString(sheet.cell(row, 2).getString());
    intervention.startTime = time(row, 10);
    intervention.endTime = time(row, 12);
    intervention.subSystem = Utils::subSystem(QString::fromStdString(sheet.cell(row, 1).getString()));

    const QStringList tags = QString::fromStdString(sheet.cell(row, 8).getString()).split(", ", Qt::SkipEmptyParts);
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

HardwareReplacement ActivitySheetReader::readReplacement(XLWorksheet &sheet, int row)
{
    HardwareReplacement replacement;
    return replacement;
}
