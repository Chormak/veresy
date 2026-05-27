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

  m_detailsView = new OrderDetailsView(this);
  contentLayout->addWidget(m_detailsView, 1);

  mainLayout->addLayout(contentLayout);

  m_proxyModel = new QSortFilterProxyModel(this);
  m_proxyModel->setSourceModel(m_view->model());
  m_view->setModel(m_proxyModel);

  m_view->horizontalHeader()->setSortIndicatorShown(true);
  m_view->setSortingEnabled(true);
  m_view->sortByColumn(0, Qt::DescendingOrder);

  auto *boardLayout = new QHBoxLayout();
  m_colCreated = new WorkflowListWidget(OrderStatus::Created, this);
  m_colInProgress = new WorkflowListWidget(OrderStatus::InProgress, this);
  m_colWaitingParts = new WorkflowListWidget(OrderStatus::WaitingParts, this);
  m_colDone = new WorkflowListWidget(OrderStatus::Done, this);

  auto lambdaBridge = [this](int id, OrderStatus stat) {
    emit orderDroppedOnBoard(id, stat);
  };
  connect(m_colCreated, &WorkflowListWidget::orderDropped, this, lambdaBridge);
  connect(m_colInProgress, &WorkflowListWidget::orderDropped, this, lambdaBridge);
  connect(m_colWaitingParts, &WorkflowListWidget::orderDropped, this, lambdaBridge);
  connect(m_colDone, &WorkflowListWidget::orderDropped, this, lambdaBridge);

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

        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, order.id);

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
    if (!currentProxyIndex.isValid()) {
        m_detailsView->clearDetails();
        return;
    }

    QModelIndex originalIndex = m_proxyModel->mapToSource(currentProxyIndex);
    int row = originalIndex.row();
    if (row >= m_currentFilteredOrders.size()) return;

    const Order& order = m_currentFilteredOrders[row];

    std::vector<HistoryRecord> history;
    emit historyRequested(order.id, history);
    m_detailsView->showOrderDetails(order, history);
}
