/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>
#include "../users/User.h"

class SessionManager {
public:
    static SessionManager& instance();

    void setSession(const User& user) { m_currentUser = user; }
    void clearSession() { m_currentUser = User(); }

    User currentUser() const { return m_currentUser; }
    bool isLoggedIn() const { return m_currentUser.id > 0; }

    bool isAdmin() const { return m_currentUser.role == "admin"; }
    bool isManager() const { return m_currentUser.role == "manager"; }
    bool isTechnician() const { return m_currentUser.role == "technician" || m_currentUser.role == "master"; }

private:
    SessionManager() = default;
    ~SessionManager() = default;
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;

    User m_currentUser;
};

#endif
