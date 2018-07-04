#include "slidingstackedwidget.h"

slidingStackedWidget::slidingStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
{
    vertical = false;
    speed = 150;
    animationtype = QEasingCurve::Type::Linear;
    now = 0;
    next = 0;
    pnow = QPoint(0,0);
    active = false;
}

slidingStackedWidget::~slidingStackedWidget()
{

}

void slidingStackedWidget::setVerticalMode(bool vertical)
{
    this->vertical = vertical;
}

void slidingStackedWidget::setSpeed(int speed)
{
    this->speed = speed;
}

void slidingStackedWidget::setAnimation(const QEasingCurve::Type animationtype)
{
    this->animationtype = animationtype;
}

void slidingStackedWidget::slideInNext()
{
    int now = currentIndex();
    if (now < count() - 1)
        slideInIdx(now + 1);
}

void slidingStackedWidget::slideInPrev()
{
    int now = currentIndex();
    if (now > 0)
        slideInIdx(now - 1);
}

void slidingStackedWidget::slideInIdx(int idx, t_direction direction)
{
    // int idx, t_direction direction=AUTOMATIC
    if (idx > count() - 1) {
        direction = vertical ? TOP2BOTTOM : RIGHT2LEFT;
        idx = (idx) % count();
    }
    else if (idx < 0) {
        direction = vertical ? BOTTOM2TOP : LEFT2RIGHT;
        idx = (idx + count()) % count();
    }

    slideInWgt(widget(idx), direction);
}

void slidingStackedWidget::slideInWgt(QWidget * newwidget, t_direction direction)
{
    // do not allow re-entrance before an animation is completed.
    if (active)
        return ;
    else
        active = true;

    enum t_direction directionhint;
    int now = currentIndex();
    int next = indexOf(newwidget);
    if (now == next) {
        active = false;
        return;
    }
    else if (now < next) {
        directionhint = vertical ? TOP2BOTTOM : RIGHT2LEFT;
    }
    else {
        directionhint = vertical ? BOTTOM2TOP : LEFT2RIGHT;
    }
    if (direction == AUTOMATIC) {
        direction = directionhint;
    }

    // calculate the shifts

    int offsetx = frameRect().width();
    int offsety = frameRect().height();

    widget(next)->setGeometry(0, 0, offsetx, offsety);

    if (direction == BOTTOM2TOP) {
        offsetx = 0;
        offsety = -offsety;
    }
    else if (direction == TOP2BOTTOM) {
        offsetx = 0;
    }
    else if (direction == RIGHT2LEFT) {
        offsetx = -offsetx;
        offsety = 0;
    }
    else if (direction == LEFT2RIGHT) {
        offsety = 0;
    }

    // re-position the next widget outside/aside of the display area
    QPoint pnext = widget(next)->pos();
    QPoint pnow = widget(now)->pos();
    this->pnow = pnow;

    widget(next)->move(pnext.x() - offsetx, pnext.y() - offsety);
    // make it visible/show
    widget(next)->show();
    widget(next)->raise();

    // animate both, the now and next widget to the side, using animation framework
    QPropertyAnimation *animnow = new QPropertyAnimation(widget(now), "pos");
    animnow->setDuration(speed);
    animnow->setEasingCurve(animationtype);
    animnow->setStartValue(QPoint(pnow.x(), pnow.y()));
    animnow->setEndValue(QPoint(offsetx + pnow.x(), offsety + pnow.y()));

    QPropertyAnimation *animnext = new QPropertyAnimation(widget(next), "pos");
    animnext->setDuration(speed);
    animnext->setEasingCurve(animationtype);
    animnext->setStartValue(QPoint(-offsetx + pnext.x(), offsety + pnext.y()));
    animnext->setEndValue(QPoint(pnext.x(), pnext.y()));

    QParallelAnimationGroup *animgroup = new QParallelAnimationGroup(this);
    animgroup->addAnimation(animnow);
    animgroup->addAnimation(animnext);

    connect(animgroup, &QParallelAnimationGroup::finished, this, &slidingStackedWidget::animationDoneSlot);
    this->next = next;
    this->now = now;
    active = true;
    animgroup->start();
}

void slidingStackedWidget::animationDoneSlot()
{
    setCurrentIndex(next); // this function is inherited from QStackedWidget
    widget(now)->hide();
    widget(now)->move(pnow);
    active = false;
    emit animationFinished();
}
