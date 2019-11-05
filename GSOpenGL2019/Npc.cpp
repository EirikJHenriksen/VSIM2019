#include "Npc.h"
#include "bsplinecurve.h"

Npc::Npc(BSplineCurve* inputCurve, VisualObject* owner)
{
    bSplineCurve = inputCurve;
    enemy = owner;
}

void Npc::update()
{
    if (state == PATROL)
        patrol();
    if (state == LEARN)
        learn();
}

void Npc::patrol()
{
    qDebug() << "curveTime" << time;

    // move
    time += 0.1f;
    float curveTime = MathLab::abs(cos(time*0.02f));
    enemy->setLocation(bSplineCurve->travelAlongSpline(curveTime).x, bSplineCurve->travelAlongSpline(curveTime).y, bSplineCurve->travelAlongSpline(curveTime).z);

    //if (ENDPOINT_ARRIVED)
    //notify(ENDPOINT_ARRIVED);

    if (isStillNearEndpoint)
    {
        // if far from both endpoints
        if ((enemy->getPosition()) - (bSplineCurve->b[0]) > (collisionDistance)&&(enemy->getPosition()) - (bSplineCurve->b[curvePoints-1]) < collisionDistance)
        {
            isStillNearEndpoint = false;
        }
    }
    else
    {
        // at an endpoint?
        if ((enemy->getPosition()) - (bSplineCurve->b[0]) < collisionDistance)
        {
            notification_queue.push(ENDPOINT_ARRIVED);
            state = LEARN;
        }
        else if ((enemy->getPosition()) - (bSplineCurve->b[curvePoints-1]) < collisionDistance)
        {
            notification_queue.push(ENDPOINT_ARRIVED);
            state = LEARN;
        }
    }


    //                if (item collected)
    //                notify(control_point_index)

    //                if (player detected)
    //                notify(player_position)
    //                npc_state = LEARN

    //                else if (obstacle detected)
    //                notify(obstacle_position, control_point_index)
    //                npc_state = LEARN

    //                else
    //                previous_position = position;
}

void Npc::learn()
{
    qDebug() << "learn()";

    isStillNearEndpoint = true;

    if (!notification_queue.empty())
    {
        switch(notification_queue.front())
        {
        case ITEM_TAKEN:
            //remove controlpoint and one (the middle) internal knot
            break;
        case ENDPOINT_ARRIVED:
            //build new path
            //npc_state = PATROL
            break;
        case ALL_ITEMS_COLLECTED:
            //stop
            break;
        }

        notification_queue.pop();

    }
    else // queue empty
    {
        state = PATROL;
    }
}

void Npc::checkCurve()
{
    if (bSplineCurve->b->length() < curvePoints)
    {
        curvePoints = bSplineCurve->b->length();
        notification_queue.push(ITEM_TAKEN);
        qDebug() << "curve point removed";
    }
}
