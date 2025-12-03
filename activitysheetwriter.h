#ifndef ACTIVITYSHEETWRITER_H
#define ACTIVITYSHEETWRITER_H

#include <activitysheet.h>

namespace QXlsx {
class Document;
}

class ActivitySheetWriter
{
public:
    ActivitySheetWriter();

    bool save(const QString &fileName, const ActivitySheet &sheet);

private:
    void writeInterventionStats(QXlsx::Document &doc, const ActivitySheet &sheet);
    void writeInterventions(QXlsx::Document &doc, const QString &technician, int row, const QList<Intervention> &interventions);
    void writeReplacements(QXlsx::Document &doc, const QDate &date, int row, const QList<HardwareReplacement> &replacements);
};

#endif // ACTIVITYSHEETWRITER_H
