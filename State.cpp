/*=======================================================================
*
*   Copyright (C) 2013 Lysine.
*
*   Filename:    State.cpp
*   Time:        2013/04/05
*   Author:      Lysine
*
*   Lysine is a student majoring in Software Engineering
*   from the School of Software, SUN YAT-SEN UNIVERSITY.
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.

*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
=========================================================================*/

#include "State.h"

State::State(QWidget *parent):
	QWidget(parent)
{
	isPop=false;
	opened=false;
	playing=false;
	sliding=false;
	updating=false;
	setAutoFillBackground(true);
	QPalette options;
	options.setColor(QPalette::Background,Qt::white);
	setPalette(options);
	duration=100;
	animation=new QPropertyAnimation(this,"pos",this);
	animation->setDuration(200);
	animation->setEasingCurve(QEasingCurve::OutCubic);
	timeT=new QLabel(this);
	volmT=new QLabel(this);
	timeT->setText(tr("Time"));
	volmT->setText(tr("Volume"));
	timeT->setGeometry(QRect(10,50,100,25));
	volmT->setGeometry(QRect(10,100,100,25));
	timeS=new QSlider(this);
	volmS=new QSlider(this);
	timeS->setOrientation(Qt::Horizontal);
	volmS->setOrientation(Qt::Horizontal);
	timeS->setGeometry(QRect(10,75,180,15));
	volmS->setGeometry(QRect(10,125,180,15));
	timeS->setRange(0,0);
	volmS->setRange(0,100);
	timeS->setValue(0);
	volmS->setValue(100);
	timeS->setTracking(false);
	volmS->setTracking(false);
	connect(timeS,&QSlider::valueChanged,[this](int _time){if(!updating){emit time(duration*_time/400);}});
	connect(timeS,&QSlider::sliderPressed, [this](){sliding=true;});
	connect(timeS,&QSlider::sliderReleased,[this](){sliding=false;});
	connect(volmS,&QSlider::valueChanged,[this](int _volm){emit volume(_volm);});
	playB=new QPushButton(this);
	stopB=new QPushButton(this);
	playB->setGeometry(QRect(10,15,25,25));
	stopB->setGeometry(QRect(40,15,25,25));
	playB->setIcon(QIcon(":/Interface/play.png"));
	stopB->setIcon(QIcon(":/Interface/stop.png"));
	connect(playB,&QPushButton::clicked,[this](){
		if(opened){
			setPlaying(!playing);
		}
		emit play();
	});
	connect(stopB,&QPushButton::clicked,[this](){emit stop();});
	durT=new QLabel(this);
	durT->setAlignment(Qt::AlignRight|Qt::AlignBottom);
	durT->setGeometry(QRect(70,15,120,25));
	durT->setText("00:00/00:00");
}

State::~State()
{
}

void State::pop()
{
	if(!isPop&&animation->state()==QAbstractAnimation::Stopped){
		animation->setStartValue(pos());
		animation->setEndValue(pos()-QPoint(200,0));
		animation->start();
		isPop=true;
	}
}

void State::push()
{
	if(isPop&&animation->state()==QAbstractAnimation::Stopped){
		animation->setStartValue(pos());
		animation->setEndValue(pos()+QPoint(200,0));
		animation->start();
		isPop=false;
	}
}

void State::setTime(qint64 _time)
{
	if(!sliding){
		updating=true;
		timeS->setValue(_time*400/duration);
		updating=false;
	}
	int c=_time/1000;
	int s=duration/1000;
	auto to=[](int num){
		QString res=QString::number(num);
		res.prepend(QString(2-res.length(),'0'));
		return res;
	};
	QString t;
	t+=to(c/60)+':'+to(c%60);
	t+='/';
	t+=to(s/60)+':'+to(s%60);
	durT->setText(t);
}

void State::setOpened(bool _opened)
{
	opened=_opened;
	playing=opened;
	playB->setIcon(QIcon(playing?":/Interface/pause.png":":/Interface/play.png"));
}

void State::setPlaying(bool _playing)
{
	playing=_playing;
	playB->setIcon(QIcon(playing?":/Interface/pause.png":":/Interface/play.png"));
}

void State::setDuration(qint64 _duration)
{
	if(_duration>0){
		duration=_duration;
		timeS->setRange(0,400);
	}
	else{
		duration=100;
		timeS->setValue(0);
		timeS->setRange(0,0);
		durT->setText("00:00/00:00");
	}
}
