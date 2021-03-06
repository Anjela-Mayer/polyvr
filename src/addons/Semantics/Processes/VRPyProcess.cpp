#include "VRPyProcess.h"
#include "addons/Semantics/Reasoning/VRPyOntology.h"
#include "core/scripting/VRPyBaseT.h"
#include "core/scripting/VRPyGraph.h"
#include "core/scripting/VRPyGeometry.h"
#include "core/scripting/VRPyTypeCaster.h"
#include "core/scripting/VRPyBaseFactory.h"

using namespace OSG;

simpleVRPyType(Process, New_named_ptr);
simpleVRPyType(ProcessNode, 0);
simpleVRPyType(ProcessDiagram, 0);
simpleVRPyType(ProcessLayout, New_VRObjects_ptr);
simpleVRPyType(ProcessEngine, New_ptr);

PyMethodDef VRPyProcess::methods[] = {
    {"open", PyWrap(Process, open, "Open file - open(path)", void, string ) },
    {"setOntology", PyWrap(Process, setOntology, "Set data from ontology - open(ontology)", void, VROntologyPtr ) },
    {"getOntology", PyWrap(Process, getOntology, "Get ontology", VROntologyPtr ) },
    {"getInteractionDiagram", PyWrap(Process, getInteractionDiagram, "Return subjects interaction diagram - getInteractionDiagram()", VRProcessDiagramPtr ) },
    {"getBehaviorDiagram", PyWrap(Process, getBehaviorDiagram, "Return subject behavior diagram - getBehaviorDiagram( int ID )", VRProcessDiagramPtr, int ) },
    {"isFunctionState", PyWrap(Process, isFunctionState, "Check whether the ProcessNode is a Function State.", bool, VRProcessNodePtr) },
    {"isSendState", PyWrap(Process, isSendState, "Check whether the ProcessNode is a Send State.", bool, VRProcessNodePtr) },
    {"isReceiveState", PyWrap(Process, isReceiveState, "Check whether the ProcessNode is a receive State.", bool, VRProcessNodePtr) },
    {"isInitialState", PyWrap(Process, isInitialState, "Check whether the ProcessNode is an initial State.", bool, VRProcessNodePtr) },
    {"getSubjects", PyWrap(Process, getSubjects, "Return subjects - [ProcessNode] getSubjects()", vector<VRProcessNodePtr> ) },
    {"getMessages", PyWrap(Process, getMessages, "Return messages - [ProcessNode] getMessages()", vector<VRProcessNodePtr> ) },
    {"getSubjectMessages", PyWrap(Process, getSubjectMessages, "Return subject messages - [ProcessNode] getSubjectMessages(subject)", vector<VRProcessNodePtr>, int ) },
    {"getMessageSubjects", PyWrap(Process, getMessageSubjects, "Return message subjects - [ProcessNode] getMessageSubjects(message)", vector<VRProcessNodePtr>, int ) },
    {"getMessageSender", PyWrap(Process, getMessageSender, "Returns the sender of the message - [ProcessNode] getMessageSender(message)", vector<VRProcessNodePtr>, int ) },
    {"getMessageReceiver", PyWrap(Process, getMessageReceiver, "Returns the receiver of the message - [ProcessNode] getMessageReceiver(message)", vector<VRProcessNodePtr>, int ) },
    {"getSubject", PyWrap(Process, getSubject, "Return subject by ID", VRProcessNodePtr, int ) },
    {"getSubjectByName", PyWrap(Process, getSubjectByName, "Return subject by name", VRProcessNodePtr, string ) },
    {"getSubjectState", PyWrap(Process, getSubjectState, "Return subject action", VRProcessNodePtr, int, string ) },
    {"getSubjectStates", PyWrap(Process, getSubjectStates, "Return subject actions - [ProcessNode] getSubjectStates(subject)", vector<VRProcessNodePtr>, int ) },
    {"getStateTransitions", PyWrap(Process, getStateTransitions, "Return action transitions - [ProcessNode] getActionTransitions(subject, action)", vector<VRProcessNodePtr>, int, int ) },
    //{"getTransitionStates", PyWrap(Process, getTransitionStates, "Return actions connected by a given transition - [ProcessNode] getTransitionActions(subject, transition)", vector<VRProcessNodePtr>, int, int ) },
    {"getStateMessages", PyWrap(Process, getStateMessages, "Returns the Messages of a send/receive state", vector<VRProcessNodePtr>, VRProcessNodePtr ) },
    {"addSubject", PyWrapOpt(Process, addSubject, "Add a new subject - ProcessNode addSubject( name )", "0", VRProcessNodePtr, string, VREntityPtr ) },
    {"addState", PyWrapOpt(Process, addState, "Add a new state to subject, by subject ID", "0", VRProcessNodePtr, string, int, VREntityPtr ) },
    {"setInitialState", PyWrap(Process, setInitialState, "Set a state to initial state.", void, VRProcessNodePtr ) },

    {"setSendState", PyWrap(Process, setSendState, "Set a state to send a message (sender, sendTransition, recvSubject, message)", void, VRProcessNodePtr, VRProcessNodePtr, int, string ) },
    {"setReceiveState", PyWrap(Process, setReceiveState, "Set a state to wait for a message (receiver, recvTransition, sendSubject, message)", void, VRProcessNodePtr, VRProcessNodePtr, int, string ) },
    {"addMessage", PyWrapOpt(Process, addMessage, "Add a new message between subjects or actions i and j - ProcessNode addMessage( name, int i, int j )", "0|0", VRProcessNodePtr, string, int, int, VRProcessDiagramPtr, VREntityPtr ) },
    {"addTransition", PyWrapOpt(Process, addTransition, "Add a new transition between actions i and j - ProcessNode addTransition( name, subject, int i, int j )", "0|0", VRProcessNodePtr, string, int, int, int, VRProcessDiagramPtr, VRProcessCbPtr ) },
    {"getNode", PyWrapOpt(Process, getNode, "Get node by ID", "0", VRProcessNodePtr, int, VRProcessDiagramPtr ) },
    {"getNodeByName", PyWrapOpt(Process, getNodeByName, "Get node by name", "0", VRProcessNodePtr, string, VRProcessDiagramPtr ) },
   {NULL}  /* Sentinel */
};

