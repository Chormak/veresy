/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QMessageLogContext>

class Logger {
public:
    enum class Level {
        Debug,
        Info,
        Warning,
        Error
    };

    static void init();
    static void log(Level level, const QString& message, const QMessageLogContext& context);

private:
    static QString levelToString(Level level);
};

#endif
