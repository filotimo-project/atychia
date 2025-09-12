/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  SPDX-FileCopyrightText: 2025 Thomas Duckworth <tduck@filotimoproject.org>
 */

#pragma once

#include <KLocalizedString>
#include <QAbstractListModel>
#include <QDBusInterface>
#include <QErrorMessage>
#include <QObject>

#include "Action.h"

class Return : public Action
{
    Q_OBJECT
    Q_PROPERTY(QString buttonText READ buttonText CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString iconSource READ iconSource CONSTANT)
    Q_PROPERTY(bool canExecute READ canExecute CONSTANT)

public:
    explicit Return(QObject *parent = nullptr, ActionContext *ctx = nullptr)
        : Action(parent, ctx)
    {
    }

    QString buttonText() const override
    {
        return i18n("Return");
    }
    QString name() const override
    {
        return i18n("Return to desktop");
    }
    QString description() const override
    {
        return i18n("Exits desktop recovery and returns to the desktop.");
    }
    QString iconSource() const override
    {
        return u"desktop-symbolic"_s;
    }

    bool canExecute() const override
    {
        return true;
    }
    Q_INVOKABLE void execute() override;

    Q_SIGNAL void errorOccurred(QString name, QString description) const;
};

class Logout : public Action
{
    Q_OBJECT
    Q_PROPERTY(QString buttonText READ buttonText CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString iconSource READ iconSource CONSTANT)
    Q_PROPERTY(bool canExecute READ canExecute CONSTANT)

public:
    explicit Logout(QObject *parent = nullptr, ActionContext *ctx = nullptr)
        : Action(parent, ctx)
    {
    }

    QString buttonText() const override
    {
        return i18n("Log out");
    }
    QString name() const override
    {
        return i18n("Log out of current user");
    }
    QString description() const override
    {
        return i18n("Immediately logs out of current user. All unsaved work will be lost.");
    }
    QString iconSource() const override
    {
        return u"system-log-out-symbolic"_s;
    }

    bool canExecute() const override
    {
        return true;
    }
    Q_INVOKABLE void execute() override;

    Q_SIGNAL void errorOccurred(QString name, QString description) const;
};

class PowerOff : public Action
{
    Q_OBJECT
    Q_PROPERTY(QString buttonText READ buttonText CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString iconSource READ iconSource CONSTANT)
    Q_PROPERTY(bool canExecute READ canExecute CONSTANT)

public:
    explicit PowerOff(QObject *parent = nullptr, ActionContext *ctx = nullptr)
        : Action(parent, ctx)
    {
    }

    QString buttonText() const override
    {
        return i18n("Power off");
    }
    QString name() const override
    {
        return i18n("Power off computer");
    }
    QString description() const override
    {
        return i18n("Immediately powers off the computer. All unsaved work will be lost.");
    }
    QString iconSource() const override
    {
        return u"system-shutdown-symbolic"_s;
    }

    bool canExecute() const override;
    Q_INVOKABLE void execute() override;

    Q_SIGNAL void errorOccurred(QString name, QString description) const;
};

class Restart : public Action
{
    Q_OBJECT
    Q_PROPERTY(QString buttonText READ buttonText CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString iconSource READ iconSource CONSTANT)
    Q_PROPERTY(bool canExecute READ canExecute CONSTANT)

public:
    explicit Restart(QObject *parent = nullptr, ActionContext *ctx = nullptr)
        : Action(parent, ctx)
    {
    }

    QString buttonText() const override
    {
        return i18n("Restart");
    }
    QString name() const override
    {
        return i18n("Restart computer");
    }
    QString description() const override
    {
        return i18n("Immediately restarts the computer. All unsaved work will be lost.");
    }
    QString iconSource() const override
    {
        return u"system-reboot-symbolic"_s;
    }

    bool canExecute() const override;
    Q_INVOKABLE void execute() override;

    Q_SIGNAL void errorOccurred(QString name, QString description) const;
};

class LaunchKonsole : public Action
{
    Q_OBJECT
    Q_PROPERTY(QString buttonText READ buttonText CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString iconSource READ iconSource CONSTANT)
    Q_PROPERTY(bool canExecute READ canExecute CONSTANT)

public:
    explicit LaunchKonsole(QObject *parent = nullptr, ActionContext *ctx = nullptr)
        : Action(parent, ctx)
    {
    }

    QString buttonText() const override
    {
        return i18n("Launch");
    }
    QString name() const override
    {
        return i18n("Open a terminal window");
    }
    QString description() const override
    {
        return i18n("Opens a Konsole window which logs into your account.");
    }
    QString iconSource() const override
    {
        return u"utilities-terminal-symbolic"_s;
    }

    bool canExecute() const override
    {
        return true;
    }
    Q_INVOKABLE void execute() override;

