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

#include "n2draw.h"
#include "mdiselector.h"


mdiSelector::mdiSelector(void)
{
    pos.set(-1,-1,0,0);
    hide();
}

void  mdiSelector::SetArea(nnRect &rect, nnPoint &_start, nnPoint &_stop)
{
    pos=rect;
    start=_start;
    stop=_stop;
    qDebug()<<"START("<<start.x<<","<<start.y<<")";
    qDebug()<<"STOP("<<stop.x<<","<<stop.y<<")";
}


void mdiSelector::draw(QPainter &p)
{
    if(visible)
    {
        QColor old=p.pen().color();
        QColor black(Qt::black);        
        QRect v,r;
        QFont oldfont,nfont("Courier", 8, QFont::Bold, true);
        QPen gray(Qt::gray, 2, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
        r.setRect(pos.start.x,pos.start.y,pos.width(),pos.height());
        oldfont=p.font();
        p.setFont(nfont);
        p.setPen(gray);
        p.drawRect(r);
        v=r;
        nnPoint diff = stop-start;
        if(diff.x>2 && diff.y>2)
        {
            QString s;
            s.sprintf("%d:%d",stop.x,stop.y);
            QFontMetrics fm=p.fontMetrics();
            int width = fm.width(s);
            p.setPen(black);
            r.setLeft(r.right());
            r.setRight(r.left()+width);
            r.setBottom(r.bottom()+14);
            r.setTop(r.bottom()-12);
            p.drawText(r,s);
            r.setLeft(v.left());
            s.sprintf("W:%d",stop.x-start.x);
            p.drawText(r,Qt::AlignHCenter,s);
            r=v;
            s.sprintf("%d:%d",start.x,start.y);
            width = fm.width(s);
            r.setRight(r.left());
            r.setLeft(r.right()-width-4);
            r.setTop(r.top()-14);
            r.setBottom(r.top()+12);
            p.drawText(r,s);
            r.setBottom(v.bottom());
            r.setLeft(r.left()-10);
            s.sprintf("H:%d",stop.x-start.x);
            p.drawText(r,Qt::AlignCenter,s);
        }
        p.setPen(old);
        p.setFont(oldfont);
    }
}
