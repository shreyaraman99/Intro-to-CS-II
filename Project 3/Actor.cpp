#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include "GameWorld.h"


/**************** ACTOR ******************/
Actor::Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* world) : GraphObject (imageID, startX, startY, dir, size, depth), m_world(world) {
    m_alive = true;
    m_world = world;
    
}

Actor::~Actor() {
    
}

StudentWorld* Actor::getStudentWorld() const {
    return m_world;
}

void Actor::moveToIfCan(double x, double y) {
    bool canMove = getStudentWorld()->canActorMoveTo(this, x, y);
    if (!canMove)
        return;
    moveTo(x, y);
}

void Actor::setAlive() {
    m_alive = true;
}

void Actor::setDead() {
    m_alive = false;
}

int Actor::getID() { 
    return m_ID;
}

/***************** STAR ******************/

Star::Star(double startX, double startY, StudentWorld* world) : Actor(IID_STAR, startX, startY, 0, 0.45*rand() / RAND_MAX + 0.05, 3, world) {
    m_alive = true;
}

void Star::doSomething() {
    moveTo(getX() - 1, getY());
}

bool Star::isAlive() {
    return m_alive;
}


/**************** EXPLOSION **************/
Explosion::Explosion(double startX, double startY, StudentWorld* world) : Actor(IID_EXPLOSION, startX, startY, 0, 0.666667, 0, world) {
    m_alive = true;
}

void Explosion::setDead() {
    m_alive = false;
}

bool Explosion::isAlive() {
    return m_alive;
}

void Explosion::doSomething() {
    if (!isAlive())
        return;
    
    setSize(1.5 * getSize());
    
    if (getSize() > 5.0625) {
        setDead();
        return;
    }
}





/***************** NACHENBLASTER ********************/
NachenBlaster::NachenBlaster(StudentWorld* world): Actor(IID_NACHENBLASTER, 0, 128, 0, 1.0, 0, world) {
    m_hitPoints = 50;
    m_cabbagePoints = 30;
    m_torpedoes = 0;
    m_alive = true;
}

void NachenBlaster::doSomething() {
    if(!isAlive())
        return;
    
    if (m_hitPoints <= 0)
        m_alive = false;
    
    int ch;
    if (getStudentWorld()->getKey(ch)) {
        switch(ch) {
            case KEY_PRESS_LEFT:
                moveToIfCan(getX() - 6, getY());
                break;
            case KEY_PRESS_RIGHT:
                moveToIfCan(getX() + 6, getY());
                break;
            case KEY_PRESS_UP:
                moveToIfCan(getX(), getY() + 6);
                break;
            case KEY_PRESS_DOWN:
                moveToIfCan(getX(), getY() - 6);
                break;
            case KEY_PRESS_SPACE:
                if (m_cabbagePoints >= 5) {
                    getStudentWorld()->addActor(new Cabbage(getX() + 12, getY(), getStudentWorld()));
                    m_cabbagePoints -=5;
                    getStudentWorld()->playSound(SOUND_PLAYER_SHOOT);
                    
                }
                break;
            case KEY_PRESS_TAB:
                if (m_torpedoes > 0) {
                    getStudentWorld()->addActor(new Torpedo(getX() + 12, getY(), 0, getStudentWorld()));
                    m_torpedoes--;
                    getStudentWorld()->playSound(SOUND_TORPEDO);
                }
                break;
        }
    }
}

void NachenBlaster::addCabbages() {
    if (m_cabbagePoints <= 29)
        m_cabbagePoints ++;
}

void NachenBlaster::loseHitPoints(int hits) {
    m_hitPoints -= hits;
}

void NachenBlaster::addHitPoints(int hits) {
    if (m_hitPoints <= 50 - hits)
        m_hitPoints += hits;
}

void NachenBlaster::addTorpedoes(int torpedoes) {
    m_torpedoes += torpedoes;
}

bool NachenBlaster::isAlive() {
    return m_alive;
}

int NachenBlaster::getHealth() {
    return m_hitPoints * 2;
}

int NachenBlaster::getCabbagePercent() {
    return m_cabbagePoints * (10.0/3.0);
}

int NachenBlaster::getTorpedoInventory() {
    return m_torpedoes;
}



/***************** ALIEN *******************/
Alien::Alien(int imageID, double startX, double startY, StudentWorld* world, int flight_plan, double speed): Actor(imageID, startX, startY, 0, 1.5, 1, world) {
    m_ID = imageID;
    m_flightPlan = flight_plan;
    m_speed = speed;
}

void Alien::travelInDirection() {
    if (m_travelDirection == 1)
        moveToIfCan(getX() - m_speed, getY() - m_speed);
    else if (m_travelDirection == 2)
        moveToIfCan(getX() - m_speed, getY() + m_speed);
    else if (m_travelDirection == 3)
        moveToIfCan(getX() - m_speed, getY());
}