    Q_SIGNAL void errorOccurred(QString name, QString description) const;
};

class LaunchSystemMonitor : public Action
{
    Q_OBJECT
    Q_PROPERTY(QString buttonText READ buttonText CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString iconSource READ iconSource CONSTANT)
    Q_PROPERTY(bool canExecute READ canExecute CONSTANT)

public:
    explicit LaunchSystemMonitor(QObject *parent = nullptr, ActionContext *ctx = nullptr)
        : Action(parent, ctx)
    {
    }

    QString buttonText() const override
    {
        return i18n("Launch");
    }
    QString name() const override
    {
        return i18n("Open System Monitor");
    }
    QString description() const override
    {
        return i18n("Opens the System Monitor application.");
    }
    QString iconSource() const override
    {
        return u"view-process-all-symbolic"_s;
    }

    bool canExecute() const override
    {
        return true;
    }
    Q_INVOKABLE void execute() override;

    Q_SIGNAL void errorOccurred(QString name, QString description) const;
};

// The model for all the actions as well as direct access to specific actions
class ActionModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Return *returnAction READ returnToDesktop CONSTANT)
    Q_PROPERTY(Logout *logoutAction READ logout CONSTANT)
    Q_PROPERTY(PowerOff *powerOffAction READ powerOff CONSTANT)
    Q_PROPERTY(Restart *restartAction READ restart CONSTANT)
    Q_PROPERTY(LaunchKonsole *launchKonsoleAction READ launchKonsole CONSTANT)
    Q_PROPERTY(LaunchSystemMonitor *launchSystemMonitorAction READ launchSystemMonitor CONSTANT)

public:
    enum ActionRoles {
        ButtonTextRole = Qt::UserRole + 1,
        NameRole,
        DescriptionRole,
        IconSourceRole,
        CanExecuteRole,
        ActionObjectRole
    };

    explicit ActionModel(QObject *parent = nullptr, ActionContext *ctx = nullptr)
        : QAbstractListModel(parent)
        , m_returnAction(new Return(this, ctx))
        , m_logoutAction(new Logout(this, ctx))
        , m_powerOffAction(new PowerOff(this, ctx))
        , m_restartAction(new Restart(this, ctx))
        , m_launchKonsoleAction(new LaunchKonsole(this, ctx))
        , m_launchSystemMonitorAction(new LaunchSystemMonitor(this, ctx))
    {
        // Initialise the list with the members
        // Any new Action needs to have a member in this class, and be added to this list
        m_actions = {m_returnAction, m_logoutAction, m_powerOffAction, m_restartAction, m_launchKonsoleAction, m_launchSystemMonitorAction};

        // Connect the errorOccurred signal from each action to the model
        for (Action *action : m_actions) {
            connect(action, &Action::errorOccurred, this, [this](QString name, QString description) {
                Q_EMIT errorOccurred(name, description);
            });
        }
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        if (parent.isValid())
            return 0;
        return m_actions.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_actions.size())
            return QVariant();

        const Action *action = m_actions.at(index.row());
        switch (role) {
        case ButtonTextRole:
            return action->buttonText();
        case NameRole:
            return action->name();
        case DescriptionRole:
            return action->description();
        case IconSourceRole:
            return action->iconSource();
        case CanExecuteRole:
            return action->canExecute();
        // return the action object itself
        // so that we can call execute() on it in qml
        // I do not like this
        case ActionObjectRole:
            return QVariant::fromValue(action);
        default:
            return QVariant();
        }
    }

    QHash<int, QByteArray> roleNames() const override
    {
        QHash<int, QByteArray> roles;
        roles[ButtonTextRole] = "buttonText";
        roles[NameRole] = "name";
        roles[DescriptionRole] = "description";
        roles[IconSourceRole] = "iconSource";
        roles[CanExecuteRole] = "canExecute";
        roles[ActionObjectRole] = "actionObject";
        return roles;
    }

    Return *returnToDesktop() const
    {
        return m_returnAction;
    }
    Logout *logout() const
    {
        return m_logoutAction;
    }
    PowerOff *powerOff() const
    {
        return m_powerOffAction;
    }
    Restart *restart() const
    {
        return m_restartAction;
    }
    LaunchKonsole *launchKonsole() const
    {
        return m_launchKonsoleAction;
    }
    LaunchSystemMonitor *launchSystemMonitor() const
    {
        return m_launchSystemMonitorAction;
    }

    Q_SIGNAL void errorOccurred(QString name, QString description) const;

private:
    QList<Action *> m_actions;

    Return *m_returnAction;
    Logout *m_logoutAction;
    PowerOff *m_powerOffAction;
    Restart *m_restartAction;
    LaunchKonsole *m_launchKonsoleAction;
    LaunchSystemMonitor *m_launchSystemMonitorAction;
};
