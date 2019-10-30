#ifndef NPC_H
#define NPC_H

#include "visualobject.h"
#include "bsplinecurve.h"
#include <queue>

//class BSplineCurve;

// We use the concepts event, notification and transition in this context.
enum NPCstates{PATROL, LEARN};
// the notifications are sent from the NPC to itself.
// The reason for this is that not all transitions can be done immediately.
enum NPCevents{NOTHING, ENDPOINT_ARRIVED, ITEM_TAKEN, ALL_ITEMS_COLLECTED};

typedef gs2019::Vector3D Vec3;
class Npc : public VisualObject
{
    NPCstates state{PATROL};
    NPCevents event{NOTHING};

    void update();
    bool gameIsRunning{true};

    BSplineCurve* bSplineCurve;
    Vec3 playerPos;

    //FSM Part
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
    void init() override {};
    void draw() override {};
};

#endif // NPC_H
