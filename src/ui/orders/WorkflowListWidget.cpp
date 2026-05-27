/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "WorkflowListWidget.h"
#include <QMimeData>
#include <QDataStream>

WorkflowListWidget::WorkflowListWidget(OrderStatus columnStatus, QWidget *parent)
    : QListWidget(parent), m_columnStatus(columnStatus)
{
    setupUi();
}

void WorkflowListWidget::setupUi() {
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWordWrap(true);
}

void WorkflowListWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->acceptProposedAction();
    }
}

void WorkflowListWidget::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void WorkflowListWidget::dropEvent(QDropEvent *event) {
    QListWidget *sourceList = qobject_cast<QListWidget*>(event->source());
    if (!sourceList) return;

    QListWidgetItem *currentItem = sourceList->currentItem();
    if (!currentItem) return;

    int orderId = currentItem->data(Qt::UserRole).toInt();

    QListWidget::dropEvent(event);

    emit orderDropped(orderId, m_columnStatus);
}
