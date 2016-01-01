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

MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    n2client=nullptr;
    hScroll = new mdiScrollBar(Qt::Orientation::Horizontal,this);
    vScroll = new mdiScrollBar(Qt::Orientation::Vertical,this);
    hScroll->hide();
    vScroll->hide();
    selector = new mdiSelector();
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
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
    if(n2client!=nullptr)
    {
        int w=0,h=0;
        if(n2client->view->needScrollBarHorz())
        {
            w=n2client->view->getScrollableHorzSize();
            if(w)
            {
                hScroll->setMinimum(0);
                hScroll->setMaximum(w);
                hScroll->show();
            }
            else
            {
                hScroll->hide();
            }
        }
        if(n2client->view->needScrollBarHorz())
        {
            h=n2client->view->getScrollableVertSize();
            if(h)
            {
                vScroll->setMinimum(0);
                vScroll->setMaximum((int)h);
                vScroll->show();
            }
            else
            {
                vScroll->hide();
            }
        }
        QSize m=maximumSize();
        m/=3;
        setMinimumSize(m);
        n2client->view->unselect();
    }
}

bool MdiChild::loadFile(const QString &fileName)
{
    bool res=false;
    if(fileName.size()>0)
    {
        QString error="NO ERROR";
        if(n2client)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            try {
                res=n2client->object_manager->load(fileName.FROMQSTRING());
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
                QMessageBox::warning(this, tr("Draw"),
                                     tr("Cannot read file %1:\n%2.")
                                     .arg(fileName)
                                     .arg(error));

            }
            QApplication::restoreOverrideCursor();
            setCurrentFile(fileName);
            refreshPixmap();
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
        if(n2client)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            try {
                res=n2client->object_manager->save(fileName.FROMQSTRING());
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
                QMessageBox::warning(this, tr("Draw"),
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
        ret = QMessageBox::warning(this, tr("Draw"),
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
    if(selector)
        selector->draw(painter);
}


void MdiChild::refreshPixmap()
{
    pixmap = QPixmap(size());
    if(n2client!=nullptr)
    {
        bmpImage &bdraw = n2client->view->getDraw();
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
    if(n2client!=nullptr)
    {
        QSize s=size();
        n2client->view->resize(s.width(),s.height());
        if(hScroll)
        {
            size_t w=n2client->view->getScrollableHorzSize();
            if(w)
            {
                hScroll->setMaximum((int)w);
                hScroll->move(s.width()/3,s.height()-DEF_SCR_XY);
                hScroll->resize(s.width()/3,DEF_SCR_XY);
                hScroll->show();
            }
            else
            {
                hScroll->hide();
            }
        }
        if(vScroll)
        {
            size_t  h=n2client->view->getScrollableVertSize();
            if(h)
            {
                vScroll->setMaximum((int)h);
                if (getMainWnd()->layoutDirection() == Qt::LeftToRight)
                {
                    vScroll->move(0,s.height()/3);
                }
                else
                {
                    vScroll->move(s.width()-DEF_SCR_XY,s.height()/3);
                }
                vScroll->resize(DEF_SCR_XY,s.height()/3);
                vScroll->show();
            }
            else
            {
                vScroll->hide();
            }
        }
        refreshPixmap();
    }
}

void MdiChild::updateDocPosHorz(void)
{
    int pos_x=0;
    if(hScroll)
    {
        pos_x=hScroll->value();
        qDebug()<<"SCROLL_X :"<<pos_x;
    }
    if(n2client)
    {
        n2client->view->handlerScrollHorz(pos_x);
        resizeSelector();
    }
}

void MdiChild::updateDocPosVert(void)
{
    int pos_y=0;
    if(vScroll)
    {
        pos_y=vScroll->value();
        qDebug()<<"SCROLL_Y :"<<pos_y;
    }
    if(n2client)
    {
        n2client->view->handlerScrollVert(pos_y);
        resizeSelector();
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
    DESTROY_OBJ(hScroll);
    DESTROY_OBJ(vScroll);
    DESTROY_OBJ(selector);
}


void MdiChild::resizeSelector(void)
{
    if(n2client && selector)
    {
        nnRect result;
        nnPoint start,stop;
        if(n2client->view->isSelectAreaPhyVisible(result,start,stop))
        {
            selector->setError(false);
            selector->SetArea(result,start,stop);
            selector->show();
            refreshPixmap();
        }
        else
        {
            selector->hide();
            refreshPixmap();
        }
    }
}

void MdiChild::errorSelector(void)
{
    if(n2client && selector && selector->getStatus())
    {
        selector->setError(true);
        refreshPixmap();
    }
}


void MdiChild::mouseMoveEvent( QMouseEvent *event )
{
    nnPoint start, stop;
    if(n2client)
    {
        QPoint p=event->globalPos();
        p=mapFromGlobal(p);
        if(event->buttons()==Qt::LeftButton)
        {
            unsigned int bt=event->buttons();
            nnPoint pos(p.x(),p.y());
            if(n2client->view->handlerMouseMove((nn_mouse_buttons)bt,pos,start,stop))
            {
                qDebug()<<"START:"<<start.x<<" - "<<start.y;
                qDebug()<<"STOP:"<<stop.x<<" - "<<stop.y;
                getMainWnd()->updatePosCursor(start,stop);
                resizeSelector();
            }
        }
        else if(event->buttons()==Qt::NoButton)
        {
            if(selector && selector->getStatus()==false)
            {
                nnPoint log,pos(p.x(),p.y());
                log=n2client->view->getCoordLog(pos);
                getMainWnd()->updatePosCursor(log,log);
            }
        }
    }
}



void MdiChild::mousePressEvent(QMouseEvent *event)
{
    nnPoint start, stop;
    if(n2client)
    {
        QPoint p=event->globalPos();
        p=mapFromGlobal(p);
        unsigned int bt=event->buttons();
        nnPoint pos(p.x(),p.y());
        if(n2client->view->handlerMouseButtonDown((nn_mouse_buttons)bt,pos,start,stop))
        {
            qDebug()<<"START:"<<start.x<<" - "<<start.y;
            qDebug()<<"STOP:"<<stop.x<<" - "<<stop.y;
            getMainWnd()->updatePosCursor(start,stop);
            resizeSelector();
        }
    }
}

void MdiChild::mouseReleaseEvent(QMouseEvent *event)
{
    nnPoint start, stop;
    if(n2client)
    {
        QPoint p=event->globalPos();
        p=mapFromGlobal(p);
        unsigned int bt=event->buttons();
        nnPoint pos(p.x(),p.y());
        if(n2client->view->handlerMouseButtonUp((nn_mouse_buttons)bt,pos,start,stop))
        {
            qDebug()<<"START:"<<start.x<<" - "<<start.y;
            qDebug()<<"STOP:"<<stop.x<<" - "<<stop.y;
            getMainWnd()->updatePosCursor(start,stop);
            resizeSelector();
        }
    }

}

void MdiChild::adjustScrollBars(const nnPoint & pos)
{
    if(hScroll)
        hScroll->setValue((int)pos.x);
    if(vScroll)
        vScroll->setValue((int)pos.y);
}


void MdiChild::keyPressEvent(QKeyEvent *event)
{
    bool res=false;
    if( n2client )
    {
        nnPoint start,stop,pos;
        qDebug()<<"modifiers()="<<event->modifiers()<<"   Key()="<<event->key();
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
            res=n2client->view->handlerEscapeButton(shift,ctrl,alt,start,stop);
            if(res)
            {
                selector->hide();
                getMainWnd()->updatePosCursor(start,stop);
            }
            break;
        case Qt::Key_Home:
            res=n2client->view->handlerHomeButton(shift,ctrl,alt,pos);
            if(res)
            {
                adjustScrollBars(pos);
            }
            break;
        case Qt::Key_End:
            res=n2client->view->handlerEndButton(shift,ctrl,alt,pos);
            if(res)
            {
                adjustScrollBars(pos);
            }
            break;
        case Qt::Key_PageUp:
            res=n2client->view->handlerPageUpButton(shift,ctrl,alt,pos);
            if(res)
            {
                adjustScrollBars(pos);
            }
            break;
        case Qt::Key_PageDown:
            res=n2client->view->handlerPageDownButton(shift,ctrl,alt,pos);
            if(res)
            {
                adjustScrollBars(pos);
            }
            break;
        }
        if(res)
        {
            refreshPixmap();
        }
        else
        {
            if(n2client->view->isStartValid())
            {
                bool needScroll;
                switch(keyb)
                {
                case Qt::Key_Left:
                    res=n2client->view->handlerLeftButton(shift,ctrl,alt,start,stop,needScroll);
                    break;
                case Qt::Key_Up:
                    res=n2client->view->handlerUpButton(shift,ctrl,alt,start,stop,needScroll);
                    break;
                case Qt::Key_Right:
                    res=n2client->view->handlerRightButton(shift,ctrl,alt,start,stop,needScroll);
                    break;
                case Qt::Key_Down:
                    res=n2client->view->handlerDownButton(shift,ctrl,alt,start,stop,needScroll);
                    break;
                }
                if(res)
                {
                    if(needScroll)
                    {
                        adjustScrollBars( n2client->view->getOffsetView());
                    }
                    getMainWnd()->updatePosCursor(start,stop);
                    resizeSelector();
                }
                else
                {
                    //at border red
                    errorSelector();
                }
            }
        }
    }
}

