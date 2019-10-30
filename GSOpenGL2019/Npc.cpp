#include "Npc.h"


Npc::Npc(BSplineCurve* inputCurve)
{
    bSplineCurve = inputCurve;
}

void Npc::update()
{
    while (gameIsRunning)
    {
        if (state == PATROL)
            patrol();
        if (state == LEARN)
            learn();
    }
}

void Npc::patrol()
{
    //Vec3 position = deBoor(t)
    //draw NPC

    if (ENDPOINT_ARRIVED)
        //notify(ENDPOINT_ARRIVED);
                state = LEARN;

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

void Npc::learn() {
    if (ITEM_TAKEN)
    {
        //remove controlpoint and one (the middle) internal knot
    }
    if (ENDPOINT_ARRIVED)
    {
        //build new path
        //npc_state = PATROL
    }
    if (ALL_ITEMS_COLLECTED)
    {
        //stop
    }
}
