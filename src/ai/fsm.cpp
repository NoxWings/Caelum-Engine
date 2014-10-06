#include "fsm.h"

using namespace Caelum;

FSM::FSM() {
    mCurrentState = null;
    mStates.clear();
    // Intro, Menu, Game, Pause, GameOver +1 Bonus = 6
    mStates.reserve(6);
}

FSM::~FSM() {
    mCurrentState = null;
    mStates.clear();
}

void FSM::addState(FState *state, bool makeCurrent) {
    mStates.push_back(state);
    if (makeCurrent) {
        mCurrentState = state;
    }
}
