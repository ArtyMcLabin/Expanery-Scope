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

#include"cell.h"

using namespace std;

int Cell::cells_population_size = 0;

void Cell::wildCard()
{

	speed = bioparent->speed + engine::randomSign(speed/10);
//	if(speed<=0)
//		speed=numeric_limits<typeof(speed)>::min();//ensure non zero division
//- always positive

	eating_speed = bioparent->eating_speed + engine::randomSign(eating_speed/10); //# caused negative eating duration bug //?
//	if(eating_speed<=0)
//		eating_speed=numeric_limits<typeof(eating_speed)>::min();//ensure non zero division
//- always positive

	maximal_mass = bioparent->maximal_mass + engine::randomSign((maximal_mass-minimal_mass)/10);
//	if(maximal_mass<=minimal_mass)
//		maximal_mass=minimal_mass+numeric_limits<typeof(minimal_mass)>::min();//ensure max>min
//- always bigger
}

Cell::Cell(Zone* zone) : ZoneObject(zone)
{
	id = ++cells_population_size;
    VERBOSE(D << "new cell: "<< id << " has born!";);

    //on stopping movement
    connect(location_animator, &QPropertyAnimation::stateChanged,this,&Cell::onReachedFood);
}


Cell::Cell(Cell* bioparent) : Cell::Cell(bioparent->zone)
{
    //Cell(bioparent->zone);
    this->mass = bioparent->mass/2;
    this->bioparent = bioparent;
    this->wildCard(); //evolve a bit
}

Cell::~Cell()
{
    zone->creatures.removeOne(this);
}

void Cell::ageUp() //- inlinify into progress()
{
    age++;
	mass -= getHunger()*(zone->timer_interval_ms / 1000.0 / zone->time_scale); // (Mass/Sec)*(Sec) = Mass

    if(mass < minimal_mass)
        die();
    if(mass > maximal_mass){
        split();
    }
 }

float Cell::getHunger()
{
	return mass/40.0;
}

double Cell::getRadius()
{
    return sqrt(mass/3.14);
}

QColor Cell::getGNAColor()
{
    int r=150-default_maximal_mass+maximal_mass; //maximal_mass
    int g=150-default_speed+speed; //speed
    int b=150-default_eating_speed+speed; //eating_speed

    if(r>255)r=255;
    if(g>255)g=255;
    if(b>255)b=255;

    if(r<0)r=0;
    if(g<0)g=0;
    if(b<0)b=0;

    return QColor(r,g,b,200);
}

void Cell::updateGeometry()
{
	int radius = getRadius();
    setGeometry(getLocation().x()-radius,getLocation().y()-radius,radius*2,radius*2);
}

unsigned Cell::findFood()
{
	if(zone->foods.empty()){
		D<<id<<": foods empty";
        return -1;
	}

    QPoint this_loc = getLocation();
	unsigned dist = -2; //maximal unsigned value before -1 which is error
    unsigned temp_dist;

	int i=0;//-

	target_food=nullptr; //ensure null if no food found
    for(ZoneObject* food : zone->foods){
        if(((Food*)food)->is_getting_fest)
            continue;
		i++;//-
        temp_dist = engine::distance(this_loc,food->getLocation());
		if(temp_dist < dist){
			dist = temp_dist;
            target_food = (Food*)food;
        }
    }
	//D<<id<<"searched free foods:"<<i<< "/" <<zone->foods.size();
	//D<<id<<"chasing:"<<target_food; //% nullptr


	if(target_food == nullptr){
        return -1;
    }

	connect(target_food, &Food::destroyed, this, &Cell::onTargetFoodDestroyed);
    return dist;
}

void Cell::startMoveToFood(unsigned distance)
{
    QRect geo_now = geometry();
    QPoint food_loc = target_food->geometry().topLeft();

    location_animator->setStartValue(geo_now);
    location_animator->setEndValue(QRect(food_loc.x(),food_loc.y(),geo_now.width(),geo_now.height()));
    //location_animator->setEasingCurve(QEasingCurve::OutQuart);
	//# location_animator->setDuration(distance/speed *1000); // *1000 is seconds->ms
			unsigned long duration = distance/speed *1000 /zone->time_scale; // *1000 is seconds->ms
			duration = duration > std::numeric_limits<int>::max() ? std::numeric_limits<int>::max() : duration;
			location_animator->setDuration((int)duration);
	location_animator->start();
}

void Cell::startConsuming()
{D<<id<<"startConsuming()";
    target_food->is_getting_fest = true;
	target_food->who_festing = this;

	//D<<id<<"eating duration will be:"<<target_food->nutrition / eating_speed * 1000 / zone->time_scale;
	QTimer::singleShot(target_food->nutrition / eating_speed * 1000 / zone->time_scale, this, &Cell::onDoneConsuming);
}


void Cell::die()
{
	D<<this->id << "died::::::::::::::::speed:"<<speed<<"eating_speed:"<<eating_speed<<"max_mass:"<<maximal_mass;
	if(target_food && target_food->who_festing==this){
		target_food->is_getting_fest=0; //% will cause bug if enable multiple creatures eat same food
		target_food->who_festing=nullptr;
	}
    this->deleteLater();
}

void Cell::split()
{
	new Cell(this);
    mass /= 2;
    updateGeometry();
}

QPoint Cell::initLocation()
{
    int r = getRadius();
    QPoint spawn;

	if(bioparent==nullptr){
        spawn = zone->const_spawn_location;
    }else{
        spawn = bioparent->getLocation();
    }

    setGeometry(spawn.x()-r, spawn.y()-r ,2*r ,2*r);
    return geometry().center();
}

void Cell::progress()
{
    ageUp();

    if(is_busy)
        return;

    if(location_animator->state()==QPropertyAnimation::Stopped){
		unsigned distance = findFood(); //also sets target_food
			if(distance==(unsigned)-1)
				D<<id<<": food not found!";

		if(target_food!=nullptr){
			startMoveToFood(distance);
		}
    }
}

void Cell::printState()
{
    //D << id << ":\t" << mass;
}

void Cell::onTargetFoodDestroyed()
{D<<id<<"onTargetFoodDestroyed()"<<target_food;

	target_food = nullptr;
    location_animator->stop();
}

void Cell::onReachedFood()
{
	//ensure reached
	if(location_animator->state()!=QPropertyAnimation::Stopped || target_food==nullptr){
		return; //# not really reached food, fake alarm
	}
	D<<id<<"onReachedFood()"<<target_food;
	if(!is_busy && !target_food->is_getting_fest){
		D<<id<<"food free on reach";
        is_busy=true;
		startConsuming();
	}else{
		D<<id<<"food busy, disconnecting"<<target_food;
		target_food->disconnect(this);
	}
}

void Cell::onDoneConsuming()
{
	D<<id<<"onDoneConsuming()"<<target_food;//-

	if(!target_food){
		D<<id<<"if(!target_food)return; triggered";
        return;
	}
    mass += target_food->nutrition;
    updateGeometry();
    target_food->deleteLater(); //safe destructor
	target_food=nullptr;
    is_busy = false;
}


void Cell::paintEvent(QPaintEvent*)
{
	  //init
      QStyleOption option;
      option.init(this);
      QPainter painter(this);
      painter.setPen(Qt::white);
      painter.setBrush(QBrush(getGNAColor()));

	  //-
	  painter.drawRect(geometry());

      //shape
      int r = getRadius();
      painter.drawEllipse(r,r,r,r);
}

