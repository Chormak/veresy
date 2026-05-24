/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "OrdersView.h"
#include <QHeaderView>

OrdersView::OrdersView(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void OrdersView::setupUi() {
  auto *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  auto *contentLayout = new QVBoxLayout();
  m_view = new OrderTableView(this);
  contentLayout->addWidget(m_view, 3);

  auto *detailGroupBox = new QGroupBox("Детальна інформація про замовлення", this);
  auto *detailLayout = new QHBoxLayout(detailGroupBox);

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

  contentLayout->addWidget(detailGroupBox, 1);
  mainLayout->addLayout(contentLayout);

  m_proxyModel = new QSortFilterProxyModel(this);
  m_proxyModel->setSourceModel(m_view->model());
  m_view->setModel(m_proxyModel);

  m_view->horizontalHeader()->setSortIndicatorShown(true);
  m_view->setSortingEnabled(true);
  m_view->sortByColumn(0, Qt::DescendingOrder);

  auto *boardLayout = new QHBoxLayout();
  m_colCreated = new QListWidget(this);
  m_colInProgress = new QListWidget(this);
  m_colWaitingParts = new QListWidget(this);
  m_colDone = new QListWidget(this);

  boardLayout->addWidget(m_colCreated);
  boardLayout->addWidget(m_colInProgress);
  boardLayout->addWidget(m_colWaitingParts);
  boardLayout->addWidget(m_colDone);
  mainLayout->addLayout(boardLayout);

  connect(m_view, &OrderTableView::statusChanged, this, &OrdersView::statusChanged);
  connect(m_view, &OrderTableView::deleteRequested, this, &OrdersView::deleteRequested);
  connect(m_view, &QTableView::doubleClicked, this, &OrdersView::doubleClicked);

  connect(m_view->selectionModel(), &QItemSelectionModel::currentChanged, this, &OrdersView::onOrderSelectionChanged);
}

void OrdersView::updateData(const std::vector<Order>& filteredOrders, bool isFilteringActive, const std::vector<Order>& allOrders) {
    m_currentFilteredOrders = filteredOrders;
    m_view->updateData(filteredOrders, isFilteringActive);
    updateWorkflowBoard(allOrders);
}

void OrdersView::updateWorkflowBoard(const std::vector<Order>& allOrders) {
    m_colCreated->clear();
    m_colInProgress->clear();
    m_colWaitingParts->clear();
    m_colDone->clear();

    for (const auto& order : allOrders) {
        QString icon = statusToIcon(order.status);
        QString itemText = QString("%1 #%2 | %3\nПроблема: %4").arg(icon).arg(order.id).arg(order.device).arg(order.issue);
        auto *item = new QListWidgetItem(itemText);

        switch (order.status) {
            case OrderStatus::Created: m_colCreated->addItem(item); break;
            case OrderStatus::InProgress: m_colInProgress->addItem(item); break;
            case OrderStatus::WaitingParts: 
                item->setForeground(getStatusColor(OrderStatus::WaitingParts));
                m_colWaitingParts->addItem(item); 
                break;
            case OrderStatus::Done: m_colDone->addItem(item); break;
            default: delete item; break;
        }
    }
}

void OrdersView::onOrderSelectionChanged(const QModelIndex &currentProxyIndex, const QModelIndex &) {
    m_historyList->clear();
    if (!currentProxyIndex.isValid()) {
        m_lblDetailId->setText("-"); m_lblDetailClient->setText("-"); m_lblDetailDevice->setText("-");
        m_lblDetailIssue->setText("-"); m_lblDetailStatus->setText("-"); m_lblDetailDate->setText("-");
        return;
    }

    QModelIndex originalIndex = m_proxyModel->mapToSource(currentProxyIndex);
    int row = originalIndex.row();
    if (row >= m_currentFilteredOrders.size()) return;

    const Order& order = m_currentFilteredOrders[row];
    m_lblDetailId->setText(QString("#%1").arg(order.id));
    m_lblDetailClient->setText(order.clientName);
    m_lblDetailDevice->setText(order.device);
    m_lblDetailIssue->setText(order.issue);
    m_lblDetailStatus->setText(statusToString(order.status));
    m_lblDetailDate->setText(order.createdAt.toString("dd.MM.yyyy HH:mm:ss"));

    std::vector<HistoryRecord> history;
    emit historyRequested(order.id, history);

    for (const auto& record : history) {
        QString timeStr = record.timestamp.toString("HH:mm");
        QString logLine = QString("[%1] [@%2] %3 → %4").arg(timeStr).arg(record.performed_by).arg(statusToString(record.oldStatus)).arg(statusToString(record.newStatus));
        if (!record.comment.isEmpty()) logLine += " (" + record.comment + ")";
        m_historyList->addItem(logLine);
    }
}
