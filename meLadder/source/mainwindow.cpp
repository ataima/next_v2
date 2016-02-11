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

#include <QtWidgets>

#include "mainwindow.h"
#include "n2exception.h"

MainWindow::MainWindow()
{
    init();
    setCurrentFile("");
}

MainWindow::MainWindow(const QString &fileName)
{
    init();
    loadFile(fileName);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    MainWindow *other = new MainWindow;
    other->move(x() + 40, y() + 40);
    other->show();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        MainWindow *existing = findMainWindow(fileName);
        if (existing) {
            existing->show();
            existing->raise();
            existing->activateWindow();
            return;
        }

        if (!isWindowModified()) {
            loadFile(fileName);
        } else {
            MainWindow *other = new MainWindow(fileName);
            if (other->isUntitled) {
                delete other;
                return;
            }
            other->move(x() + 40, y() + 40);
            other->show();
        }
    }
}

bool MainWindow::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                       curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About SDI"),
                       tr("The <b>SDI</b> example demonstrates how to write single "
                          "document interface applications using Qt."));
}

void MainWindow::documentWasModified()
{
    setWindowModified(true);
}



#if _LOGGER_
class testLogger
    :public IPrinter
{
public :
    void out(std::string & msg)
    {
     qDebug()<<msg.c_str();
    }
};
#endif

void MainWindow::init()
{
    setAttribute(Qt::WA_DeleteOnClose);

    isUntitled = true;

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    n2App=IAppManager::getInstance(0);
    if(n2App)
    {
        IChild *client=nullptr;
        try {
            QString   path=qApp->applicationDirPath();
#ifdef __APPLE__
                    path+=X("/../../..");
#endif
            path+=X("/");
            STRING file;
#ifdef _UNICODE
            file=X("conf_utf16.xml");
#else
            file="conf_utf8.xml";
#endif


            STRING conf=path.FROMQSTRING();
            client=n2App->createObjects(file,conf);
        }
        catch(n2exception *e)
        {
            if(e!=nullptr)
            {
                const char *msg=e->msg();
                QMessageBox m(QMessageBox::Icon::Critical,
                              "ERROR",
                              msg,
                              QMessageBox::StandardButton::Ok);
                m.exec();
                if(msg!=nullptr)
                    delete msg;
                delete e;
            }
        }
        catch(...)
        {
            QMessageBox m(QMessageBox::Icon::Critical,
                          "ERROR",
                          "Unhandled exception",
                          QMessageBox::StandardButton::Ok);
            m.exec();
        }
        if(client!=nullptr)
        {
            client->setExtHandler( &MainWindow::externCommandRequest,
                                   this
                                 );
#if _LOGGER_
                {
                    testLogger *tL = new testLogger();
                    n2App->setPrinter(tL);
                }
#endif
            QDesktopWidget dw;
            int x=dw.width()*0.6;
            int y=dw.height()*0.6;
            resize(x,y);
        }
        else
        {
            close();
        }

    }
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::paintEvent(QPaintEvent *  /*event*/ )
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
}


void MainWindow::externCommandRequest(void * dest, size_t type_param,IParam *user_param)
{
    if(dest)
    {
        MainWindow *child = static_cast< MainWindow *>(dest);
        child->requestCommand(type_param,user_param);
    }
}




void MainWindow::directCommand(IParam * user_param)
{
    nnAbstractParam<int> *t=static_cast<nnAbstractParam<int>*>(user_param);
    if(t)
    {
        qDebug()<<"RX user param :"<<t->value();
        // from conf...xml toolbars
        switch(t->value())
        {
        case 4000:
            save();
            break;
        case 4001:
            open();
            break;
        case 4002:
            newFile();
            break;
        case 4003:
            //copy();
            break;
        case 4004:
            //cut();
            break;
        case 4005:
            //paste();
            break;
        }
    }
}

void MainWindow::requestCommand(size_t type_param,IParam * user_param)
{
    switch(type_param)
    {
    case action_align_windows:
    {
        nnAbstractParam<nnPoint> *t=static_cast<nnAbstractParam<nnPoint>*>(user_param);
        if(t)
        {
            int x = t->value().x;
            int y = t->value().y;
            if(isMaximized()&& ( x>size().width() || y>size().height()))
            {
                IChild * child=n2App->active();
                nnPoint c=child->getView()->getConstPhy();
                if(x>size().width())
                {
                    x-=c.x;
                }
                if(y>size().height())
                {
                    y-=c.y;
                }

            }
            QSize s(x,y);
            resize(s);
        }
    }
    break;
    case action_update_statusbars_panes:
    case action_update_statusbars_info:
    case action_redraw:
        refreshPixmap();
        break;
    case action_host_command:
        directCommand(user_param);
        break;
    case action_close_windows:
        delete n2App;
        n2App = nullptr;
        close();
        break;
    case action_move_window:
        {
            nnAbstractParam<nnPoint> *t = static_cast<nnAbstractParam<nnPoint>*>(user_param);
            if (t)
            {
                int x = t->value().x+pos().x();
                int y = t->value().y+pos().y();
                move(x,y);
            }
        }
    break;
    case action_maximize_windows:
        showMaximized();
        break;
    case action_iconize_windows:
        showMinimized();
        break;
    case action_medialize_windows:
    {
        QDesktopWidget dw;
        int x=dw.width()*0.6;
        int y=dw.height()*0.6;
        resize(x,y);
    }
        break;
    }
}



