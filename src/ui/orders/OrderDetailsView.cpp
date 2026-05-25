/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "OrderDetailsView.h"

OrderDetailsView::OrderDetailsView(QWidget *parent) : QGroupBox("Детальна інформація про замовлення", parent) {
    setupUi();
}

void OrderDetailsView::setupUi() {
    auto *detailLayout = new QHBoxLayout(this);

    auto *textDetailsLayout = new QFormLayout();
    m_lblDetailId = new QLabel("-", this);
    m_lblDetailClient = new QLabel("-", this);
    m_lblDetailDevice = new QLabel("-", this);
    m_lblDetailIssue = new QLabel("-", this);
    m_lblDetailStatus = new QLabel("-", this);
    m_lblDetailDate = new QLabel("-", this);

    textDetailsLayout->addRow("Номер замовлення:", m_lblDetailId);
    textDetailsLayout->addRow("Клієнт:", m_lblDetailClient);
    textDetailsLayout->addRow("Пристрій:", m_lblDetailDevice);
    textDetailsLayout->addRow("Опис проблеми:", m_lblDetailIssue);
    textDetailsLayout->addRow("Поточний статус:", m_lblDetailStatus);
    textDetailsLayout->addRow("Дата створення:", m_lblDetailDate);
    detailLayout->addLayout(textDetailsLayout, 2);

    m_historyList = new QListWidget(this);
    m_historyList->addItem("Оберіть замовлення для перегляду історії");
    detailLayout->addWidget(m_historyList, 1);
}

void OrderDetailsView::showOrderDetails(const Order& order, const std::vector<HistoryRecord>& history) {
    m_historyList->clear();

    m_lblDetailId->setText(QString("#%1").arg(order.id));
    m_lblDetailClient->setText(order.clientName);
    m_lblDetailDevice->setText(order.device);
    m_lblDetailIssue->setText(order.issue);
    m_lblDetailStatus->setText(statusToString(order.status));
    m_lblDetailDate->setText(order.createdAt.toString("dd.MM.yyyy HH:mm:ss"));

    for (const auto& record : history) {
        QString timeStr = record.timestamp.toString("HH:mm");
        QString logLine = QString("[%1] [@%2] %3 → %4")
                          .arg(timeStr)
                          .arg(record.performed_by)
                          .arg(statusToString(record.oldStatus))
                          .arg(statusToString(record.newStatus));
        if (!record.comment.isEmpty()) {
            logLine += " (" + record.comment + ")";
        }
        m_historyList->addItem(logLine);
    }
}

void OrderDetailsView::clearDetails() {
    m_historyList->clear();
    m_historyList->addItem("Оберіть замовлення для перегляду історії");

    m_lblDetailId->setText("-");
    m_lblDetailClient->setText("-");
    m_lblDetailDevice->setText("-");
    m_lblDetailIssue->setText("-");
    m_lblDetailStatus->setText("-");
    m_lblDetailDate->setText("-");
}
