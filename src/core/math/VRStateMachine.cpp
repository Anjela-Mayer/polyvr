#include "VRStateMachine.h"

using namespace OSG;

template<class P>
VRStateMachine<P>::State::State(string name, VRTransitionCbPtr t) {
    auto ns = setNameSpace("State");
    ns->setUniqueNames(false);
    setName(name);
    transitionCb = t;
}

template<class P>
VRStateMachine<P>::State::~State() {}

template<class P>
typename VRStateMachine<P>::StatePtr VRStateMachine<P>::State::create(string name, VRTransitionCbPtr t) { return StatePtr( new State(name, t) ); }

template<class P>
string VRStateMachine<P>::State::process(const P& params) {
    if (!transitionCb) return ""; //state transition CB
    return (*transitionCb)(params);
}

template<class P>
void VRStateMachine<P>::State::setStateEnterCB(VRStateEnterCbPtr ptr){
    enterCb = ptr;
}

template<class P>
void VRStateMachine<P>::State::setStateLeaveCB(VRStateLeaveCbPtr ptr){
    leaveCb = ptr;
}

template<class P>
void VRStateMachine<P>::State::enter() {
    if (enterCb) (*enterCb)(0); //state enter CB
}

template<class P>
void VRStateMachine<P>::State::leave() {
    if (leaveCb) (*leaveCb)(0); //state leave CB
}

template<class P>
VRStateMachine<P>::VRStateMachine(string name) {
    setNameSpace("StateMachine");
    setName(name);
}

template<class P>
VRStateMachine<P>::~VRStateMachine() {}

template<class P>
shared_ptr<VRStateMachine<P>> VRStateMachine<P>::create(string name) { return shared_ptr<VRStateMachine<P>>( new VRStateMachine<P>(name) ); }

template<class P>
typename VRStateMachine<P>::StatePtr VRStateMachine<P>::addState(string s, VRTransitionCbPtr t) {
    auto state = State::create(s,t);
    states[s] = state;
    return state;
}

template<class P> typename VRStateMachine<P>::StatePtr VRStateMachine<P>::setCurrentState(string s) { if (states.count(s)) currentState = states[s]; return currentState; }
template<class P> typename VRStateMachine<P>::StatePtr VRStateMachine<P>::getState(string s) { return states.count(s) ? states[s] : 0; }
template<class P> typename VRStateMachine<P>::StatePtr VRStateMachine<P>::getCurrentState() { return currentState; }

template<class P>
typename VRStateMachine<P>::StatePtr VRStateMachine<P>::process(const P& params) {
    if (!currentState) return 0;
    string newStateName = currentState->process(params);
    if (newStateName != currentState->getName()) { // state change
        if (states.count(newStateName)) {
            currentState->leave();
            setCurrentState(newStateName);
            states[newStateName]->enter();
        }
    }
    return currentState;
}
