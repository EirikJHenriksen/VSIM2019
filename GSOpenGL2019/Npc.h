#ifndef NPC_H
#define NPC_H

#include "visualobject.h"
#include "bsplinecurve.h"
#include <queue>

//class BSplineCurve;

enum NPCstates{SLEEP, PATROL, LEARN, CHASE};
enum NPCevents{ENDPOINT_ARRIVED, ITEM_TAKEN, PLAYER_DETECTED, OBSTACLE_DETECTED};

typedef gs2019::Vector3D Vec3;
class Npc : public VisualObject
{
    BSplineCurve* bSplineCurve;
    Vec3 playerPos;

    //FSM Part
    int state;
    void notify(int notification);

    std::queue<int> notification_queue;

    float elapsedTime;
    // B-Spline kurven har en skjøtvektor, f.eks (0, 0, 0, 1, 2, 3, 3, 3)
    // elapsed_time er en parameter
    // 0 <= elapsed_time <= 3 i dette tilfellet

    void buildPath();
    // Lage en (ny) bane for patruljering
    // gjøre en permutasjon av kontrollpunktene (kun items, ikke endepunkter)
    // viktig å ikke endre endepunktene

public:
    Npc(BSplineCurve* inputCurve);

    // FSM Part
    void patrol();
    void learn();

    // Drawing part
    void init() override;
    void draw() override;
};

#endif // NPC_H