void Alien::moveDownAndLeft() {
    m_travelDirection = 1;
}


void Alien::moveUpAndLeft() {
    m_travelDirection = 2;
    
}

void Alien::moveDueLeft() {
    m_travelDirection = 3;
}

int Alien::getID() {
    return m_ID;
}



/****************** SMALLGON ******************/
Smallgon::Smallgon(double startX, double startY, StudentWorld* world) : Alien(IID_SMALLGON, startX, startY, world, 0, 2.0) {
    m_ID = IID_SMALLGON;
    m_alive = true;
    m_flightPlan = 0;
    m_speed = 2.0;
    m_hitPoints = 5 * (1 + (getStudentWorld()->getLevel() - 1) * 0.1);
}

bool Smallgon::isAlive() {
    return m_alive;
}

int Smallgon::flightPlan() {
    return m_flightPlan;
}

int Smallgon::travelSpeed() {
    return m_speed;
}

void Smallgon::possiblyDropGoodie() {}

void Smallgon::whenAlienDies() {
    getStudentWorld()->increaseScore(250);
    m_alive = false;
    getStudentWorld()->killShip();
    getStudentWorld()->playSound(SOUND_DEATH);
    getStudentWorld()->addActor(new Explosion(getX(), getY(), getStudentWorld()));
    return;
}

void Smallgon::sufferDamage(int hits) {
    m_hitPoints -= hits;
    if (m_hitPoints <= 0) {
        whenAlienDies();
        return;
    }
    else
        getStudentWorld()->playSound(SOUND_BLAST);
    
}



void Smallgon::doSomething() {
    if(!isAlive())
        return;
    if (getX() < 2) {       // should be < 0 but the smallgon stays on the screen unless i put < 2
        m_alive = false;
        getStudentWorld()->shipDead();
        return;
    }
    
    // check for collision with nachenblaster
    if (getStudentWorld()->collisionNB(getX(), getY(), getRadius())) {
        getStudentWorld()->NBloseHits(5);
        whenAlienDies();
        return;
    }
    
    
    if (flightPlan() == 0 || getY() <= 0 || getY() >= VIEW_HEIGHT - 1) {
        if (getY() >= VIEW_HEIGHT - 1)
            moveDownAndLeft();
        else if (getY() <= 0)
            moveUpAndLeft();
        if (flightPlan() == 0) {
            int random = rand() % 3 + 1;
            if (random == 1)
                moveUpAndLeft();
            if (random == 2)
                moveDownAndLeft();
            if (random == 3)
                moveDueLeft();
        }
        m_flightPlan = rand() % 32 + 1;
    }
    
    if (getStudentWorld()->isNearNB(getX(), getY())) {
        if ( rand() % ( (20 / getStudentWorld()->getLevel()) + 5 ) + 1 == 1) {
            getStudentWorld()->addActor(new Turnip(getX() - 14, getY(), getStudentWorld()));
            getStudentWorld()->playSound(SOUND_ALIEN_SHOOT);
            return;
        }
    }
    
    travelInDirection();
    m_flightPlan--;
    
}


/****************** SMOREGON ******************/
Smoregon::Smoregon(double startX, double startY, StudentWorld* world) : Alien(IID_SMOREGON, startX, startY, world, 0, 2.0) {
    m_ID = IID_SMOREGON;
    m_alive = true;
    m_flightPlan = 0;
    m_speed = 2.0;
    m_hitPoints = 5 * (1 + (getStudentWorld()->getLevel() - 1) * 0.1);
}

bool Smoregon::isAlive() {
    return m_alive;
}

int Smoregon::flightPlan() {
    return m_flightPlan;
}

int Smoregon::travelSpeed() {
    return m_speed;
}

void Smoregon::possiblyDropGoodie() {
    if (rand() % 3 + 1 == 2) {
        if (rand() % 2 + 1 == 1)
            getStudentWorld()->addActor(new RepairGoodie(getX(), getY(), getStudentWorld()));
        else
            getStudentWorld()->addActor(new TorpedoGoodie(getX(), getY(), getStudentWorld()));
    }
}

void Smoregon::whenAlienDies() {
    getStudentWorld()->increaseScore(250);
    getStudentWorld()->killShip();
    m_alive = false;
    getStudentWorld()->addActor(new Explosion(getX(), getY(), getStudentWorld()));
    getStudentWorld()->playSound(SOUND_DEATH);
    possiblyDropGoodie();
}

void Smoregon::sufferDamage(int hits) {
    m_hitPoints -= hits;
    if (m_hitPoints <= 0) {
        whenAlienDies();
        return;
    }
    else
        getStudentWorld()->playSound(SOUND_BLAST);
    
}



