/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef WORKFLOWLISTWIDGET_H
#define WORKFLOWLISTWIDGET_H

#include <QListWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include "../../core/orders/Order.h"

class WorkflowListWidget : public QListWidget {
    Q_OBJECT
public:
    explicit WorkflowListWidget(OrderStatus columnStatus, QWidget *parent = nullptr);

signals:
    void orderDropped(int orderId, OrderStatus targetStatus);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void setupUi();
private:
    OrderStatus m_columnStatus;
};

#endif
