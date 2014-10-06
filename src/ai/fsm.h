#ifndef FSM_H
#define FSM_H

#include <vector>


namespace Caelum {

// Fordward declaration
class FState;

class FSM {
  public:
    FSM();
    virtual ~FSM();

    void addState(FState* state, bool makeCurrent);
    void getCurrentState() {return mCurrentState;}

  private:
    FState* mCurrentState;
    std::vector<FState*> mStates;
};

}
#endif // FSM_H