void Smoregon::doSomething() {
    if(!isAlive())
        return;
    if (getX() < 2) {       // should be < 0 but the smoregon stays on the screen unless i put < 2
        m_alive = false;
        getStudentWorld()->shipDead();
        return;
    }
    
    // check for collision with nachenblaster
    if (getStudentWorld()->collisionNB(getX(), getY(), getRadius())) {
        getStudentWorld()->NBloseHits(5);
        whenAlienDies();
        return;
    }
    
    
    
    if (flightPlan() == 0 || getY() <= 0 || getY() >= VIEW_HEIGHT - 1) {
        if (getY() >= VIEW_HEIGHT - 1)
            moveDownAndLeft();
        else if (getY() <= 0)
            moveUpAndLeft();
        if (flightPlan() == 0) {
            int random = rand() % 3 + 1;
            if (random == 1)
                moveUpAndLeft();
            if (random == 2)
                moveDownAndLeft();
            if (random == 3)
                moveDueLeft();
        }
        m_flightPlan = rand() % 32 + 1;
    }
    
    if (getStudentWorld()->isNearNB(getX(), getY())) {
        if ( rand() % ( (20 / getStudentWorld()->getLevel()) + 5 ) + 1 == 1) {
            getStudentWorld()->addActor(new Turnip(getX() - 14, getY(), getStudentWorld()));
            getStudentWorld()->playSound(SOUND_ALIEN_SHOOT);
            return;
        }
        if ( rand() % ( (20 / getStudentWorld()->getLevel()) + 5 ) + 1 == 2) {
            moveDueLeft();
            m_flightPlan = VIEW_WIDTH;
            m_speed = 5;
        }
    }
    
    travelInDirection();
    m_flightPlan--;
    
}


/****************** SNAGGLEGON ******************/
Snagglegon::Snagglegon(double startX, double startY, StudentWorld* world) : Alien(IID_SNAGGLEGON, startX, startY, world, 0, 1.75) {
    m_ID = IID_SNAGGLEGON;
    m_alive = true;
    m_flightPlan = 0;
    m_speed = 2.0;
    m_hitPoints = 10 * (1 + (getStudentWorld()->getLevel() - 1) * 0.1);
    moveDownAndLeft();
}

bool Snagglegon::isAlive() {
    return m_alive;
}

int Snagglegon::flightPlan() {
    return m_flightPlan;
}

int Snagglegon::travelSpeed() {
    return m_speed;
}

void Snagglegon::possiblyDropGoodie() {
    if (rand() % 6 + 1 == 2)
        getStudentWorld()->addActor(new ExtraLifeGoodie(getX(), getY(), getStudentWorld()));
}

void Snagglegon::whenAlienDies() {
    getStudentWorld()->increaseScore(1000);
    getStudentWorld()->killShip();
    m_alive = false;
    getStudentWorld()->addActor(new Explosion(getX(), getY(), getStudentWorld()));
    getStudentWorld()->playSound(SOUND_DEATH);
    possiblyDropGoodie();
    
}

void Snagglegon::sufferDamage(int hits) {
    m_hitPoints -= hits;
    if (m_hitPoints <= 0) {
        whenAlienDies();
        return;
    }
    else
        getStudentWorld()->playSound(SOUND_BLAST);
    
}




void Snagglegon::doSomething() {
    if(!isAlive())
        return;
    if (getX() < 2) {       // should be < 0 but the snagglegon stays on the screen unless i put < 2
        m_alive = false;
        getStudentWorld()->shipDead();
        return;
    }

    // check for collision with nachenblaster
    if (getStudentWorld()->collisionNB(getX(), getY(), getRadius())) {
        getStudentWorld()->NBloseHits(15);
        whenAlienDies();
        return;
    }
    
    
    if (getY() <= 0 || getY() >= VIEW_HEIGHT - 1) {
        if (getY() >= VIEW_HEIGHT - 1)
            moveDownAndLeft();
        else if (getY() <= 0)
            moveUpAndLeft();
    }
    
    if (getStudentWorld()->isNearNB(getX(), getY())) {
        if ( rand() % ( (15 / getStudentWorld()->getLevel()) + 10 ) + 1 == 1) {
            getStudentWorld()->addActor(new Torpedo(getX() - 14, getY(), 180, getStudentWorld()));
            getStudentWorld()->playSound(SOUND_TORPEDO);
            return;
        }
    }
    
    travelInDirection();
    
}






/***************** CABBAGE ******************/
Cabbage::Cabbage(double startX, double startY, StudentWorld* world) : Actor(IID_CABBAGE, startX, startY, 0, 0.5, 1, world) {
    m_alive = true;
}


bool Cabbage::isAlive() {
    return m_alive;
}

