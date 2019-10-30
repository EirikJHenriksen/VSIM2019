#include "Npc.h"


Npc::Npc(BSplineCurve* inputCurve)
{
    bSplineCurve = inputCurve;
}

//while (game running)
//if (npc_state == PATROL) patrol()
//if (npc_state == CHASE) chase()
//if (npc_state == LEARN) learn()

void patrol()
{
//    Vec3 position = deBoor(t)
//            draw NPC
//            if (endpoint arrived)
//            notify(endpoint_arrived)
//            npc_state = LEARN
//            if (item collected)
//            notify(control_point_index)
//            if (player detected)
//            notify(player_position)
//            npc_state = LEARN
//            else if (obstacle detected)
//            notify(obstacle_position, control_point_index)
//            npc_state = LEARN
//            else
//            previous_position = position;
}

void learn() {
//    if (ITEM_TAKEN)
//        remove controlpoint and one (the middle) internal knot
//                if (player_detected)
//                build chase path by Chasing path algorithm
//                npc_state = CHASE
//                if (obstacle_detected)
//                mark edge unavailable
//                build obstacle path by Obstacle path algorithm
//                npc_state = CHASE
//                if (endpoint arrived)
//                build new path
//                npc_state = PATROL
//                if (all items collected)
//                stop
}
