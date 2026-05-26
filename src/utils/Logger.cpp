/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "Logger.h"
#include <QDateTime>
#include <QFileInfo>
#include <iostream>
#include <QDir>
#include <QFile>
#include <QTextStream>

void qtMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    Logger::Level level = Logger::Level::Info;

    switch (type) {
        case QtDebugMsg:    level = Logger::Level::Debug; break;
        case QtInfoMsg:     level = Logger::Level::Info; break;
        case QtWarningMsg:  level = Logger::Level::Warning; break;
        case QtCriticalMsg:
        case QtFatalMsg:    level = Logger::Level::Error; break;
    }

    Logger::log(level, msg, context);
}

void Logger::init() {
    qInstallMessageHandler(qtMessageOutput);
}

void Logger::log(Level level, const QString& message, const QMessageLogContext& context) {
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");

    QString fileName = context.file ? QFileInfo(context.file).fileName() : "UnknownFile";
    int line = context.line;

    QString consoleLine = QString("[%1] [%2] [%3:%4] %5")
                          .arg(timestamp)
                          .arg(levelToString(level))
                          .arg(fileName)
                          .arg(line)
                          .arg(message);

    QString fileLine = QString("[%1] [%2] %3")
                       .arg(timestamp)
                       .arg(levelToString(level))
                       .arg(message);

    if (level == Level::Error) {
        std::cerr << consoleLine.toStdString() << std::endl;
    } else {
        std::cout << consoleLine.toStdString() << std::endl;
    }

    QDir dir;
    if (!dir.exists("logs")) {
        dir.mkdir("logs");
    }

    QFile logFile("logs/veresy.log");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&logFile);
        stream.setEncoding(QStringConverter::Utf8);
        stream << fileLine << "\n";
        logFile.close();
    }
}

QString Logger::levelToString(Level level) {
    switch (level) {
        case Level::Debug:   return "DEBUG";
        case Level::Info:    return "INFO ";
        case Level::Warning: return "WARN ";
        case Level::Error:   return "ERROR";
    }
    return "UNKNOWN";
}
