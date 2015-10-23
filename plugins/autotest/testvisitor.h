/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd
** All rights reserved.
** For any questions to The Qt Company, please use contact form at
** http://www.qt.io/contact-us
**
** This file is part of the Qt Creator Enterprise Auto Test Add-on.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.
**
** If you have questions regarding the use of this file, please use
** contact form at http://www.qt.io/contact-us
**
****************************************************************************/

#ifndef TESTVISITOR_H
#define TESTVISITOR_H

#include "testtreeitem.h"

#include <cplusplus/ASTVisitor.h>
#include <cplusplus/CppDocument.h>
#include <cplusplus/Overview.h>
#include <cplusplus/Scope.h>
#include <cplusplus/SymbolVisitor.h>

#include <cpptools/symbolfinder.h>

#include <qmljs/parser/qmljsastvisitor_p.h>
#include <qmljs/qmljsdocument.h>

#include <QMap>
#include <QString>

namespace Autotest {
namespace Internal {

class TestVisitor : public CPlusPlus::SymbolVisitor
{
public:
    TestVisitor(const QString &fullQualifiedClassName);
    virtual ~TestVisitor();

    QMap<QString, TestCodeLocationAndType> privateSlots() const { return m_privSlots; }

    bool visit(CPlusPlus::Class *symbol);

private:
    CppTools::SymbolFinder m_symbolFinder;
    QString m_className;
    QMap<QString, TestCodeLocationAndType> m_privSlots;
};

class TestAstVisitor : public CPlusPlus::ASTVisitor
{
public:
    TestAstVisitor(CPlusPlus::Document::Ptr doc);
    virtual ~TestAstVisitor();

    bool visit(CPlusPlus::CallAST *ast);
    bool visit(CPlusPlus::CompoundStatementAST *ast);

    QString className() const { return m_className; }

private:
    QString m_className;
    CPlusPlus::Scope *m_currentScope;
    CPlusPlus::Document::Ptr m_currentDoc;

};

class TestDataFunctionVisitor : public CPlusPlus::ASTVisitor
{
public:
    TestDataFunctionVisitor(CPlusPlus::Document::Ptr doc);
    virtual ~TestDataFunctionVisitor();

    bool visit(CPlusPlus::UsingDirectiveAST *ast);
    bool visit(CPlusPlus::FunctionDefinitionAST *ast);
    bool visit(CPlusPlus::CallAST *ast);
    bool preVisit(CPlusPlus::AST *ast);
    void postVisit(CPlusPlus::AST *ast);
    QMap<QString, TestCodeLocationList> dataTags() const { return m_dataTags; }

private:
    bool newRowCallFound(CPlusPlus::CallAST *ast, unsigned *firstToken) const;

    CPlusPlus::Document::Ptr m_currentDoc;
    CPlusPlus::Overview m_overview;
    QString m_currentFunction;
    QMap<QString, TestCodeLocationList> m_dataTags;
    TestCodeLocationList m_currentTags;
    unsigned m_currentAstDepth;
    unsigned m_insideUsingQTestDepth;
    bool m_insideUsingQTest;
};

class TestQmlVisitor : public QmlJS::AST::Visitor
{
public:
    TestQmlVisitor(QmlJS::Document::Ptr doc);
    virtual ~TestQmlVisitor();

    bool visit(QmlJS::AST::UiObjectDefinition *ast);
    bool visit(QmlJS::AST::ExpressionStatement *ast);
    bool visit(QmlJS::AST::UiScriptBinding *ast);
    bool visit(QmlJS::AST::FunctionDeclaration *ast);
    bool visit(QmlJS::AST::StringLiteral *ast);

    QString testCaseName() const { return m_currentTestCaseName; }
    TestCodeLocationAndType testCaseLocation() const { return m_testCaseLocation; }
    QMap<QString, TestCodeLocationAndType> testFunctions() const { return m_testFunctions; }

private:
    QmlJS::Document::Ptr m_currentDoc;
    QString m_currentTestCaseName;
    TestCodeLocationAndType m_testCaseLocation;
    QMap<QString, TestCodeLocationAndType> m_testFunctions;

};

} // namespace Internal
} // namespace Autotest

#endif // TESTVISITOR_H