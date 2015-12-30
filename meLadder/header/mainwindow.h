/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MdiChild;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QLabel;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
QT_END_NAMESPACE


#include "images.h"
#include "n2draw.h"
#include "n2drawmanager.h"
#include "n2miniXml.h"
#include "n2imagemanager.h"
#include "n2viewglue.h"
#include "n2appmanager.h"
#include "n2connection.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
private :
    IAppManager *n2app;
    static MainWindow *instance;
public:
    MainWindow();
    ~MainWindow();
    bool openFile(const QString &fileName);
    static MainWindow * getInstance(void){return instance;}
    static IAppManager * getn2AppManager(void){return instance->n2app;}
    void updatePosCursor(nnPoint & start,nnPoint & stop);
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void newFile();
    void open();
    void save();
    void saveAs();
    void about();
    void cut();
    void copy();
    void paste();
    void updateMenus();
    void updateWindowMenu();
    MdiChild *createMdiChild();
    void switchLayoutDirection();
    void setActiveSubWindow(QWidget *window);

private:
    //void keyPressEvent(QKeyEvent *event);
    void destroyObjects();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    MdiChild *activeMdiChild();
    QMdiSubWindow *findMdiChild(const QString &fileName);

    QMdiArea *mdiArea;
    QSignalMapper *windowMapper;

    QMenu *fileMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    QMenu *editMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QLabel *m_statusLeft;
    QLabel *m_statusRight;
    QLabel *m_statusMiddleLeft;
    QLabel *m_statusMiddleRight;
};


#define getMainWnd()    MainWindow::getInstance()
#define getn2App()      MainWindow::getn2AppManager()


#define DESTROY_OBJ(obj)  if(obj!=nullptr){delete obj;obj=nullptr;}

#endif
