/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  SPDX-FileCopyrightText: 2025 Thomas Duckworth <tduck@filotimoproject.org>
 */

#include "Actions.h"
#include "directories.h"

#include <KIO/ApplicationLauncherJob>
#include <KLocalizedString>
#include <KService>
#include <QCoreApplication>
#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QMessageBox>
#include <QProcess>
#include <QThread>
#include <csignal>
#include <qcoreapplication.h>
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

static void switchToGreeter(uint32_t seatNumber)
{
    QDBusInterface displaymanager{u"org.freedesktop.DisplayManager"_s,
                                  u"/org/freedesktop/DisplayManager/Seat%1"_s.arg(seatNumber),
                                  u"org.freedesktop.DisplayManager.Seat"_s,
                                  QDBusConnection::systemBus()};

    displaymanager.callWithArgumentList(QDBus::Block, u"SwitchToGreeter"_s, {});

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
    switchToGreeter(m_ctx->seatNumber());

    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};
    const auto message = logind.callWithArgumentList(QDBus::Block,
                                                     u"TerminateUser"_s,
                                                     {
                                                         m_ctx->userUid(),
                                                     });

    QDBusPendingReply<QString> killedUser = message;

    Q_ASSERT(killedUser.isFinished());
    if (killedUser.isError()) {
        const auto error = killedUser.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        Q_EMIT errorOccurred(error.name(), error.message());
    }

    QCoreApplication::exit(0);
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

// Launch Konsole
void LaunchKonsole::execute()
{
    const KService::Ptr service = KService::serviceByDesktopName(u"org.kde.konsole"_s);
    KIO::ApplicationLauncherJob *job = new KIO::ApplicationLauncherJob(service, {});
    job->start();
}

// Launch System Monitor
void LaunchSystemMonitor::execute()
{
    const KService::Ptr service = KService::serviceByDesktopName(u"org.kde.plasma-systemmonitor"_s);
    KIO::ApplicationLauncherJob *job = new KIO::ApplicationLauncherJob(service, {});
    job->start();
}
