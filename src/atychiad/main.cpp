/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  SPDX-FileCopyrightText: 2025 Thomas Duckworth <tduck@filotimoproject.org>
 */

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusObjectPath>
#include <QDebug>
#include <QProcess>
#include <QString>

#include "directories.h"

using namespace Qt::Literals::StringLiterals;

class AtychiaDaemon : public QObject
{
    Q_OBJECT

public:
    explicit AtychiaDaemon(QObject *parent = nullptr)
        : QObject(parent)
    {
        auto bus = QDBusConnection::systemBus();
        if (!bus.isConnected()) {
            qCritical("Cannot connect to the system D-Bus");
            QCoreApplication::quit();
            return;
        }

        if (!bus.connect(u"org.freedesktop.login1"_s,
                         u"/org/freedesktop/login1"_s,
                         u"org.freedesktop.login1.Manager"_s,
                         u"SecureAttentionKey"_s,
                         this,
                         SLOT(onSecureAttentionKey(QString, QDBusObjectPath)))) {
            qCritical("Failed to connect to SecureAttentionKey signal");
            QCoreApplication::quit();
        }
    }

public Q_SLOTS:
    void onSecureAttentionKey(const QString &seat_id, const QDBusObjectPath &seat_object_path)
    {
        qDebug() << "SecureAttentionKey signal received for seat" << seat_id << "with seat object path" << seat_object_path.path();
        qDebug() << "Launching atychia-launch via pkexec...";
        // The installed policy indicates that only the active user can run this.
        // This sucks, but it is what it is.
        QProcess::startDetached(u"pkexec"_s, {u"%1/atychia-launch"_s.arg(BINDIR)});
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    AtychiaDaemon daemon;
    return app.exec();
}

#include "main.moc"
