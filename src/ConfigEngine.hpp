#pragma once

#include <QtCore/QHash>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include "private/node.h"

class JsonQObject;
class QQmlEngine;

class ConfigEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* config READ config NOTIFY configChanged)
    Q_PROPERTY(StateFlags state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

    Q_PROPERTY(bool globalConfigModified READ globalConfigModified NOTIFY globalConfigModifiedChanged)
    Q_PROPERTY(bool themeOverlayConfigModified READ themeOverlayConfigModified NOTIFY themeOverlayConfigModifiedChanged)
    Q_PROPERTY(bool userConfigModified READ userConfigModified NOTIFY userConfigModifiedChanged)
    Q_PROPERTY(bool projectConfigModified READ projectConfigModified NOTIFY projectConfigModifiedChanged)

public:
    enum ConfigLevel {
        Global = 0,
		ThemeOverlay,
        User,
        Project,
        LevelsCount,
        MergeLevels = -1
    };
    Q_ENUM(ConfigLevel)

    enum StateFlag {
        Error                      = 0x001,
        GlobalConfigLoaded         = 0x002,
        ThemeOverlayConfigLoaded   = 0x004,
        UserConfigLoaded           = 0x008,
        ProjectConfigLoaded        = 0x010,
        GlobalConfigModified       = 0x020,
        ThemeOverlayConfigModified = 0x040,
        UserConfigModified         = 0x080,
        ProjectConfigModified      = 0x100
    };
    Q_DECLARE_FLAGS(StateFlags, StateFlag);
    Q_FLAG(StateFlags);

    ConfigEngine(QObject *parent = nullptr);

    virtual ~ConfigEngine();

    QObject *config() const;

    void setQmlEngine(QQmlEngine *qmlEngine);

    bool globalConfigModified() const;
    bool themeOverlayConfigModified() const;
    bool userConfigModified() const;
    bool projectConfigModified() const;

    const StateFlags &state() const;

    const QString &errorString() const;
    void setErrorString(const QString &errorString);

public slots:
    void loadConfig(const QUrl &url, ConfigLevel level = Global);
    void loadData(const QByteArray &data, ConfigLevel level = Global);
    void writeConfig(const QUrl &url, ConfigLevel level = User);
    void unloadConfig(ConfigLevel level);
    void clear();
    void setProperty(const QString &key, QVariant value, ConfigLevel level = User);
    QVariant getProperty(const QString &key, ConfigLevel level = Global);

signals:
    void configChanged();
    void globalConfigModifiedChanged();
    void themeOverlayConfigModifiedChanged();
    void userConfigModifiedChanged();
    void projectConfigModifiedChanged();
    void stateChanged();
    void warning(QString message);

    void errorStringChanged();

private:
    friend class JsonQObject;
    friend struct Node;

	void resetContextProperty();
    Node *getNodeHelper(const QString &key, int &indexOfProperty);

    Node m_root;
    QQmlEngine *m_qmlEngine = nullptr;
    StateFlags m_state;

    void updateTree(ConfigLevel level, QJsonObject data);
    void setModifiedFlag(ConfigLevel level, bool on = true);
    void setConfigLoadedFlag(ConfigLevel level, bool on = true);

    QString m_errorString;
};