void Cabbage::doSomething() {
    if(!isAlive())
        return;
    if (getX() >= VIEW_WIDTH - 8) {    // should be VIEW_WIDTH but the cabbage stays on the screen unless i put VIEW_WIDTH - 8
        m_alive = false;
        return;
    }
    
    // if collided with alien
    if (getStudentWorld()->collideWithAlien(this) != nullptr) {
        Alien* a = static_cast<Alien*>(getStudentWorld()->collideWithAlien(this));
        a->sufferDamage(2);
        m_alive = false;
        return;
    }
    
    else {
        moveToIfCan(getX() + 8, getY());
        setDirection(getDirection() + 20);
    }
    
}

/***************** TURNIP ******************/
Turnip::Turnip(double startX, double startY, StudentWorld* world) : Actor(IID_TURNIP, startX, startY, 0, 0.5, 1, world) {
    m_alive = true;
}


bool Turnip::isAlive() {
    return m_alive;
}

void Turnip::doSomething() {
    if(!isAlive())
        return;
    if (getX() < 8) {    // should be < 0 but the turnip stays on the screen unless i put < 8
        m_alive = false;
        return;
    }
    
    // check for collision with nachenblaster
    if (getStudentWorld()->collisionNB(getX(), getY(), getRadius())) {
        getStudentWorld()->NBloseHits(2);
        m_alive = false;
        return;
    }
    else {
        moveToIfCan(getX() - 6, getY());
        setDirection(getDirection() + 20);
    }
}


/***************** TORPEDO ******************/
Torpedo::Torpedo(double startX, double startY, int dir, StudentWorld* world) : Actor(IID_TORPEDO, startX, startY, dir, 0.5, 1, world) {
    m_alive = true;
}

bool Torpedo::isAlive() {
    return m_alive;
}



void Torpedo::doSomething() {
    if(!isAlive())
        return;
    
    // fired by snagglegon
    if (getDirection() == 180) {
        if (getX() < 8) {           // should be < 0 but the torpedo stays on the screen unless i put < 8
            m_alive = false;
            return;
        }
        
        // check for collision with nachenblaster
        if (getStudentWorld()->collisionNB(getX(), getY(), getRadius())) {
            getStudentWorld()->NBloseHits(8);
            m_alive = false;
            return;
        }
        else {
            moveToIfCan(getX() - 8, getY());
        }
    }
    
     // fired by nachenblaster
    if (getDirection() == 0) {
        if (getX() >= VIEW_WIDTH - 8) {         // should be VIEW_WIDTH but the torpedo stays on the screen unless i put VIEW_WIDTH - 8
            m_alive = false;
            return;
        }
        
        // collision with alien ship
        if (getStudentWorld()->collideWithAlien(this) != nullptr) {
            Alien* a = static_cast<Alien*>(getStudentWorld()->collideWithAlien(this));
            a->sufferDamage(8);
            m_alive = false;
            return;
        }
        
        else {
            moveToIfCan(getX() + 8, getY());
        }
    }
}




/***************** GOODIE *******************/
Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* world): Actor(imageID, startX, startY, 0, 0.5, 1, world) {
    m_alive = true;
}

void Goodie::doSomething() {
    if (!isAlive())
        return;
    if (getX() <= 0 || getY() <= 0) {
        m_alive = false;
        return;
    }
    
    // check for collision with nachenblaster
    if (getStudentWorld()->collisionNB(getX(), getY(), getRadius())) {
        getStudentWorld()->increaseScore(100);
        m_alive = false;
        getStudentWorld()->playSound(SOUND_GOODIE);
        grantSpecificReward();
        return;
    }
    moveTo(getX() - 0.75, getY() - 0.75);
    
}

bool Goodie::isAlive() {
    return m_alive;
}



/********** REPAIR GOODIE ************/
RepairGoodie::RepairGoodie(double startX, double startY, StudentWorld* world): Goodie(IID_REPAIR_GOODIE, startX, startY, world) {}
void RepairGoodie::grantSpecificReward(){
    getStudentWorld()->NBgainHits(5);
}

/********** EXTRA LIFE GOODIE ************/
ExtraLifeGoodie::ExtraLifeGoodie(double startX, double startY, StudentWorld* world): Goodie(IID_LIFE_GOODIE, startX, startY, world) {}
void ExtraLifeGoodie::grantSpecificReward(){
    getStudentWorld()->incLives();
}



/********** TORPEDO GOODIE ************/
TorpedoGoodie::TorpedoGoodie(double startX, double startY, StudentWorld* world): Goodie(IID_TORPEDO_GOODIE, startX, startY, world) {}
void TorpedoGoodie::grantSpecificReward(){
    getStudentWorld()->NBgainTorpedoes(5);
}
