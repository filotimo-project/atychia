/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2024 Thomas Duckworth <tduck973564@gmail.com>
*/

#include <QApplication>
#include <QtGlobal>

#include <QCommandLineParser>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QUrl>

#include "Actions.h"
#include "version-atychia.h"
#include <KAboutData>
#include <KColorScheme>
#include <KColorSchemeManager>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QAbstractItemModel>
#include <cstdint>
#include <ksharedconfig.h>

using namespace Qt::Literals::StringLiterals;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Default to org.kde.desktop style unless the user forces another style
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(u"org.kde.desktop"_s);
    }

    KColorSchemeManager *manager = KColorSchemeManager::instance();
    QModelIndex index = manager->indexForScheme(u"Filotimo Dark"_s);
    manager->activateScheme(index);

    KLocalizedString::setApplicationDomain("atychia");
    QCoreApplication::setOrganizationName(u"Filotimo Project"_s);

    KAboutData aboutData(
        // The program name used internally.
        u"atychia"_s,
        // A displayable program name string.
        i18nc("@title", "Desktop Recovery"),
        // The program version string.
        QStringLiteral(ATYCHIA_VERSION_STRING),
        // Short description of what the app does.
        i18n("Utility akin to the Ctrl-Alt-Delete menu on Windows, allowing a user to recover from a broken session without a terminal."),
        // The license this code is released under.
        KAboutLicense::GPL,
        // Copyright Statement.
        i18n("Thomas Duckworth (c) 2024"));
    aboutData.addAuthor(i18nc("@info:credit", "Thomas Duckworth"),
                        i18nc("@info:credit", "Author"),
                        u"tduck973564@gmail.com"_s,
                        u"https://github.com/filotimo-project"_s);
    aboutData.setTranslator(i18nc("NAME OF TRANSLATORS", "Your names"), i18nc("EMAIL OF TRANSLATORS", "Your emails"));
    KAboutData::setApplicationData(aboutData);
    QGuiApplication::setWindowIcon(QIcon::fromTheme(u"system-help"_s));

    QCommandLineParser parser;
    parser.setApplicationDescription(aboutData.shortDescription());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(
        u"tty"_s,
        i18nc("TTY should not be translated, number is a literal digit, and TTY is an environment to return to", "TTY number to return to."));
    parser.addPositionalArgument(u"uid"_s,
                                 i18nc("UID should not be translated, originating as in the user that the process comes from", "UID of originating user."));
    parser.addPositionalArgument(u"seat_number"_s, i18nc("Originating as in the user that the process comes from", "Seat number of originating user."));

    parser.process(app);

    const uint32_t ttyNumber = std::stoi(parser.positionalArguments()[0].toStdString());
    const uint32_t userUid = std::stoi(parser.positionalArguments()[1].toStdString());
    const uint32_t seatNumber = std::stoi(parser.positionalArguments()[2].toStdString());

    Actions actions = Actions(&app, ttyNumber, userUid, seatNumber);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance<Actions>("org.filotimoproject.atychia", 1, 0, "Actions", &actions);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.loadFromModule("org.filotimoproject.atychia", u"Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
