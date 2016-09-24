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

#ifndef CELL_H
#define CELL_H

#include<QWidget>
#include<QStyleOption>
#include<QPainter>
#include<QBrush>
#include<QPoint>
#include<QPropertyAnimation>
#include<QEasingCurve>
#include<QTimer>

#include"engine.h"
#include"zoneobject.h"
#include"food.h"




class Cell : public ZoneObject
{
    Q_OBJECT

protected:
    QPropertyAnimation* location_animator = new QPropertyAnimation(this,"geometry");

public:
    //personal
    unsigned long age = 0;
    unsigned id;
    float mass = 150.0;
    Cell* bioparent = NULL;
    Food* target_food = NULL;
    bool is_busy=false;


    //default DNA
    static constexpr float default_speed = 30.0;
    static constexpr float default_eating_speed = 30.0; //nutrition per sec
    static constexpr float default_minimal_mass = 50.0;
    static constexpr float default_maximal_mass = 300.0;

    //DNA
    float speed = default_speed;
    float eating_speed = default_eating_speed; //nutrition per sec
    float minimal_mass = default_minimal_mass;
    float maximal_mass = default_maximal_mass;
    void wildCard();


    //static
    static int cells_population_size;
	

    explicit Cell(Zone* zone); //dry spawn
    explicit Cell(Cell* bioparent); //parented spawn (cell split)
    ~Cell();

    void ageUp();
    float getHunger();
	double getRadius();
    QColor getGNAColor();
    void updateGeometry();

    unsigned findFood(); //returns distance to the food
		void startMoveToFood(unsigned distance);
            void startConsuming();


	void die(); //differs from destructor, for future implementation of dead body
    void split();

    QPoint initLocation();
    void progress();
    void printState();

	

public slots:
	void onTargetFoodDestroyed();
    void onReachedFood();
    void onDoneConsuming();

    void paintEvent(QPaintEvent*);

};



#endif
