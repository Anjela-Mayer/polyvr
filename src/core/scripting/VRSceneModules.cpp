#include "VRSceneModules.h"
#include "VRSceneGlobals.h"
#include "VRScriptManagerT.h"

#include "VRPyMath.h"
#include "VRPyNamed.h"
#include "VRPyObject.h"
#include "VRPyGeometry.h"
#include "VRPyAnimation.h"
#include "VRPySocket.h"
#include "VRPySprite.h"
#include "VRPySound.h"
#include "VRPyDevice.h"
#include "VRPyIntersection.h"
#include "VRPyPose.h"
#include "VRPyPath.h"
#include "VRPyStateMachine.h"
#include "VRPyGraph.h"
#include "VRPyPolygon.h"
#include "VRPyBoundingbox.h"
#include "VRPyTriangulator.h"
#include "VRPyStroke.h"
#include "VRPyColorChooser.h"
#include "VRPyTextureRenderer.h"
#include "VRPyTextureMosaic.h"
#include "VRPyConstraint.h"
#include "VRPyHaptic.h"
#include "VRPyMouse.h"
#include "VRPyMultiTouch.h"
#include "VRPyMobile.h"
#include "VRPyBaseT.h"
#include "VRPyMaterial.h"
#include "VRPyTextureGenerator.h"
#include "VRPyLight.h"
#include "VRPyLightBeacon.h"
#include "VRPyCamera.h"
#include "VRPyLod.h"
#include "VRPyKinematics.h"
#include "VRPyRecorder.h"
#include "VRPyPathtool.h"
#include "VRPyConstructionKit.h"
#include "VRPySnappingEngine.h"
#include "VRPyAnnotationEngine.h"
#include "VRPyAnalyticGeometry.h"
#include "VRPySelector.h"
#include "VRPySelection.h"
#include "VRPyPatchSelection.h"
#include "VRPyPolygonSelection.h"
#include "VRPySpatialCollisionManager.h"
#include "VRPyMenu.h"
#include "VRPyClipPlane.h"
#include "VRPyListMath.h"
#include "VRPySetup.h"
#include "VRPyRendering.h"
#include "VRPyNavigator.h"
#include "VRPyNavPreset.h"
#include "VRPyWaypoint.h"
#include "VRPyMeasure.h"
#include "VRPyJointTool.h"
#include "VRPyImage.h"
#include "VRPyNetworking.h"
#include "VRPyProjectManager.h"
#include "VRPyGeoPrimitive.h"
#include "VRPyProgress.h"
#include "VRPyUndoManager.h"
#include "VRPyObjectManager.h"
#include "VRPySky.h"
#include "VRPyScenegraphInterface.h"
#include "VRPyOPCUA.h"
#include "VRPyCodeCompletion.h"
#ifndef NO_ENCRYPTION
#include "VRPyEncryption.h"
#endif

#include "addons/Character/VRPyCharacter.h"
#include "addons/Algorithms/VRPyGraphLayout.h"
#include "addons/Algorithms/VRPyPathFinding.h"
#include "addons/CaveKeeper/VRPyCaveKeeper.h"
#include "addons/Bullet/Particles/VRPyParticles.h"
#include "addons/Bullet/Fluids/VRPyFluids.h"
#include "addons/Bullet/CarDynamics/VRPyCarDynamics.h"
#include "addons/Engineering/Factory/VRPyFactory.h"
#include "addons/Engineering/Factory/VRPyLogistics.h"
#include "addons/Engineering/Factory/VRPyProduction.h"
#include "addons/Engineering/Factory/VRPyAMLLoader.h"
#include "addons/Engineering/Mechanics/VRPyMechanism.h"
#include "addons/Engineering/VRPyNumberingEngine.h"
#include "addons/CEF/VRPyCEF.h"
#include "addons/CEF/VRPyWebCam.h"
#include "addons/Semantics/Segmentation/VRPySegmentation.h"
#include "addons/Semantics/Segmentation/VRPyAdjacencyGraph.h"
#include "addons/Semantics/Processes/VRPyProcess.h"
#include "addons/Engineering/Chemistry/VRPyMolecule.h"
#include "addons/Engineering/Milling/VRPyMillingMachine.h"
#include "addons/Engineering/Milling/VRPyMillingWorkPiece.h"
#include "addons/Engineering/Milling/VRPyMillingCuttingToolProfile.h"
#include "addons/Engineering/VRPyRobotArm.h"
#include "addons/WorldGenerator/VRPyWorldGenerator.h"
#include "addons/WorldGenerator/nature/VRPyNature.h"
#include "addons/WorldGenerator/terrain/VRPyTerrain.h"
#include "addons/WorldGenerator/weather/VRPyWeather.h"
#include "addons/WorldGenerator/traffic/VRPyTrafficSimulation.h"
#include "addons/Engineering/CSG/VRPyCSG.h"
#include "addons/RealWorld/VRPyRealWorld.h"
#include "addons/SimViDekont/VRPySimViDekont.h"
#include "addons/Semantics/Reasoning/VRPyOntology.h"
#include "addons/LeapMotion/VRPyHandGeo.h"
#include "addons/LeapMotion/VRPyLeap.h"

