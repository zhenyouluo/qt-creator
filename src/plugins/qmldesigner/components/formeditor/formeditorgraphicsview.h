/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#ifndef FORMEDITORGRAPHICSVIEW_H
#define FORMEDITORGRAPHICSVIEW_H

#include <QGraphicsView>

namespace QmlDesigner {

class FormEditorGraphicsView : public QGraphicsView
{
Q_OBJECT
public:
    explicit FormEditorGraphicsView(QWidget *parent = 0);

    void setRootItemRect(const QRectF &rect);
    QRectF rootItemRect() const;

    void activateCheckboardBackground();
    void activateColoredBackground(const QColor &color);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    bool m_isPanning;
    int m_panStartX, m_panStartY;
    QRectF m_rootItemRect;
};

} // namespace QmlDesigner

#endif // FORMEDITORGRAPHICSVIEW_H
