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

#include "mdichild.h"
#include "mainwindow.h"
#include "n2exception.h"

MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    n2Client=nullptr;
}


MdiChild::~MdiChild()
{
    destroyObjects();
}





void MdiChild::newFile()
{
    /* Open Maximized */
    static int sequenceNumber = 1;
    isUntitled = true;
    curFile = tr("untitled%1.edb").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");
    documentWasModified(false);
    if(n2Client)
    {
        n2Client->setExtHandler( handler_exec_command,
                                 &MdiChild::externCommandRequest,
                                 this
                                 );
    }
    QSize m=maximumSize();
    m/=3;
    setMinimumSize(m);
}

bool MdiChild::loadFile(const QString &fileName)
{
    bool res=false;
    if(fileName.size()>0)
    {
        QString error="NO ERROR";
        if(n2Client)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            try {
                res=n2Client->getManager()->load(fileName.FROMQSTRING());
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
            if(!res)
            {
                QMessageBox::warning(this, "meLadder",
                                     tr("Cannot read file %1:\n%2.")
                                     .arg(fileName)
                                     .arg(error));

            }
            QApplication::restoreOverrideCursor();
            setCurrentFile(fileName);
        }
    }
    return res;
}

bool MdiChild::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                       curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MdiChild::saveFile(const QString &fileName)
{

    bool res=false;
    if(fileName.size()>0)
    {
        QString error="NO ERROR";
        if(n2Client)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            try {
                res=n2Client->getManager()->save(fileName.FROMQSTRING());
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
            if(!res)
            {
                QMessageBox::warning(this, "meLadder",
                                     tr("Cannot write file %1:\n%2.")
                                     .arg(fileName)
                                     .arg(error));

            }
            QApplication::restoreOverrideCursor();
        }
        setCurrentFile(fileName);
    }
    return res;
}

QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MdiChild::documentWasModified(bool v)
{
    setWindowModified(v);
}


bool MdiChild::maybeSave()
{
    /*if (document()->isModified())*/ {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, "meLadder",
                                   tr("'%1' has been modified.\n"
                                      "Do you want to save your changes?")
                                   .arg(userFriendlyCurrentFile()),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


/////////////////////////////////////////////////////////////////////

void MdiChild::paintEvent(QPaintEvent * /* event */)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
}




void MdiChild::externCommandRequest(void * dest, handlerAction type_param,size_t user_param)
{
    if(dest)
    {
        MdiChild *child = static_cast< MdiChild *>(dest);
        child->requestCommand(type_param,user_param);
    }
}


void MdiChild::directCommand(size_t user_param)
{
    qDebug()<<"RX user param :"<<user_param;
    // from conf...xml toolbars
    switch(user_param)
    {
    case 4000:
        getMainWnd()->save();
        break;
    case 4001:
        getMainWnd()->open();
        break;
    case 4002:
        getMainWnd()->newFile();
        break;
    case 4003:
        copy();
        break;
    case 4004:
        cut();
        break;
    case 4005:
        paste();
        break;
    }
}

void MdiChild::requestCommand(handlerAction type_param,size_t user_param)
{
    switch(type_param)
    {
    case action_align_windows:
                   {
                       int x = (user_param & 0xffff0000) >> 16;
                       int y = (user_param & 0xffff);
                       QSize s(x,y);
                       resize(s);
                   }
        break;
    case action_redraw:
        refreshPixmap();
        break;
    case action_host_command:
        directCommand(user_param);
        break;
    case action_update_statusbars_info:
    {
         char *p=(char *)user_param;
         getMainWnd()->statusBar()->showMessage(p);
    }
    break;
    case action_update_statusbars_panes:
    {
        if(n2Client && n2Client->getView())
        {
            nnPoint start,stop;
            n2Client->getView()->getSelectArea(start,stop);
            getMainWnd()->updatePosCursor(start,stop);
        }
    }
    break;
    }
}


void MdiChild::refreshPixmap(void)
{
    pixmap = QPixmap(size());
    if(n2Client!=nullptr)
    {
        bmpImage &bdraw = n2Client->getView()->getDraw();
        pixmap.loadFromData((unsigned char *)(LPBITMAPFILEHEADER)bdraw,
                            bdraw.getTotalSize(),
                            "BMP");
    }
    update();
}


//////////////////////////////////////////////////////////////////

#define DEF_SCR_XY      18

void MdiChild::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    if(n2Client!=nullptr)
    {
        QSize s=e->size();
        n2Client->getView()->resize(s.width(),s.height());
    }
}





void MdiChild::paste(void)
{

}

void MdiChild::cut(void)
{

}

void MdiChild::copy(void)
{

}

void MdiChild::destroyObjects(void)
{
}



void MdiChild::mouseMoveEvent( QMouseEvent *event )
{
    IHandler *handler = getn2App()->active();
    if (handler)
    {
        QPoint p=event->globalPos();
        p=mapFromGlobal(p);
        nnPoint pos(p.x(),p.y());
        if(event->buttons()==Qt::LeftButton)
        {
            handler->handlerMouseMove(nn_m_button_left,pos);
        }
        else
            if(event->buttons()==Qt::NoButton)
            {
                handler->handlerMouseMove(nn_m_button_unknow,pos);
            }
    }
}



void MdiChild::mousePressEvent(QMouseEvent *event)
{
    const char * error=nullptr;
    IHandler *handler = getn2App()->active();
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

void MdiChild::mouseReleaseEvent(QMouseEvent *event)
{
    nnPoint start, stop;
    IHandler *handler = getn2App()->active();
    if (handler)
    {
        QPoint p=event->globalPos();
        p=mapFromGlobal(p);
        unsigned int bt=event->buttons();
        nnPoint pos(p.x(),p.y());
        handler->handlerMouseButtonUp((nn_mouse_buttons)bt,pos);
    }
}



void MdiChild::keyPressEvent(QKeyEvent *event)
{
    bool res=false;
    IHandler *handler = getn2App()->active();
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

