#ifndef ACTIVITYSHEETREADER_H
#define ACTIVITYSHEETREADER_H

#include <activitysheet.h>

namespace QXlsx {
class Document;
}

class ActivitySheetReader
{
public:
    ActivitySheetReader();

    ActivitySheet read(const QString &fileName);
    ActivitySheet read(QIODevice *device);

private:
    Intervention readIntervention(QXlsx::Document &doc, int row);
    HardwareReplacement readReplacement(QXlsx::Document &doc, int row);
};

#endif // ACTIVITYSHEETREADER_H
