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

#include "treemodelmanager.h"

#include "qmt/model/mobject.h"
#include "qmt/model/mpackage.h"
#include "qmt/model/mrelation.h"
#include "qmt/model_controller/modelcontroller.h"
#include "qmt/model_controller/mselection.h"
#include "qmt/model_ui/treemodel.h"
#include "qmt/model_ui/modeltreeviewinterface.h"

#include <QItemSelection>

namespace qmt {

TreeModelManager::TreeModelManager(QObject *parent) :
    QObject(parent),
    m_treeModel(0),
    m_modelTreeView(0)
{
}

TreeModelManager::~TreeModelManager()
{
}

void TreeModelManager::setTreeModel(TreeModel *tree_model)
{
    m_treeModel = tree_model;
}

void TreeModelManager::setModelTreeView(ModelTreeViewInterface *model_tree_view)
{
    m_modelTreeView = model_tree_view;
}

bool TreeModelManager::isRootPackageSelected() const
{
    foreach (const QModelIndex &index, m_modelTreeView->getSelectedSourceModelIndexes()) {
        MObject *object = dynamic_cast<MObject *>(m_treeModel->getElement(index));
        if (object && !object->getOwner()) {
            return true;
        }
    }
    return false;
}

MObject *TreeModelManager::getSelectedObject() const
{
    MObject *object = 0;
    if (m_modelTreeView->getCurrentSourceModelIndex().isValid()) {
        MElement *element = m_treeModel->getElement(m_modelTreeView->getCurrentSourceModelIndex());
        if (element) {
            object = dynamic_cast<MObject *>(element);
        }
    }
    return object;
}

MPackage *TreeModelManager::getSelectedPackage() const
{
    if (m_modelTreeView->getCurrentSourceModelIndex().isValid())
    {
        MElement *element = m_treeModel->getElement(m_modelTreeView->getCurrentSourceModelIndex());
        QMT_CHECK(element);
        if (MPackage *package = dynamic_cast<MPackage *>(element)) {
            return package;
        } else if (MObject *object = dynamic_cast<MObject *>(element)) {
            package = dynamic_cast<MPackage *>(object->getOwner());
            if (package) {
                return package;
            }
        }
    }
    return m_treeModel->getModelController()->getRootPackage();
}

MSelection TreeModelManager::getSelectedObjects() const
{
    MSelection model_selection;
    foreach (const QModelIndex &index, m_modelTreeView->getSelectedSourceModelIndexes()) {
        MElement *element = m_treeModel->getElement(index);
        if (MObject *object = dynamic_cast<MObject *>(element)) {
            model_selection.append(object->getUid(), m_treeModel->getModelController()->getOwnerKey(object));
        } else if (MRelation *relation = dynamic_cast<MRelation *>(element)) {
            model_selection.append(relation->getUid(), m_treeModel->getModelController()->getOwnerKey(relation));
        }
    }
    return model_selection;
}

}
