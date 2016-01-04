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

#ifndef MDICHILD_H
#define MDICHILD_H

#include <QWidget>
#include <QScrollBar>
#include <QToolBar>
#include <QMenu>



#include "images.h"
#include "n2draw.h"

#include "mdiselector.h"

#include "n2drawmanager.h"
#include "n2miniXml.h"
#include "n2imagemanager.h"
#include "n2viewglue.h"
#include "n2appmanager.h"
#include "n2connection.h"



class mdiScrollBar;


class MdiChild : public QWidget
{
    Q_OBJECT
private:
    mdiScrollBar  *vScroll;
    mdiScrollBar  *hScroll;
    childApps   *n2client;
    mdiSelector *selector;
    QString curFile;
    bool isUntitled;
    QPixmap pixmap;
public:
    MdiChild();
    ~MdiChild();
    void newFile();
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }
    inline void setClient (childApps *c){n2client=c;}
    void updateDocPosHorz(void);
    void updateDocPosVert(void);
    void paste(void);
    void cut(void);
    void copy(void);
protected:
    static void updateViewEventRequest(void *dest,size_t param);
    static void externCommandRequest(void * dest, size_t param);
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private slots:
    void documentWasModified(bool v);
    void resizeEvent(QResizeEvent *) override;
private:
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void paintEvent(QPaintEvent * /* event */) override;
    void refreshPixmap(size_t param);
    void requestCommand(size_t param);
    void destroyObjects(void);
    void mouseMoveEvent( QMouseEvent *event ) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void resizeSelector(void);
    void errorSelector(void);
    void keyPressEvent(QKeyEvent *event) override;
    void adjustScrollBars(const nnPoint &pos);
};


class mdiScrollBar: public QScrollBar
{
    Q_OBJECT
public:
    mdiScrollBar(Qt::Orientation orientation,
        MdiChild *parent=0):
        QScrollBar(orientation,parent){
        setFocusPolicy(Qt::NoFocus);
    }

    void sliderChange(SliderChange change)
    {
        QScrollBar::sliderChange(change);
        refreshDoc();
    }

    void wheelEvent(QWheelEvent *event)
    {
        QScrollBar::wheelEvent(event);
        refreshDoc();
    }
private :
    void refreshDoc(void)
    {
        MdiChild *v=static_cast<MdiChild *>(parent());
        if(v)
        {
            if(orientation()==Qt::Orientation::Horizontal)
                v->updateDocPosHorz();
            else
                if(orientation()==Qt::Orientation::Vertical)
                    v->updateDocPosVert();
        }
    }
};




#endif
