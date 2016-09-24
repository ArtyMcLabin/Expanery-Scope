/* BSD 3-Clause License
 *
 *	 Copyright (c) 2016, Arty McLabin
 *	 All rights reserved.
 *
 * 	 Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are met:
 *
 *	 * Redistributions of source code must retain the above copyright notice, this
 *	   list of conditions and the following disclaimer.
 *
 *	 * Redistributions in binary form must reproduce the above copyright notice,
 *	   this list of conditions and the following disclaimer in the documentation
 *	   and/or other materials provided with the distribution.
 *
 *	 * Neither the name of the copyright holder nor the names of its
 *	   contributors may be used to endorse or promote products derived from
 *	   this software without specific prior written permission.
 *
 *	 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *	 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *	 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *	 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *	 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *	 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *	 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *	 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *	 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef ZONE_H
#define ZONE_H

#include<QWidget>
#include<QPainter>
#include<QStyleOption>
#include<QVector>
#include<QPoint>
#include<QChildEvent>

#include"engine.h"
#include"zoneobject.h"


class ZoneObject;



class Zone : public QWidget
{
    Q_OBJECT

    int timer_id;

public:
    const QPoint const_spawn_location = QPoint(150,150);
	const int timer_interval_ms = 100; //discluding scale
    QVector<ZoneObject*> creatures;
    QVector<ZoneObject*> foods;
	float time_scale=1.0;

    Zone(QWidget *parent = 0);
    Zone(int width, int height, QWidget *parent = 0);
    ~Zone(){};

    QPoint randomLocation();
	void printStates(); //states of all creatures
    void start();
    void stop();

    QSize minimumSizeHint();
    QSize sizeHint();
	

signals:

public slots:
    void childEvent(QChildEvent* event);
	void timerEvent(QTimerEvent*); //update all
    void paintEvent(QPaintEvent*);

};



#endif
