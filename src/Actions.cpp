/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  SPDX-FileCopyrightText: 2024 Thomas Duckworth <tduck973564@gmail.com>
 */

#include "Actions.h"
#include "directories.h"

#include <KLocalizedString>
#include <QCoreApplication>
#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QProcess>
#include <csignal>

using namespace Qt::Literals::StringLiterals;

Actions::Actions(QObject *parent, QString tty_number, QString user_uid, QString session_id)
    : QObject(parent)
{
    ttyNumber = tty_number;
    userUid = user_uid;
    sessionId = session_id;
}

void Actions::returnToTTYNumber(QString ttyNum) const
{
    // TODO This should also be done programmatically
    QStringList args{ttyNum};
    QProcess process;
    process.startDetached(u"%1/chvt"_s.arg(QString::fromUtf8(BINDIR)), args);
    process.waitForFinished();
    qInfo().noquote() << "returnToTTYNumber() outputted: " << process.readAllStandardOutput();
}

void Actions::returnToTTYAndQuit() const
{
    returnToTTYNumber(ttyNumber);
    QCoreApplication::quit();
}

void Actions::logout() const
{ // TODO
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};
    const auto message = logind.callWithArgumentList(QDBus::Block,
                                                     u"KillSession"_s,
                                                     {
                                                         sessionId,
                                                         u"all"_s,
                                                         SIGTERM,
                                                     });
    QDBusPendingReply<QString> killedSession = message;

    Q_ASSERT(killedSession.isFinished());
    if (killedSession.isError()) {
        const auto error = killedSession.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        return;
    }

    returnToTTYNumber(u"1"_s);
    QCoreApplication::quit();
}

void Actions::relaunchPlasma() const
{
    /* this won't work
     *   QDBusInterface systemd { u"org.freedesktop.systemd1"_s, u"/org/freedesktop/systemd1"_s, u"org.freedesktop.systemd1.Manager"_s,
QDBusConnection::systemBus()};
     *   const auto message = systemd.callWithArgumentList(QDBus::Block, u"TryRestartUnit"_s, {u"plasma_plasmashell"_s,});
     *   QDBusPendingReply< QString > unitRestarted = message;
     *
     *   Q_ASSERT(unitRestarted.isFinished());
     *   if (unitRestarted.isError()) {
     *       const auto error = unitRestarted.error();
     *       qWarning().noquote()
     *       << i18n("Asynchronous call finished with error: %1 (%2)")
     *       .arg(error.name(), error.message());
     *       return;
     *
     *   returnToTTYAndTerminate();
}*/

    // TODO This is not good, dbus should be used but I don't know how to restart a user service.
    // Needs elevation
    QStringList args{u"shell"_s,
                     u"--uid=%1"_s.arg(userUid),
                     u"--setenv=SHELL=%1/bash"_s.arg(QString::fromUtf8(BINDIR)),
                     u".host"_s,
                     u"%1/bash"_s.arg(QString::fromUtf8(BINDIR)),
                     u"-lc"_s,
                     u"%1/atychia-restart-plasmashell"_s.arg(QString::fromUtf8(LIBEXECDIR))};
    QProcess process;
    process.startDetached(u"%1/machinectl"_s.arg(QString::fromUtf8(BINDIR)), args);
    process.waitForFinished();
    qInfo().noquote() << "relaunchPlasma() outputted: " << process.readAllStandardOutput();

    returnToTTYAndQuit();
}

void Actions::shutdown() const
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};
    const auto message = logind.callWithArgumentList(QDBus::Block, u"CanPowerOff"_s, {});
    QDBusPendingReply<QString> canPowerOff = message;

    Q_ASSERT(canPowerOff.isFinished());
    if (canPowerOff.isError()) {
        const auto error = canPowerOff.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
    }

    if (canPowerOff.value() == u"yes"_s) {
        QDBusPendingReply<> powerOff = logind.callWithArgumentList(QDBus::Block,
                                                                   u"PowerOff"_s,
                                                                   {
                                                                       true,
                                                                   });
        Q_ASSERT(powerOff.isFinished());
        if (powerOff.isError()) {
            const auto error = powerOff.error();
            qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        }

    } else {
        qCritical().noquote() << i18n("poweroff", u"Can't power off: CanPowerOff() result is %1"_s).arg(canPowerOff.value());
    }
}

void Actions::reboot() const
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};
    const auto message = logind.callWithArgumentList(QDBus::Block, u"CanReboot"_s, {});
    QDBusPendingReply<QString> canReboot = message;

    Q_ASSERT(canReboot.isFinished());
    if (canReboot.isError()) {
        const auto error = canReboot.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
    }

    if (canReboot.value() == u"yes"_s) {
        QDBusPendingReply<> reboot = logind.callWithArgumentList(QDBus::Block,
                                                                 u"Reboot"_s,
                                                                 {
                                                                     true,
                                                                 });
        Q_ASSERT(reboot.isFinished());
        if (reboot.isError()) {
            const auto error = reboot.error();
            qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        }

    } else {
        qCritical().noquote() << i18n("reboot", u"Can't reboot: CanReboot() result is %1"_s).arg(canReboot.value());
    }
}

void Actions::rebootToFirmwareSetup() const
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};
    const auto message = logind.callWithArgumentList(QDBus::Block, u"CanRebootToFirmwareSetup"_s, {});
    QDBusPendingReply<QString> canReboot = message;

    Q_ASSERT(canReboot.isFinished());
    if (canReboot.isError()) {
        const auto error = canReboot.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
    }

    if (canReboot.value() == u"yes"_s) {
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
        }

    } else {
        qCritical().noquote() << i18n("reboot", u"Can't reboot to firmware setup: CanRebootToFirmwareSetup() result is %1"_s).arg(canReboot.value());
    }
}
