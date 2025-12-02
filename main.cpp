#include <QCoreApplication>
#include <QTextStream>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <activitysheetreader.h>
#include <activitysheetwriter.h>

#ifdef QT_NETWORK_LIB
#include "clockify.h"
#endif

bool write(const QString &dir, const ActivitySheet &sheet);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("idcam-daily");
    app.setApplicationVersion(TOOL_VERSION);

    QTextStream out(stdout);

    QCommandLineOption technician({"t", "technician"}, "Technician name", "technician");
    QCommandLineOption region({"r", "region"}, "Technician affectation region", "region");
    QCommandLineOption input({"i", "input"}, "Clockify XLSX file", "clockify-report");
    QCommandLineOption workspace({"w", "workspace"}, "Clockify workspace id", "clockify-workspace");
    QCommandLineOption token("token", "Clockify token", "clockify-token");
    QCommandLineOption output({"o", "output"}, "output directory", "directory");

    QCommandLineParser parser;
    parser.setApplicationDescription(app.applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({ technician, region, input });
#ifdef QT_NETWORK_LIB
    parser.addOptions({ workspace, token });
#endif
    parser.addOption(output);
    parser.process(app);

    ActivitySheetReader reader;
    const QString dir = parser.value("output");

    auto completeSheet = [&parser, &technician, &region](ActivitySheet &sheet) {
        sheet.technician = parser.value(technician).toUpper();
        sheet.region = parser.value(region).toUpper();
    };

    if (parser.isSet(input)) {
        ActivitySheet sheet = reader.read(parser.value(input));
        completeSheet(sheet);
        return (write(dir.isEmpty() ? "." : dir, sheet) ? 0 : 1);
    }

#ifdef QT_NETWORK_LIB
    if (parser.isSet(workspace) && parser.isSet(token)) {
        out << "Processing..." << Qt::endl;

        Clockify c;
        c.setToken(parser.value(token));
        c.getReport(parser.value(workspace), [&out, &reader, &completeSheet, &dir, &app](QIODevice *device) {
            if (device == nullptr) {
                out << "Error during download" << Qt::endl;
                app.quit();
            } else {
                ActivitySheet sheet = reader.read(device);
                completeSheet(sheet);
                app.exit(write(dir.isEmpty() ? "." : dir, sheet) ? 0 : 1);
            }
        });

        return app.exec();
    }
#endif

    out << "no input file specified !" << Qt::endl;
    return 2;
}

bool write(const QString &dir, const ActivitySheet &sheet)
{
    QString technician = sheet.technician;
    technician.replace(' ', '_');
    QString fileName = "IDCAM_Fiche_Journaliere_" + technician + '_' + sheet.date.toString("dd-MM-yyyy") + ".xlsx";

    static ActivitySheetWriter writer;
    return writer.save(dir + '/' + fileName, sheet);
}