void MainWindow::refreshPixmap(void)
{
    pixmap = QPixmap(size());
    IChild * n2Client=n2App->active();
    if(n2Client!=nullptr)
    {
        bmpImage &bdraw = n2Client->getView()->getDraw();
        pixmap.loadFromData((unsigned char *)(LPBITMAPFILEHEADER)bdraw,
                            bdraw.getTotalSize(),
                            "BMP");
    }
    update();
}



void MainWindow::resizeEvent(QResizeEvent *e)
{
    IChild * n2Client=n2App->active();
    QWidget::resizeEvent(e);
    if(n2Client!=nullptr)
    {
        QSize s=size();
        n2Client->getView()->resize(s.width(),s.height());
    }
}







void MainWindow::destroyObjects(void)
{
}



void MainWindow::mouseMoveEvent( QMouseEvent *event )
{
    IHandler *handler = n2App->active();
    if (handler)
    {
        QPoint p=event->globalPos();
        p=mapFromGlobal(p);
        nnPoint pos(p.x(),p.y());
        if(event->buttons()==Qt::LeftButton)
        {
            handler->handlerMouseMove(nn_m_button_left,pos);
        }
        else if(event->buttons()==Qt::NoButton)
        {
            handler->handlerMouseMove(nn_m_button_unknow,pos);
        }
    }
}



void MainWindow::mousePressEvent(QMouseEvent *event)
{
    const char * error=nullptr;
    IHandler *handler = n2App->active();
    if (handler)
    {
        QPoint p=event->globalPos();
        p=mapFromGlobal(p);
        nn_mouse_buttons bt=(nn_mouse_buttons)(unsigned int)event->buttons();
        nnPoint pos(p.x(),p.y());
        try {
            handler->handlerMouseButtonDown(bt,pos);
        }
        catch(n2exception *e)
        {
            error=e->msg();
            delete e->msg();
            delete e;
        }
        catch(...)
        {
            error="unknow exception";
        }
        if(error!=nullptr)
        {
            QMessageBox::warning(this,"meLadder",
                                 tr("At draw toolbars\n%1.")
                                 .arg(error));
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    nnPoint start, stop;
    IHandler *handler = n2App->active();
    if (handler)
    {
        QPoint p=event->globalPos();
        p=mapFromGlobal(p);
        unsigned int bt=event->buttons();
        nnPoint pos(p.x(),p.y());
        handler->handlerMouseButtonUp((nn_mouse_buttons)bt,pos);
    }
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    bool res=false;
    IHandler *handler = n2App->active();
    if (handler)
    {
//        qDebug()<<"modifiers()="<<event->modifiers()<<"   Key()="<<event->key();
        Qt::KeyboardModifiers mod=event->modifiers();
        Qt::Key keyb=(Qt::Key)event->key();
        bool alt=false,ctrl=false,shift=false;
        if(mod&Qt::ShiftModifier)
            shift=true;
        if(mod&Qt::ControlModifier)
            ctrl=true;
        if(mod&Qt::AltModifier)
            alt=true;
        switch(keyb)
        {
        case Qt::Key_Escape:
            res=handler->handlerEscapeButton(shift,ctrl,alt);
            break;
        case Qt::Key_Home:
            res=handler->handlerHomeButton(shift,ctrl,alt);
            break;
        case Qt::Key_End:
            res=handler->handlerEndButton(shift,ctrl,alt);
            break;
        case Qt::Key_PageUp:
            res=handler->handlerPageUpButton(shift,ctrl,alt);
            break;
        case Qt::Key_PageDown:
            res=handler->handlerPageDownButton(shift,ctrl,alt);
            break;
        case Qt::Key_Left:
            res=handler->handlerLeftButton(shift,ctrl,alt);
            break;
        case Qt::Key_Up:
            res=handler->handlerUpButton(shift,ctrl,alt);
            break;
        case Qt::Key_Right:
            res=handler->handlerRightButton(shift,ctrl,alt);
            break;
        case Qt::Key_Down:
            res=handler->handlerDownButton(shift,ctrl,alt);
            break;
        }
    }
}





bool MainWindow::maybeSave()
{
    if (1) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("SDI"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SDI"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

//    QTextStream in(&file);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SDI"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

//   QTextStream out(&file);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    static int sequenceNumber = 1;

    isUntitled = fileName.isEmpty();
    if (isUntitled) {
        curFile = tr("document%1.txt").arg(sequenceNumber++);
    } else {
        curFile = QFileInfo(fileName).canonicalFilePath();
    }
    setWindowModified(false);
    setWindowFilePath(curFile);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

MainWindow *MainWindow::findMainWindow(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QWidget *widget, qApp->topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin && mainWin->curFile == canonicalFilePath)
            return mainWin;
    }
    return 0;
}