PyMethodDef VRPyProcessNode::methods[] = {
    {"getLabel", PyWrap(ProcessNode, getLabel, "Get node label", string) },
    {"getID", PyWrap(ProcessNode, getID, "Get node graph ID", int) },
    {"getEntity", PyWrap(ProcessNode, getEntity, "Get entity", VREntityPtr) },
    {"getSubjectID", PyWrap(ProcessNode, getSubjectID, "Get subject ID", int) },
    {"setWaitForExternalAction", PyWrap(ProcessNode, setWaitForExternalAction, "Set an action state to wait for external actions", void, bool) },
    {NULL}  /* Sentinel */
};

PyMethodDef VRPyProcessDiagram::methods[] = {
    {"getNodes", PyWrap(ProcessDiagram, getNodes, "Get nodes", vector<VRProcessNodePtr>) },
    {NULL}  /* Sentinel */
};

PyMethodDef VRPyProcessLayout::methods[] = {
    {"setParameters", PyWrap(ProcessLayout, setParameters, "Set layout parameters - (height, scale)", void, float, float ) },
    {"getSIDPathtool", PyWrap(ProcessLayout, getSIDPathtool, "Access path tool", VRPathtoolPtr ) },
    {"getSBDPathtool", PyWrap(ProcessLayout, getSBDPathtool, "Access path tool", VRPathtoolPtr, int ) },
    {"setProcess", PyWrap(ProcessLayout, setProcess, "Set process - setProcess( process )", void, VRProcessPtr ) },
    {"getProcess", PyWrap(ProcessLayout, getProcess, "Get process - ProcessNodePtr getProcess()", VRProcessPtr) },
    {"getElement", PyWrap(ProcessLayout, getElement, "Return element by ID - obj getElement( int ID )", VRObjectPtr, int ) },
    {"getElementID", PyWrap(ProcessLayout, getElementID, "Return element ID - ID getElementID( VRObjectPtr geo )", int, VRObjectPtr ) },
    {"getProcessNode", PyWrap(ProcessLayout, getProcessNode, "Return process node by ID - process node getElementID( int i )", VRProcessNodePtr, int ) },
    {"addElement", PyWrap(ProcessLayout, addElement, "Add process element", VRObjectPtr, VRProcessNodePtr) },
    {"selectElement", PyWrap(ProcessLayout, selectElement, "Select a node geometry by changing its appearance", void, VRGeometryPtr ) },
    {"setElementName", PyWrap(ProcessLayout, setElementName, "Change the name of a node", void, int, string ) },
    {"remElement", PyWrap(ProcessLayout, remElement, "Remove element", void, VRObjectPtr ) },
    {"setEngine", PyWrap(ProcessLayout, setEngine, "Set process engine", void, VRProcessEnginePtr ) },
    {"storeLayout", PyWrapOpt(ProcessLayout, storeLayout, "Store layout to file", "", void, string ) },
    {"loadLayout", PyWrapOpt(ProcessLayout, loadLayout, "Load layout from file", "", void, string ) },
    {"pauseUpdate", PyWrapOpt(ProcessLayout, pauseUpdate, "Pause update", "1", void, bool ) },
    {"updatePathtools", PyWrap(ProcessLayout, updatePathtools, "Update Process Pathtools", void ) },
    {NULL}  /* Sentinel */
};

PyMethodDef VRPyProcessEngine::methods[] = {
    {"setProcess", PyWrap(ProcessEngine, setProcess, "Set process - setProcess( process )", void, VRProcessPtr ) },
    {"getProcess", PyWrap(ProcessEngine, getProcess, "Get the current process - process getProcess()", VRProcessPtr ) },
    {"run", PyWrapOpt(ProcessEngine, run, "Run the simulation with a simulation tick speed in seconds", "1", void, float ) },
    {"reset", PyWrap(ProcessEngine, reset, "Reset simulation - reset()", void ) },
    {"pause", PyWrap(ProcessEngine, pause, "Pause simulation - pause()", void ) },
    {"getCurrentStates", PyWrap(ProcessEngine, getCurrentStates, "Current states of all subjects - [ProcessNode] getCurrentStates()", vector<VRProcessNodePtr> ) },
    {"getTraversedPath", PyWrap(ProcessEngine, getTraversedPath, "returns the traversed path for a subject - [ProcessNode] getTraversedPath(int subjectID)", vector<VRProcessNodePtr>, int ) },
    {"getCurrentState", PyWrap(ProcessEngine, getCurrentState, "returns the current state of a subject - ProcessNode getCurrentState(int subjectID)", VRProcessNodePtr, int ) },
    {"continueWith", PyWrap(ProcessEngine, continueWith, "Continue execution with this next transition", void, VRProcessNodePtr ) },
    {"tryAdvance", PyWrap(ProcessEngine, tryAdvance, "Try advancing subject sID", void, int ) },
    {"finishAction", PyWrap(ProcessEngine, finishAction, "Finish action of subject sID", void, int ) },
    {"sendMessage", PyWrap(ProcessEngine, sendMessage, "Send a message (message, senderID, receiverID)", void, string, int, int ) },
    {NULL}  /* Sentinel */
};
