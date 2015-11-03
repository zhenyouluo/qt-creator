/***************************************************************************
**
** Copyright (C) 2015 Jochen Becher
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "dboundary.h"

#include "dvisitor.h"
#include "dconstvisitor.h"

namespace qmt {

DBoundary::DBoundary()
    : DElement()
{
}

DBoundary::DBoundary(const DBoundary &rhs)
    : DElement(rhs),
      m_text(rhs.m_text),
      m_pos(rhs.m_pos),
      m_rect(rhs.m_rect)
{
}

DBoundary::~DBoundary()
{
}

DBoundary &DBoundary::operator=(const DBoundary &rhs)
{
    if (this != &rhs) {
        DElement::operator=(rhs);
        m_text = rhs.m_text;
        m_pos = rhs.m_pos;
        m_rect = rhs.m_rect;
    }
    return *this;
}

void DBoundary::setText(const QString &text)
{
    m_text = text;
}

void DBoundary::setPos(const QPointF &pos)
{
    m_pos = pos;
}

void DBoundary::setRect(const QRectF &rect)
{
    m_rect = rect;
}

void DBoundary::accept(DVisitor *visitor)
{
    visitor->visitDBoundary(this);
}

void DBoundary::accept(DConstVisitor *visitor) const
{
    visitor->visitDBoundary(this);
}

}
