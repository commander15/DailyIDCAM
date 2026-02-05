#ifndef ACTIVITYSHEETREADER_H
#define ACTIVITYSHEETREADER_H

#include <activitysheet.h>

namespace OpenXLSX {
class XLWorksheet;
}

class ActivitySheetReader
{
public:
    ActivitySheetReader();

    ActivitySheet read(const QString &fileName);
    ActivitySheet read(QIODevice *device);

private:
    Intervention readIntervention(OpenXLSX::XLWorksheet &sheet, int row);
    HardwareReplacement readReplacement(OpenXLSX::XLWorksheet &sheet, int row);
};

#endif // ACTIVITYSHEETREADER_H
