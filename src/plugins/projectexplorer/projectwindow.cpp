/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#include "projectwindow.h"

#include "doubletabwidget.h"

#include "project.h"
#include "environment.h"
#include "projectexplorer.h"
#include "projectexplorerconstants.h"
#include "iprojectproperties.h"
#include "session.h"
#include "projecttreewidget.h"
#include "runconfiguration.h"
#include "buildconfiguration.h"

#include <coreplugin/minisplitter.h>
#include <coreplugin/fileiconprovider.h>
#include <coreplugin/icore.h>
#include <coreplugin/ifile.h>
#include <extensionsystem/pluginmanager.h>
#include <utils/qtcassert.h>
#include <utils/styledbar.h>
#include <utils/stylehelper.h>

#include <QtGui/QApplication>
#include <QtGui/QBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QScrollArea>
#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QMenu>

using namespace ProjectExplorer;
using namespace ProjectExplorer::Internal;

namespace {
const int ICON_SIZE(64);

const int ABOVE_HEADING_MARGIN(10);
const int ABOVE_CONTENTS_MARGIN(4);
const int BELOW_CONTENTS_MARGIN(16);
}

///
// OnePixelBlackLine
///

class OnePixelBlackLine : public QWidget
{
public:
    OnePixelBlackLine(QWidget *parent)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setMinimumHeight(1);
        setMaximumHeight(1);
    }
    void paintEvent(QPaintEvent *e)
    {
        Q_UNUSED(e);
        QPainter p(this);
        p.fillRect(contentsRect(), QBrush(Utils::StyleHelper::borderColor()));
    }
};

///
// PanelsWidget
///

PanelsWidget::PanelsWidget(QWidget *parent) :
    QScrollArea(parent),
    m_root(new QWidget(this))
{
    // We want a 900px wide widget with and the scrollbar at the
    // side of the screen.
    m_root->setFixedWidth(900);
    // The layout holding the individual panels:
    m_layout = new QGridLayout(m_root);
    m_layout->setColumnMinimumWidth(0, ICON_SIZE + 4);
    m_layout->setSpacing(0);

    setWidget(m_root);
    setFrameStyle(QFrame::NoFrame);
    setWidgetResizable(true);
}

PanelsWidget::~PanelsWidget()
{
    qDeleteAll(m_panels);
}

/*
 * Add a widget with heading information into the grid
 * layout of the PanelsWidget.
 *
 *     ...
 * +--------+-------------------------------------------+ ABOVE_HEADING_MARGIN
 * | icon   | name                                      |
 * +        +-------------------------------------------+
 * |        | line                                      |
 * +--------+-------------------------------------------+ ABOVE_CONTENTS_MARGIN
 * |          widget (with contentsmargins adjusted!)   |
 * +--------+-------------------------------------------+ BELOW_CONTENTS_MARGIN
 */
void PanelsWidget::addPropertiesPanel(IPropertiesPanel *panel)
{
    QTC_ASSERT(panel, return);

    // icon:
    const int headerRow(m_layout->rowCount() - 1);
    m_layout->setRowStretch(headerRow, 0);

    if (!panel->icon().isNull()) {
        QLabel *iconLabel = new QLabel(m_root);
        iconLabel->setPixmap(panel->icon().pixmap(ICON_SIZE, ICON_SIZE));
        iconLabel->setContentsMargins(0, ABOVE_HEADING_MARGIN, 0, 0);
        m_layout->addWidget(iconLabel, headerRow, 0, 2, 1, Qt::AlignTop | Qt::AlignHCenter);
    }

    // name:
    QLabel *nameLabel = new QLabel(m_root);
    nameLabel->setText(panel->displayName());
    nameLabel->setContentsMargins(0, ABOVE_HEADING_MARGIN, 0, 0);
    QFont f = nameLabel->font();
    f.setBold(true);
    f.setPointSizeF(f.pointSizeF() * 1.4);
    nameLabel->setFont(f);
    m_layout->addWidget(nameLabel, headerRow, 1, 1, 1, Qt::AlignBottom | Qt::AlignLeft);

    // line:
    const int lineRow(headerRow + 1);
    m_layout->addWidget(new OnePixelBlackLine(m_root), lineRow, 1);

    // add the widget:
    const int widgetRow(lineRow + 1);
    addPanelWidget(panel, widgetRow);
}

void PanelsWidget::addPanelWidget(IPropertiesPanel *panel, int row)
{
    QWidget *widget = panel->widget();
    widget->setContentsMargins(m_layout->columnMinimumWidth(0),
                                           ABOVE_CONTENTS_MARGIN, 0,
                                           BELOW_CONTENTS_MARGIN);
    widget->setParent(m_root);
    m_layout->addWidget(widget, row, 0, 1, 2);

    const int stretchRow(row + 1);
    m_layout->setRowStretch(stretchRow, 10);

    m_panels.append(panel);
}

///
// ProjectWindow
///

