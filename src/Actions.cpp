/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  SPDX-FileCopyrightText: 2025 Thomas Duckworth <tduck@filotimoproject.org>
 */

#include "Actions.h"
#include "directories.h"

#include <KLocalizedString>
#include <QCoreApplication>
#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QMessageBox>
#include <QProcess>
#include <QThread>
#include <csignal>
#include <pwd.h>
#include <unistd.h>

using namespace Qt::Literals::StringLiterals;

static void returnToTTYNumberAndQuit(uint32_t ttyNum, uint32_t seatNumber)
{
    QDBusInterface seatd{u"org.freedesktop.login1"_s,
                         u"/org/freedesktop/login1/seat/seat%1"_s.arg(seatNumber),
                         u"org.freedesktop.login1.Seat"_s,
                         QDBusConnection::systemBus()};

    seatd.callWithArgumentList(QDBus::Block, u"SwitchTo"_s, {ttyNum});

    QCoreApplication::exit(0);
}

static bool canDoAction(QString actionText)
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};
    const auto message = logind.callWithArgumentList(QDBus::Block, actionText, {});
    QDBusPendingReply<QString> canDoAction = message;

    Q_ASSERT(canDoAction.isFinished());
    if (canDoAction.isError()) {
        const auto error = canDoAction.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        return false;
    }

    if (canDoAction.value() == u"yes"_s) {
        return true;
    }

    return false;
}

// Return to desktop
void Return::execute()
{
    returnToTTYNumberAndQuit(m_ctx->ttyNumber(), m_ctx->seatNumber());
}

// Logout
void Logout::execute()
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};
    const auto message = logind.callWithArgumentList(QDBus::Block,
                                                     u"KillUser"_s,
                                                     {
                                                         m_ctx->userUid(),
                                                         SIGTERM,
                                                     });

    QDBusPendingReply<QString> killedUser = message;

    Q_ASSERT(killedUser.isFinished());
    if (killedUser.isError()) {
        const auto error = killedUser.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        Q_EMIT errorOccurred(error.name(), error.message());
    }

    returnToTTYNumberAndQuit(1, m_ctx->seatNumber());
}

// Power off
void PowerOff::execute()
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};

    QDBusPendingReply<> powerOff = logind.callWithArgumentList(QDBus::Block,
                                                               u"PowerOff"_s,
                                                               {
                                                                   true,
                                                               });

    Q_ASSERT(powerOff.isFinished());
    if (powerOff.isError()) {
        const auto error = powerOff.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        Q_EMIT errorOccurred(error.name(), error.message());
        return;
    }

    QCoreApplication::exit(0);
}

bool PowerOff::canExecute() const
{
    return canDoAction(u"CanPowerOff"_s);
}

// Restart
void Restart::execute()
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};

    QDBusPendingReply<> restart = logind.callWithArgumentList(QDBus::Block,
                                                              u"Reboot"_s,
                                                              {
                                                                  true,
                                                              });

    Q_ASSERT(restart.isFinished());
    if (restart.isError()) {
        const auto error = restart.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        Q_EMIT errorOccurred(error.name(), error.message());
        return;
    }

    QCoreApplication::exit(0);
}

bool Restart::canExecute() const
{
    return canDoAction(u"CanReboot"_s);
}

// Restart to firmware setup
void RestartToFirmwareSetup::execute()
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};

    const auto message = logind.callWithArgumentList(QDBus::Block,
                                                     u"SetRebootToFirmwareSetup"_s,
                                                     {
                                                         true,
                                                     });
    QDBusPendingReply<QString> setRebootToFirmwareSetup = message;

    Q_ASSERT(setRebootToFirmwareSetup.isFinished());
    if (setRebootToFirmwareSetup.isError()) {
        const auto error = setRebootToFirmwareSetup.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        Q_EMIT errorOccurred(error.name(), error.message());
        return;
    }

    QDBusPendingReply<> reboot = logind.callWithArgumentList(QDBus::Block,
                                                             u"Reboot"_s,
                                                             {
                                                                 true,
                                                             });
    Q_ASSERT(reboot.isFinished());
    if (reboot.isError()) {
        const auto error = reboot.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        Q_EMIT errorOccurred(error.name(), error.message());
        return;
    }

    QCoreApplication::exit(0);
}

bool RestartToFirmwareSetup::canExecute() const
{
    return canDoAction(u"CanRebootToFirmwareSetup"_s);
}

// Launch a terminal
void LaunchKonsole::execute()
{
    struct passwd *pwd = getpwuid(m_ctx->userUid());
    QStringList args{
        u"-e"_s,
        u"/%1/su"_s.arg(QString::fromUtf8(BINDIR)),
        u"-"_s,
        u"%1"_s.arg(QString::fromUtf8(pwd->pw_name)),
    };
    QProcess process;
    process.startDetached(u"/%1/konsole"_s.arg(QString::fromUtf8(BINDIR)), args);
}
