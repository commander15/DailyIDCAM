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
};

#endif // ACTIVITYSHEETWRITER_H
