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
    // move
    time += 0.1f;
    float curveTime = MathLab::abs(cos(time*0.02f));
    qDebug() << "curveTime" << curveTime;
    enemy->setLocation(bSplineCurve->travelAlongSpline(curveTime).x, bSplineCurve->travelAlongSpline(curveTime).y, bSplineCurve->travelAlongSpline(curveTime).z);

    //if (ENDPOINT_ARRIVED)
    //notify(ENDPOINT_ARRIVED);

    // at endpoint?
    if ((enemy->getPosition()) - (bSplineCurve->b[nextEndpoint]) < collisionDistance)
    {
        notification_queue.push(ENDPOINT_ARRIVED);
        state = LEARN;
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
    if (nextEndpoint == bSplineCurve->b->length())
    {
        nextEndpoint = 0;
    }
    else
    {
        nextEndpoint = bSplineCurve->b->length();
    }

    qDebug() << "learn()";
    if (!notification_queue.empty())
    {
        notification_queue.pop();
        //    if (ITEM_TAKEN)
        //    {
        //        //remove controlpoint and one (the middle) internal knot
        //    }
        //    if (ENDPOINT_ARRIVED)
        //    {
        //        //build new path
        //        //npc_state = PATROL
        //    }
        //    if (ALL_ITEMS_COLLECTED)
        //    {
        //        //stop
        //    }
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
