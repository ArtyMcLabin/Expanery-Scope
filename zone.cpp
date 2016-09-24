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

#include"zone.h"

using namespace std;



Zone::Zone(QWidget* parent) : QWidget(parent)
{
    setStyleSheet("background-color: rgb(224, 226, 182);");
    qsrand(time(0));
}

Zone::Zone(int width, int height, QWidget* parent) : Zone::Zone(parent)
{
    this->resize(width,height);
}

QPoint Zone::randomLocation()
{
    return QPoint(qrand()%(width()+1),qrand()%(height()+1));
}

void Zone::printStates()
{
    for(ZoneObject* zo : creatures){
        zo->printState();
    }
}

void Zone::start()
{
	timer_id = startTimer(timer_interval_ms / time_scale);
}
void Zone::stop()
{
    killTimer(timer_id);
}





void Zone::childEvent(QChildEvent* event)
{

    ZoneObject* zo;
    if((zo = qobject_cast<ZoneObject*>(event->child())) == NULL){
        return;
    }else if(zo->inherits("Food")){
        foods.push_back(zo);
    }else{
        creatures.push_back(zo);
    }
    zo->initLocation();
    zo->show();

}

void Zone::timerEvent(QTimerEvent*)
{
    for(ZoneObject* zo : creatures){
        zo->progress();
		zo->updateGeometry();
		D<<"global geometry update";
    }
    for(ZoneObject* zo : foods){
        zo->progress();
		//+ zo->updateGeometry();
    }

    VERBOSE(printStates())
}

void Zone::paintEvent(QPaintEvent*)
{
    //init
    QStyleOption option;
    option.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);

    painter.setPen(Qt::black);


}