ProjectWindow::ProjectWindow(QWidget *parent)
    : QWidget(parent),
    m_panelsWidget(0)
{
    ProjectExplorer::SessionManager *session = ProjectExplorerPlugin::instance()->session();

    // Setup overall layout:
    QVBoxLayout *viewLayout = new QVBoxLayout(this);
    viewLayout->setMargin(0);
    viewLayout->setSpacing(0);

    m_tabWidget = new DoubleTabWidget(this);
    m_tabWidget->setTitle(tr("Select a Project:"));
    viewLayout->addWidget(m_tabWidget);

    // Setup our container for the contents:
    m_centralWidget = new QStackedWidget(this);
    viewLayout->addWidget(m_centralWidget);

    // no projects label:
    m_noprojectLabel = new QWidget;
    QVBoxLayout *noprojectLayout = new QVBoxLayout;
    noprojectLayout->setMargin(0);
    QLabel *label = new QLabel(m_noprojectLabel);
    label->setText(tr("No project loaded."));
    {
        QFont f = label->font();
        f.setPointSizeF(f.pointSizeF() * 1.4);
        f.setBold(true);
        label->setFont(f);
    }
    label->setMargin(10);
    label->setAlignment(Qt::AlignTop);
    noprojectLayout->addWidget(label);
    noprojectLayout->addStretch(10);
    m_centralWidget->addWidget(m_noprojectLabel);

    // connects:
    connect(m_tabWidget, SIGNAL(currentIndexChanged(int,int)),
            this, SLOT(showProperties(int,int)));

    connect(session, SIGNAL(sessionLoaded()), this, SLOT(restoreStatus()));
    connect(session, SIGNAL(aboutToSaveSession()), this, SLOT(saveStatus()));

    connect(session, SIGNAL(projectAdded(ProjectExplorer::Project*)),
            this, SLOT(projectAdded(ProjectExplorer::Project*)));
    connect(session, SIGNAL(aboutToRemoveProject(ProjectExplorer::Project*)),
            this, SLOT(aboutToRemoveProject(ProjectExplorer::Project*)));

    // Update properties to empty project for now:
    showProperties(-1, -1);
}

ProjectWindow::~ProjectWindow()
{
}

void ProjectWindow::shutdown()
{
    showProperties(-1, -1); // TODO that's a bit stupid, but otherwise stuff is still
                             // connected to the session
    disconnect(ProjectExplorerPlugin::instance()->session(), 0, this, 0);
}

void ProjectWindow::projectAdded(ProjectExplorer::Project *project)
{
    QList<Project *> projects = ProjectExplorerPlugin::instance()->session()->projects();
    int index = projects.indexOf(project);
    QStringList subtabs;
    foreach (IPanelFactory *panelFactory, ExtensionSystem::PluginManager::instance()->getObjects<IPanelFactory>()) {
        if (panelFactory->supports(project))
            subtabs << panelFactory->displayName();
    }
    m_tabIndexToProject.insert(index, project);
    m_tabWidget->insertTab(index, project->displayName(), subtabs);
}

void ProjectWindow::aboutToRemoveProject(ProjectExplorer::Project *project)
{
    int index = m_tabIndexToProject.indexOf(project);
    m_tabIndexToProject.removeAt(index);
    m_tabWidget->removeTab(index);
}

void ProjectWindow::restoreStatus()
{
    // TODO
}

void ProjectWindow::saveStatus()
{
    // TODO
}

void ProjectWindow::showProperties(int index, int subIndex)
{
    if (index < 0) {
        m_centralWidget->setCurrentWidget(m_noprojectLabel);
        if (m_panelsWidget) {
            m_centralWidget->removeWidget(m_panelsWidget);
            delete m_panelsWidget;
            m_panelsWidget = 0;
        }
        return;
    }
    Project *project = m_tabIndexToProject.at(index);


    PanelsWidget *panelsWidget = 0;
    // Set up custom panels again:
    QList<IPanelFactory *> pages =
            ExtensionSystem::PluginManager::instance()->getObjects<IPanelFactory>();
    int indexOfPanel = 0;
    foreach (IPanelFactory *panelFactory, pages) {
        if (panelFactory->supports(project)) {
            if (indexOfPanel == subIndex) {
                panelsWidget = new PanelsWidget(m_centralWidget);
                IPropertiesPanel *panel = panelFactory->createPanel(project);
                panelsWidget->addPropertiesPanel(panel);
                panel->widgetWasAddedToLayout();
                break;
            }
            ++indexOfPanel;
        }
    }
    if (panelsWidget) {
        // add the new stuff to the stack widget
        m_centralWidget->addWidget(panelsWidget);
        m_centralWidget->setCurrentWidget(panelsWidget);
        if (m_panelsWidget) {
            m_centralWidget->removeWidget(m_panelsWidget);
            delete m_panelsWidget;
        }
        m_panelsWidget = panelsWidget;
    }
}
