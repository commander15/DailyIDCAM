#ifndef ACTIVITYSHEETWRITER_H
#define ACTIVITYSHEETWRITER_H

#include <activitysheet.h>

namespace OpenXLSX {
class XLWorksheet;
}

class ActivitySheetWriter
{
public:
    ActivitySheetWriter();

    bool save(const QString &fileName, const ActivitySheet &sheet);

private:
    void writeInterventionStats(OpenXLSX::XLWorksheet &doc, const ActivitySheet &sheet);
    void writeInterventions(OpenXLSX::XLWorksheet &doc, const QString &technician, int row, const QList<Intervention> &interventions);
    void writeReplacements(OpenXLSX::XLWorksheet &doc, const QDate &date, int row, const QList<HardwareReplacement> &replacements);
};

#endif // ACTIVITYSHEETWRITER_H