using namespace OSG;

void VRSceneModules::setup(VRScriptManager* sm, PyObject* pModVR) {
    sm->registerModule<VRPyName>("Named", pModVR, VRPyStorage::typeRef);
    sm->registerModule<VRPyObject>("Object", pModVR, VRPyName::typeRef);
    sm->registerModule<VRPyTransform>("Transform", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyCollision>("Collision", pModVR);
    sm->registerModule<VRPyGeometry>("Geometry", pModVR, VRPyTransform::typeRef);
    sm->registerModule<VRPySpatialCollisionManager>("SpatialCollisionManager", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyMaterial>("Material", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyTextureGenerator>("TextureGenerator", pModVR);
    sm->registerModule<VRPyImage>("Image", pModVR);
    sm->registerModule<VRPyLight>("Light", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyLightBeacon>("LightBeacon", pModVR, VRPyTransform::typeRef);
    sm->registerModule<VRPyCamera>("Camera", pModVR, VRPyTransform::typeRef);
    sm->registerModule<VRPyKinematics>("Kinematics", pModVR);
    sm->registerModule<VRPyFABRIK>("FABRIK", pModVR);
    sm->registerModule<VRPyLod>("Lod", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyLodLeaf>("LodLeaf", pModVR, VRPyTransform::typeRef);
    sm->registerModule<VRPyLodTree>("LodTree", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPySprite>("Sprite", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPySound>("Sound", pModVR);
    sm->registerModule<VRPySoundManager>("SoundManager", pModVR);
    sm->registerModule<VRPySocket>("Socket", pModVR);
    sm->registerModule<VRPyStroke>("Stroke", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyConstraint>("Constraint", pModVR);
    sm->registerModule<VRPyDevice>("Device", pModVR, VRPyName::typeRef);
    sm->registerModule<VRPyIntersection>("Intersection", pModVR);
    sm->registerModule<VRPyHaptic>("Haptic", pModVR, VRPyDevice::typeRef);
    sm->registerModule<VRPyServer>("Mobile", pModVR, VRPyDevice::typeRef);
    sm->registerModule<VRPyMouse>("Mouse", pModVR, VRPyDevice::typeRef);
    sm->registerModule<VRPyMultiTouch>("MultiTouch", pModVR, VRPyDevice::typeRef);
    sm->registerModule<VRPyServer>("Server", pModVR, VRPyDevice::typeRef);
    sm->registerModule<VRPyAnimation>("Animation", pModVR);
    sm->registerModule<VRPyPose>("Pose", pModVR);
    sm->registerModule<VRPyPath>("Path", pModVR);
    sm->registerModule<VRPyGraph>("Graph", pModVR);
    sm->registerModule<VRPyStateMachine>("StateMachine", pModVR);
    sm->registerModule<VRPyHDLC>("HDLC", pModVR);
    sm->registerModule<VRPyState>("State", pModVR);
    sm->registerModule<VRPyGraphLayout>("GraphLayout", pModVR);
    sm->registerModule<VRPyPathFinding>("PathFinding", pModVR);
    sm->registerModule<VRPyBoundingbox>("Boundingbox", pModVR);
    sm->registerModule<VRPyPolygon>("Polygon", pModVR);
    sm->registerModule<VRPyTriangulator>("Triangulator", pModVR);
    sm->registerModule<VRPyRecorder>("Recorder", pModVR);
    sm->registerModule<VRPyProjectManager>("ProjectManager", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyHandle>("Handle", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyGeoPrimitive>("GeoPrimitive", pModVR, VRPyTransform::typeRef);
    sm->registerModule<VRPyStorage>("Storage", pModVR);
    sm->registerModule<VRPySnappingEngine>("SnappingEngine", pModVR);
    sm->registerModule<VRPyAnnotationEngine>("AnnotationEngine", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyAnalyticGeometry>("AnalyticGeometry", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyConstructionKit>("ConstructionKit", pModVR);
    sm->registerModule<VRPyPathtool>("Pathtool", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPySelector>("Selector", pModVR);
    sm->registerModule<VRPySelection>("Selection", pModVR);
    sm->registerModule<VRPyPatchSelection>("PatchSelection", pModVR, VRPySelection::typeRef);
    sm->registerModule<VRPyPolygonSelection>("PolygonSelection", pModVR, VRPySelection::typeRef);
    sm->registerModule<VRPyNavigator>("Navigator", pModVR);
    sm->registerModule<VRPyNavPreset>("NavPreset", pModVR);
    sm->registerModule<VRPyRendering>("Rendering", pModVR);
    sm->registerModule<VRPyRenderStudio>("RenderStudio", pModVR);
    sm->registerModule<VRPySky>("Sky", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyScenegraphInterface>("ScenegraphInterface", pModVR, VRPyObject::typeRef);
#ifdef WITH_OPCUA
    sm->registerModule<VRPyOPCUA>("OPCUA", pModVR);
    sm->registerModule<VRPyOPCUANode>("OPCUANode", pModVR);
#endif

#ifndef NO_ENCRYPTION
    sm->registerModule<VRPyEncryption>("Encryption", pModVR);
#endif
    sm->registerModule<VRPyProgress>("Progress", pModVR);
    sm->registerModule<VRPyUndoManager>("UndoManager", pModVR);
    sm->registerModule<VRPyObjectManager>("ObjectManager", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyMenu>("Menu", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyClipPlane>("ClipPlane", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyWaypoint>("Waypoint", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyMeasure>("Measure", pModVR, VRPyAnalyticGeometry::typeRef);
    sm->registerModule<VRPyJointTool>("JointTool", pModVR, VRPyGeometry::typeRef);
	sm->registerModule<VRPyColorChooser>("ColorChooser", pModVR);
	sm->registerModule<VRPyTextureRenderer>("TextureRenderer", pModVR, VRPyObject::typeRef);
	sm->registerModule<VRPyTextureMosaic>("TextureMosaic", pModVR, VRPyImage::typeRef);
    sm->registerModule<VRPyCaveKeeper>("CaveKeeper", pModVR);
    sm->registerModule<VRPyParticles>("Particles", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyFluids>("Fluids", pModVR, VRPyParticles::typeRef);
    sm->registerModule<VRPyMetaBalls>("MetaBalls", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyCarDynamics>("CarDynamics", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyCarSound>("CarSound", pModVR);
    sm->registerModule<VRPyDriver>("Driver", pModVR);
    sm->registerModule<VRPyCEF>("CEF", pModVR);
    sm->registerModule<VRPyWebCam>("Webcam", pModVR, VRPySprite::typeRef);
    sm->registerModule<VRPySegmentation>("Segmentation", pModVR);
    sm->registerModule<VRPyAdjacencyGraph>("AdjacencyGraph", pModVR);
    sm->registerModule<VRPyMechanism>("Mechanism", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyNumberingEngine>("NumberingEngine", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPySkeleton>("Skeleton", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyCharacter>("Character", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyTree>("Tree", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyNature>("Nature", pModVR, VRPyObject::typeRef);
    sm->registerModule<VRPyTerrain>("Terrain", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyRain>("Rain", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyRainCarWindshield>("RainCarWindshield", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyPlanet>("Planet", pModVR, VRPyTransform::typeRef);
    sm->registerModule<VRPyMillingMachine>("MillingMachine", pModVR);
    sm->registerModule<VRPyMillingWorkPiece>("MillingWorkPiece", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyMillingCuttingToolProfile>("MillingCuttingToolProfile", pModVR);
    sm->registerModule<VRPyMolecule>("Molecule", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyCrystal>("Crystal", pModVR, VRPyMolecule::typeRef);
    sm->registerModule<VRPyRobotArm>("RobotArm", pModVR);
    sm->registerModule<VRPyOntology>("Ontology", pModVR, VRPyName::typeRef);
    sm->registerModule<VRPyProcess>("Process", pModVR, VRPyName::typeRef);
    sm->registerModule<VRPyProcessNode>("ProcessNode", pModVR, VRPyName::typeRef);
    sm->registerModule<VRPyProcessDiagram>("ProcessDiagram", pModVR, VRPyGraph::typeRef);
    sm->registerModule<VRPyProcessLayout>("ProcessLayout", pModVR, VRPyTransform::typeRef);
    sm->registerModule<VRPyProcessEngine>("ProcessEngine", pModVR);
    sm->registerModule<VRPyOntologyRule>("OntologyRule", pModVR);
    sm->registerModule<VRPyProperty>("Property", pModVR, VRPyName::typeRef);
    sm->registerModule<VRPyConcept>("Concept", pModVR, VRPyName::typeRef);
    sm->registerModule<VRPyEntity>("Entity", pModVR, VRPyName::typeRef);
    sm->registerModule<VRPyReasoner>("Reasoner", pModVR);
    sm->registerModule<VRPyScript>("Script", pModVR);

    sm->registerModule<VRPyHandGeo>("HandGeo", pModVR, VRPyGeometry::typeRef);
    sm->registerModule<VRPyLeap>("Leap", pModVR, VRPyDevice::typeRef);
    sm->registerModule<VRPyLeapFrame>("LeapFrame", pModVR);

	sm->registerModule<VRPyCSG>("CSGGeometry", pModVR, VRPyGeometry::typeRef);
	sm->registerModule<VRPyRealWorld>("RealWorld", pModVR, VRPyObject::typeRef);
	sm->registerModule<VRPyTrafficSimulation>("TrafficSimulation", pModVR, VRPyObject::typeRef);
	sm->registerModule<VRPySimViDekont>("SimViDekont", pModVR);

    PyObject* pModMath = sm->newModule("Math", VRSceneGlobals::methods, "VR math module");
    sm->registerModule<VRPyVec2f>("Vec2", pModMath, 0, "Math");
    sm->registerModule<VRPyVec3f>("Vec3", pModMath, 0, "Math");
    sm->registerModule<VRPyLine>("Line", pModMath, 0, "Math");
    sm->registerModule<VRPyExpression>("Expression", pModVR);
    sm->registerModule<VRPyMathExpression>("MathExpression", pModMath, VRPyExpression::typeRef, "Math");
    sm->registerModule<VRPyTSDF>("TSDF", pModVR, 0, "Math");
    sm->registerModule<VRPyOctree>("Octree", pModVR, 0, "Math");
    sm->registerModule<VRPyOctreeNode>("OctreeNode", pModVR, 0, "Math");

    PyObject* pModSetup = sm->newModule("Setup", VRSceneGlobals::methods, "VR setup module");
    sm->registerModule<VRPySetup>("Setup", pModSetup, 0, "Setup");
    sm->registerModule<VRPyView>("View", pModSetup, 0, "Setup");
    sm->registerModule<VRPyWindow>("Window", pModSetup, 0, "Setup");

    PyObject* pModWorldGenerator = sm->newModule("WorldGenerator", VRSceneGlobals::methods, "VR world generator module");
    sm->registerModule<VRPyWorldGenerator>("WorldGenerator", pModWorldGenerator, VRPyTransform::typeRef, "WorldGenerator");
    sm->registerModule<VRPyAsphalt>("Asphalt", pModWorldGenerator, VRPyMaterial::typeRef, "WorldGenerator");
    sm->registerModule<VRPyRoadBase>("RoadBase", pModWorldGenerator, VRPyObject::typeRef, "WorldGenerator");
    sm->registerModule<VRPyRoad>("Road", pModWorldGenerator, VRPyRoadBase::typeRef, "WorldGenerator");
    sm->registerModule<VRPyRoadIntersection>("RoadIntersection", pModWorldGenerator, VRPyRoadBase::typeRef, "WorldGenerator");
    sm->registerModule<VRPyRoadNetwork>("RoadNetwork", pModWorldGenerator, VRPyRoadBase::typeRef, "WorldGenerator");
    sm->registerModule<VRPyTrafficSigns>("TrafficSigns", pModWorldGenerator, VRPyRoadBase::typeRef, "WorldGenerator");
    sm->registerModule<VRPyDistrict>("District", pModWorldGenerator, 0, "WorldGenerator");

    PyObject* pModFactory = sm->newModule("Factory", VRSceneGlobals::methods, "VR factory module");
    sm->registerModule<FPyNode>("Node", pModFactory, 0, "Factory");
    sm->registerModule<FPyNetwork>("Network", pModFactory, 0, "Factory");
    sm->registerModule<FPyPath>("FPath", pModFactory, 0, "Factory");
    sm->registerModule<FPyTransporter>("Transporter", pModFactory, 0, "Factory");
    sm->registerModule<FPyContainer>("Container", pModFactory, 0, "Factory");
    sm->registerModule<FPyProduct>("Product", pModFactory, 0, "Factory");
    sm->registerModule<FPyLogistics>("Logistics", pModFactory, 0, "Factory");
    sm->registerModule<VRPyFactory>("Factory", pModFactory, 0, "Factory");
    sm->registerModule<VRPyProduction>("Production", pModFactory, 0, "Factory");
    sm->registerModule<VRPyAMLLoader>("AMLLoader", pModFactory, 0, "Factory");
}




