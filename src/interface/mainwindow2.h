#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QDomElement>
#include <QMainWindow>

class QActionGroup;
class Editor;
class ScribbleArea;
class Object;
class Palette;
class DisplayOptionDockWidget;
class ToolOptionDockWidget;
class TimeLine;
class ToolSetWidget;
class Preferences;
template<typename T> class QList;


namespace Ui
{
class MainWindow2;
}



class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow2(QWidget* parent = 0);
    ~MainWindow2();

    void addRecentFile(QString filePath);

    // Data Model
    Object* m_object;

    // UI: central view
    Editor* editor;

    // UI: central Drawing Area
    ScribbleArea* m_pScribbleArea;

    // UI: Dock widgets
    Palette* m_colorPalette;
    DisplayOptionDockWidget* m_displayOptionWidget;
    ToolOptionDockWidget*    m_toolOptionWidget;
    TimeLine* m_pTimeLine;
    ToolSetWidget* m_toolSet;

    // Other windows
    Preferences* m_pPreferences;

private:
    Ui::MainWindow2* ui;

    // Old code migration
public slots:
    void setOpacity(int opacity);
    void undoActSetText(void);
    void undoActSetEnabled(void);
    void newDocument();
    void openDocument();
    bool saveAsNewDocument();
    void saveDocument();
    bool maybeSave();
    void showPreferences();
    bool openObject(QString strFilename);
    void resetToolsSettings();

private slots:
    void exportFile();
    bool saveObject(QString strSavedFilename);
    void openRecent();
    void dockAllPalettes();
    void helpBox();
    void aboutPencil();

    void loadAllShortcuts();
    void unloadAllShortcuts();

    void importPalette();
    void exportPalette();

    // XML save/load
    QDomElement createDomElement(QDomDocument& doc);
    bool loadDomElement(QDomElement element,  QString filePath);

private:
    void arrangePalettes();
    void makePreferenceConnections();
    void makeTimeLineConnections();
    void createMenus();
    void addToMenu(QObject* plugin, const QString text, QMenu* menu, const char* member, QActionGroup* actionGroup = 0);

    void closeEvent(QCloseEvent*);


    void readSettings();
    void writeSettings();
    QString sc(QString);

    QString path;
    QMenu* openRecentMenu;
};

#endif // MAINWINDOW2_H
