#include "VRML.h"
#include "core/utils/VRProgress.h"
#include "core/utils/toString.h"
#include "core/math/pose.h"
#include "core/objects/geometry/VRGeoData.h"
#include "core/objects/geometry/sprite/VRSprite.h"
#include "core/objects/material/VRMaterial.h"
#include "core/objects/VRLight.h"
#include "core/objects/VRLightBeacon.h"
#include "core/objects/VRCamera.h"

#include <string>
#include <iostream>
#include <fstream>
#include <stack>

#include <OpenSG/OSGColor.h>
#include <OpenSG/OSGQuaternion.h>

using namespace OSG;

struct VRMLUtils {
    int version = 1;

    bool isNumber(string number) {
        if (number[0] >= '0' && number[0] <= '9') return true;
        if (number[0] == '-' || number[0] == '+' || number[0] == '.') {
            if (number[1] >= '0' && number[1] <= '9') return true;
        }
        return false;
    }

    bool isBool(string b) {
        if (b == "false" || b == "False" || b == "FALSE") return true;
        if (b == "true" || b == "True" || b == "TRUE") return true;
        return false;
    }

    bool isBracket(string bracket) {
        if (bracket == "{" || bracket == "}") return true;
        if (bracket == "{}") return true;

        if (version == 2) {
            if (bracket == "[" || bracket == "]") return true;
            if (bracket == "[]") return true;
        }

        return false;
    }

    bool isGroupNode(string node) {
        if (version == 1) {
            if (node == "Group") return true;
            if (node == "Separator") return true;
            if (node == "Switch") return true;
            if (node == "TransformSeparator") return true;
        }

        if (version == 2) {
            if (node == "children") return true;
        }

        return false;
    }

    bool isGeometryNode(string node) {
        if (node == "IndexedFaceSet") return true;
        if (node == "IndexedLineSet") return true;
        if (node == "PointSet") return true;
        if (node == "Cone") return true;
        if (node == "Sphere") return true;
        if (node == "Cylinder") return true;

        if (version == 1) {
            if (node == "Cube") return true;
            if (node == "AsciiText") return true;
        }

        if (version == 2) {
            if (node == "Box") return true;
            if (node == "ElevationGrid") return true;
            if (node == "Extrusion") return true;
            if (node == "Text") return true;
        }

        return false;
    }

    bool isPropertyNode(string node) {
        if (node == "DirectionalLight") return true;
        if (node == "PointLight") return true;
        if (node == "SpotLight") return true;
        if (node == "Normal") return true;
        if (node == "FontStyle") return true;
        if (node == "Material") return true;
        if (node == "LOD") return true;
        if (node == "Transform") return true;

        if (version == 1) {
            if (node == "Coordinate3") return true;
            if (node == "Info") return true;
            if (node == "MaterialBinding") return true;
            if (node == "NormalBinding") return true;
            if (node == "Texture2") return true;
            if (node == "Texture2Transform") return true;
            if (node == "TextureCoordinate2") return true;
            if (node == "ShapeHints") return true;
            if (node == "MatrixTransform") return true;
            if (node == "Rotation") return true;
            if (node == "Scale") return true;
            if (node == "Translation") return true;
            if (node == "OrthographicCamera") return true;
            if (node == "PerspectiveCamera") return true;
        }

        if (version == 2) {
            if (node == "Group") return true;
            if (node == "Anchor") return true;
            if (node == "Billboard") return true;
            if (node == "Collision") return true;
            if (node == "Inline") return true;
            if (node == "Switch") return true;
            if (node == "AudioClip") return true;
            if (node == "Script") return true;
            if (node == "Shape") return true;
            if (node == "Sound") return true;
            if (node == "WorldInfo") return true;
            if (node == "Color") return true;
            if (node == "Coordinate") return true;
            if (node == "TextureCoordinate") return true;
            if (node == "Appearance") return true;
            if (node == "ImageTexture") return true;
            if (node == "MovieTexture") return true;
            if (node == "PixelTexture") return true;
            if (node == "TextureTransform") return true;
        }

        return false;
    }

    bool isOtherNode(string node) {
        if (version == 1) {
            if (node == "WWWAnchor") return true;
            if (node == "WWWInline") return true;
        }

        if (version == 2) {
            if (node == "CylinderSensor") return true;
            if (node == "PlaneSensor") return true;
            if (node == "ProximitySensor") return true;
            if (node == "SphereSensor") return true;
            if (node == "TimeSensor") return true;
            if (node == "TouchSensor") return true;
            if (node == "VisibilitySensor") return true;
            if (node == "ColorInterpolator") return true;
            if (node == "CoordinateInterpolator") return true;
            if (node == "NormalInterpolator") return true;
            if (node == "OrientationInterpolator") return true;
            if (node == "PositionInterpolator") return true;
            if (node == "ScalarInterpolator") return true;
            if (node == "Background") return true;
            if (node == "Fog") return true;
            if (node == "NavigationInfo") return true;
            if (node == "Viewpoint") return true;
        }
        return false;
    }

    bool isNode(string node) {
        if (isGroupNode(node)) return true;
        if (isGeometryNode(node)) return true;
        if (isPropertyNode(node)) return true;
        if (isOtherNode(node)) return true;
        return false;
    }

    bool isTransformationNode(string node) {
        if (version == 1) {
            if (node == "MatrixTransform") return true;
            if (node == "Rotation") return true;
            if (node == "Scale") return true;
            if (node == "Transform") return true;
            if (node == "Translation") return true;
        }

        if (version == 2) {}

        return false;
    }

    bool isTransformationResetNode(string node) {
        if (version == 1) {
            if (node == "Separator") return true;
            if (node == "TransformSeparator") return true;
        }

        if (version == 2) {}

        return false;
    }

};

struct VRMLNode : VRMLUtils {
    string name;
    string type;
    VRMLNode* parent = 0;
    vector<VRMLNode*> children;
    map<string, string> params;
    vector<Vec3d> positions;
    vector<Vec3d> normals;
    vector<int> coordIndex;
    VRObjectPtr obj;
    VRMaterialPtr material;
    Matrix4d pose;

    VRMLNode(string t, string n = "Unnamed") : name(n), type(t) {}
    virtual ~VRMLNode() {
        for (auto c : children) delete c;
    }

    virtual VRMLNode* addChild(string t, string n) = 0;

    void print(string padding = "") {
        cout << padding << "Node '" << name << "' of type " << type << " with params [";
        for (auto p : params) cout << " (" << p.first << " : " << p.second << ") ";
        cout << "]" << endl;
        for (auto c : children) c->print(padding+" ");
    }


    // build OSG ---------------------

    vector<string> parseStringField(string mf) {
        vector<string> v;
        string s;
        int Qstate = 0;
        int Estate = 0;
        for (auto c : mf) {
            if (c == '\\') { // handle escape character
                if (Estate == 0) { Estate = 1; continue; }
                if (Estate == 1) Estate = 0;
            }

            if (c == '"' && Estate == 0) { // handle quote character
                if (Qstate == 0) {
                    s = "";
                    Qstate = 1;
                    continue;
                }

                if (Qstate == 1) {
                    Qstate = 0;
                    v.push_back(s);
                    continue;
                }
            }

            s += c;
        }
        if (v.size() == 0) v.push_back(mf);
        return v;
    }

    template<typename T>
    vector<T> splitNumberMF(string mf) {
        vector<T> v;
        string s;
        for (auto c : mf) {
            if (c == '[' || c == ']') continue; // ignore brackets
            if (c == ',') { // handle next number
                v.push_back( toValue<T>(s) );
                s = "";
                continue;
            }
            s += c;
        }
        v.push_back( toValue<T>(s) );
        return v;
    }

    typedef map<string,bool> BitMask;

    BitMask parseBitMask(map<string, string>& data, string var, vector<string> bits, vector<string> def) {
        vector<string> v;
        if (data.count(var)) {
            string mf = data[var];

            string s;
            for (auto c : mf) {
                if (c == '(' || c == ')') continue; // ignore brackets
                if (c == '|') { // handle next bit
                    v.push_back( s );
                    s = "";
                    continue;
                }
                s += c;
            }
            v.push_back( s );
        } else v = def;

        BitMask m;
        for (auto b : bits) m[b] = false;
        for (auto b : v)    m[b] = true;
        return m;
    }

    bool checkBit(BitMask& bitMask, const string& bit) {
        if (bitMask.count("ALL")) if (bitMask["ALL"]) return true;
        if (bitMask.count(bit)) return bitMask[bit];
        return false;
    }

    bool getSFBool(map<string, string>& data, string var, bool def) {
        return data.count(var) ? toValue<bool>(data[var]) : def;
    }

    float getSFFloat(map<string, string>& data, string var, float def) {
        return data.count(var) ? toValue<float>(data[var]) : def;
    }

    Color3f getSFColor(map<string, string>& data, string var, Color3f def) {
        return data.count(var) ? toValue<Color3f>(data[var]) : def;
    }

    Vec3d getSFVec3f(map<string, string>& data, string var, Vec3d def) {
        return data.count(var) ? toValue<Vec3d>(data[var]) : def;
    }

    Vec4d getSFRotation(map<string, string>& data, string var, Vec4d def) {
        return data.count(var) ? toValue<Vec4d>(data[var]) : def;
    }

    vector<int> getMFLong(map<string, string>& data, string var, vector<int> def) {
        return data.count(var) ? toValue<vector<int>>(data[var]) : def;
    }

    vector<float> getMFFloat(map<string, string>& data, string var, vector<float> def) {
        return data.count(var) ? toValue<vector<float>>(data[var]) : def;
    }

    vector<Color3f> getMFColor(map<string, string>& data, string var, vector<Color3f> def) {
        return data.count(var) ? toValue<vector<Color3f>>(data[var]) : def;
    }

    vector<Vec3d> getMFVec3f(map<string, string>& data, string var, vector<Vec3d> def) {
        return data.count(var) ? toValue<vector<Vec3d>>(data[var]) : def;
    }


    VRObjectPtr makeObject() {
        cout << "make object '" << name << "' of type " << type << endl;
        if (isGroupNode(type)) return VRObject::create(name);
        if (isGeometryNode(type)) {
            if (type == "AsciiText") return VRSprite::create(name);
            return VRGeometry::create(name);
        }
        if (isPropertyNode(type)) {
            if (type == "PointLight") return VRLight::create(name);
            if (type == "DirectionalLight") return VRLight::create(name);
            if (type == "SpotLight") return VRLight::create(name);
            if (type == "PerspectiveCamera") return VRCamera::create(name, false);
        }
        return VRObject::create(name);
    }

    void applyPose() {
        VRTransformPtr t = dynamic_pointer_cast<VRTransform>(obj);
        t->setMatrix(pose);
    }

    void applyMaterial() {
        VRGeometryPtr g = dynamic_pointer_cast<VRGeometry>(obj);
        if (g && material) g->setMaterial(material);
    }

    void handleAsciiText(map<string, string> data) {
        float spacing = getSFFloat(data, "spacing", 1);
        string justification = "LEFT"; // SFEnum
        vector<string> text({string()}); // MFString
        vector<float> width({0}); // MFFloat
        if (data.count("justification")) justification = data["justification"];
        if (data.count("string")) text = parseStringField( data["string"] );
        if (data.count("width")) width = splitNumberMF<float>( data["width"] );
        VRSpritePtr s = dynamic_pointer_cast<VRSprite>(obj);
        if (!s) { cout << "WARNING in VRML handleAsciiText, cast failed" << endl; return; }
        string t;
        for (auto l : text) t += l + "\n";
        s->setLabel(t);
        applyPose();
        applyMaterial();
    }

    void handleCone(map<string, string> data) {
        BitMask parts = parseBitMask(data, "parts", {"SIDES", "BOTTOM", "ALL"}, {"ALL"});
        float bottomRadius = getSFFloat(data, "bottomRadius", 1);
        float height = getSFFloat(data, "height", 2);
        VRGeometryPtr geo = dynamic_pointer_cast<VRGeometry>(obj);
        if (!geo) { cout << "WARNING in VRML handleCone, cast failed" << endl; return; }
        bool doBottom = checkBit(parts, "BOTTOM");
        bool doSides = checkBit(parts, "SIDES");
        geo->setPrimitive("Cone " + toString(height) + " " + toString(bottomRadius) + " 16 " + toString(doBottom) + " " + toString(doSides));
        applyPose();
        applyMaterial();
    }

    void handleCube(map<string, string> data) {
        float width = getSFFloat(data, "width", 2);
        float height = getSFFloat(data, "height", 2);
        float depth = getSFFloat(data, "depth", 2);
        VRGeometryPtr geo = dynamic_pointer_cast<VRGeometry>(obj);
        if (!geo) { cout << "WARNING in VRML handleCube, cast failed" << endl; return; }
        geo->setPrimitive("Box " + toString(width) + " " + toString(height) + " " + toString(depth) + " 1 1 1");
        applyPose();
        applyMaterial();
    }

    void handleSphere(map<string, string> data) {
        float radius = getSFFloat(data, "radius", 1);
        VRGeometryPtr geo = dynamic_pointer_cast<VRGeometry>(obj);
        if (!geo) { cout << "WARNING in VRML handleSphere, cast failed" << endl; return; }
        geo->setPrimitive("Sphere " + toString(radius) + " 2");
        applyPose();
        applyMaterial();
    }

    void handleCylinder(map<string, string> data) {
        BitMask parts = parseBitMask(data, "parts", {"SIDES", "TOP", "BOTTOM", "ALL"}, {"ALL"}); // SFBitMask
        float radius = getSFFloat(data, "radius", 1);
        float height = getSFFloat(data, "height", 2);
        VRGeometryPtr geo = dynamic_pointer_cast<VRGeometry>(obj);
        if (!geo) { cout << "WARNING in VRML handleCylinder, cast failed" << endl; return; }
        bool doTop = checkBit(parts, "TOP");
        bool doBottom = checkBit(parts, "BOTTOM");
        bool doSides = checkBit(parts, "SIDES");
        geo->setPrimitive("Cylinder " + toString(height) + " " + toString(radius) + " 16 " + toString(doTop) + " " + toString(doBottom) + " " + toString(doSides));
        applyPose();
        applyMaterial();
    }

    void handlePointLight(map<string, string> data) {
        bool on = getSFBool(data, "on", 1);
        float intensity = getSFFloat(data, "intensity", 1);
        Color3f color = getSFColor(data, "color", Color3f(1,1,1));
        Vec3d location = getSFVec3f(data, "location", Vec3d(0,0,1));

        VRLightPtr light = dynamic_pointer_cast<VRLight>(obj);
        if (!light) { cout << "WARNING in VRML handlePointLight, cast failed" << endl; return; }
        light->setPointlight();
        light->setOn(on);
        light->setDiffuse(toColor4f(color*intensity, 1));
        auto beacon = light->addBeacon();
        beacon->setFrom(location);
    }

    void handleDirectionalLight(map<string, string> data) {
        bool on = getSFBool(data, "on", 1);
        float intensity = getSFFloat(data, "intensity", 1);
        Color3f color = getSFColor(data, "color", Color3f(1,1,1));
        Vec3d direction = getSFVec3f(data, "direction", Vec3d(0,0,-1));

        VRLightPtr light = dynamic_pointer_cast<VRLight>(obj);
        if (!light) { cout << "WARNING in VRML handleDirectionalLight, cast failed" << endl; return; }
        light->setDirectionallight();
        light->setOn(on);
        light->setDiffuse(toColor4f(color*intensity, 1));
        auto beacon = light->addBeacon();
        beacon->setDir(direction);
    }

    void handleSpotLight(map<string, string> data) {
        bool on = getSFBool(data, "on", 1);
        float intensity = getSFFloat(data, "intensity", 1);
        Color3f color = getSFColor(data, "color", Color3f(1,1,1));
        Vec3d location = getSFVec3f(data, "location", Vec3d(0,0,1));
        Vec3d direction = getSFVec3f(data, "direction", Vec3d(0,0,-1));
        float dropOffRate = getSFFloat(data, "dropOffRate", 1); // TODO
        float cutOffAngle = getSFFloat(data, "cutOffAngle", 1);

        VRLightPtr light = dynamic_pointer_cast<VRLight>(obj);
        if (!light) { cout << "WARNING in VRML handleSpotLight, cast failed" << endl; return; }
        light->setSpotlight();
        light->setOn(on);
        light->setDiffuse(toColor4f(color*intensity, 1));
        auto beacon = light->addBeacon();
        beacon->setFrom(location);
        beacon->setDir(direction);
    }

    void handlePerspectiveCamera(map<string, string> data) {
        Vec3d position = getSFVec3f(data, "position", Vec3d(0,0,1));
        Vec4d orientation = getSFRotation(data, "orientation", Vec4d(0,0,1,0));
        float focalDistance = getSFFloat(data, "focalDistance", 5);
        float heightAngle = getSFFloat(data, "heightAngle", 0.785398);

        VRCameraPtr c = dynamic_pointer_cast<VRCamera>(obj);
        if (!c) { cout << "WARNING in VRML handlePerspectiveCamera, cast failed" << endl; return; }
        c->rotate(orientation[3], Vec3d(orientation[0], orientation[1], orientation[2]));
        c->setFrom(position);
        c->setType(VRCamera::PERSPECTIVE);
        c->setAspect(1);
        c->setFov(1.0/heightAngle);
    }

    void handleTranslation(map<string, string> data) {
        pose.setTranslate( getSFVec3f(data, "translation", Vec3d(0,0,0)) );
    }

    void handleRotation(map<string, string> data) {
        Vec4d rotation = getSFRotation(data, "rotation", Vec4d(0,0,1,0));
        pose.setRotate( Quaterniond( Vec3d(rotation[0], rotation[1], rotation[2]), rotation[3] ) );
    }

    void handleScale(map<string, string> data) {
        pose.setScale( getSFVec3f(data, "scaleFactor", Vec3d(1,1,1)) );
    }

    void handleTransform(map<string, string> data) {
        Vec3d translation = getSFVec3f(data, "translation", Vec3d(0,0,0));
        Vec4d rotation = getSFRotation(data, "rotation", Vec4d(0,0,1,0));
        Vec3d scaleFactor = getSFVec3f(data, "scaleFactor", Vec3d(1,1,1));
        Vec4d scaleOrientation = getSFRotation(data, "scaleOrientation", Vec4d(0,0,1,0));
        Vec3d center = getSFVec3f(data, "center", Vec3d(0,0,0));
        Matrix4d m1; m1.setTranslate(translation+center); m1.setRotate( Quaterniond( Vec3d(rotation[0], rotation[1], rotation[2]), rotation[3] ) );
        Matrix4d m2; m2.setRotate( Quaterniond( Vec3d(scaleOrientation[0], scaleOrientation[1], scaleOrientation[2]), scaleOrientation[3] ) );
        Matrix4d m3; m3.setScale(scaleFactor);
        Matrix4d m4; m4.setTranslate(-center); m4.setRotate( Quaterniond( Vec3d(scaleOrientation[0], scaleOrientation[1], scaleOrientation[2]), -scaleOrientation[3] ) );
        Matrix4d M = m1; M.mult(m2); M.mult(m3); M.mult(m4);
        pose = M;
    }

    void handleCoordinate3(map<string, string> data) {
        positions = getMFVec3f(data, "point", {});
    }

    void handleNormal(map<string, string> data) {
        normals = getMFVec3f(data, "normal", {});
    }

    void handleIndexedFaceSet(map<string, string> data) {
        coordIndex = getMFLong(data, "coordIndex", {0});
        //vector<int> materialIndex = getMFLong(data, "materialIndex", {-1}); // TODO
        //vector<int> normalIndex = getMFLong(data, "normalIndex", {-1});
        //vector<int> textureCoordIndex = getMFLong(data, "textureCoordIndex", {-1});
    }

    void handleMaterial(map<string, string> data) {
        vector<Color3f> ambientColor = getMFColor(data, "ambientColor", {Color3f(0.2,0.2,0.2)});
        vector<Color3f> diffuseColor = getMFColor(data, "diffuseColor", {Color3f(0.8,0.8,0.8)});
        vector<Color3f> specularColor = getMFColor(data, "specularColor", {Color3f(0,0,0)});
        vector<Color3f> emissiveColor = getMFColor(data, "emissiveColor", {Color3f(0,0,0)});
        vector<float> shininess = getMFFloat(data, "shininess", {0.2});
        vector<float> transparency = getMFFloat(data, "transparency", {0});
        auto m = VRMaterial::create("material");
        for (int i=0; i<diffuseColor.size(); i++) {
            if (i > 0) m->addPass();
            m->setAmbient( ambientColor[i] );
            m->setDiffuse( diffuseColor[i] );
            m->setSpecular( specularColor[i] );
        }
        material = m;
    }

    void applyProperties() {
        if (isGroupNode( type )) return; // group nodes have no properties
        if (isOtherNode( type )) return; // other nodes are ignored
        else if (type == "PointLight") handlePointLight(params);
        else if (type == "SpotLight") handleSpotLight(params);
        else if (type == "DirectionalLight") handleDirectionalLight(params);
        else if (type == "PerspectiveCamera") handlePerspectiveCamera(params);
        else if (type == "Sphere") handleSphere(params);
        else if (type == "Cube" || type == "Box") handleCube(params);
        else if (type == "Cone") handleCone(params);
        else if (type == "Cylinder") handleCylinder(params);
        else if (type == "IndexedFaceSet") handleIndexedFaceSet(params);
        else if (type == "Coordinate3") handleCoordinate3(params);
        else if (type == "Normal") handleNormal(params);
        else if (type == "Translation") handleTranslation(params);
        else if (type == "Rotation") handleRotation(params);
        else if (type == "Scale") handleScale(params);
        else if (type == "Transform") handleTransform(params);
        else if (type == "Material") handleMaterial(params);
        else {
            cout << "VRML applyProperties, node " << type << " not handled!" << endl;
        }
    }

    void buildOSG() {
        if (!obj) {
            obj = makeObject();
            if (parent && parent->obj) parent->obj->addChild(obj);
            else cout << "WARNING in VRMLNode::buildOSG, cannot append object to parent!" << endl;
            applyProperties();
        }

        for (auto c : children) c->buildOSG();
    }

    virtual Matrix4d applyTransformations(Matrix4d m = Matrix4d()) = 0;
    virtual VRMaterialPtr applyMaterials(VRMaterialPtr m = 0) = 0;
    virtual VRGeoData applyGeometries(VRGeoData data = VRGeoData()) = 0;
};

struct VRML1Node : VRMLNode {
    VRML1Node(string type, string name = "Unnamed") : VRMLNode(type, name) { version = 1; }
    ~VRML1Node() {}

    VRML1Node* addChild(string t, string n) {
        cout << "VRML1Node::addChild '" << n << "' of type " << t << endl;
        auto c = new VRML1Node(t,n);
        c->parent = this;
        children.push_back(c);
        return c;
    }

    Matrix4d applyTransformations(Matrix4d m = Matrix4d()) {
        if (isGeometryNode(type)) {
            VRTransformPtr t = dynamic_pointer_cast<VRTransform>(obj);
            if (t) t->setMatrix(m);
        } else if(isTransformationNode(type)) {
            m.mult(pose);
        } else if(isTransformationResetNode(type)) {
            m.setIdentity();
        }

        for (auto c : children) {
            m = c->applyTransformations(m);
        }

        return m;
    }

    VRMaterialPtr applyMaterials(VRMaterialPtr m = 0) {
        if (isGeometryNode(type)) {
            VRGeometryPtr g = dynamic_pointer_cast<VRGeometry>(obj);
            if (g) g->setMaterial(m);
        } else if(material) {
            m = material;
        } else if(type == "Separator") {
            m = 0;
        }

        for (auto c : children) {
            m = c->applyMaterials(m);
        }

        return m;
    }

    VRGeoData applyGeometries(VRGeoData data = VRGeoData()) {
        if (type == "IndexedFaceSet") {
            VRGeometryPtr g = dynamic_pointer_cast<VRGeometry>(obj);
            if (g) {
                vector<int> face;
                cout << "handleIndexedFaceSet " << coordIndex.size() << endl;
                for (auto i : coordIndex) {
                    if (i == -1) {
                        cout << " face " << face.size() << endl;
                        if (face.size() == 3) data.pushTri(face[0], face[1], face[2]);
                        if (face.size() == 4) data.pushQuad(face[0], face[1], face[2], face[3]);
                        face.clear();
                        continue;
                    }
                    face.push_back(i);
                }
                data.apply(g);
            }
        } else if(type == "Coordinate3") {
            for (auto p : positions) data.pushVert(Pnt3d(p));
        } else if(type == "Normal") {
            for (auto n : normals) data.pushNorm(n);
        } else if(type == "Separator") {
            data = VRGeoData();
        }

        for (auto c : children) {
            data = c->applyGeometries(data);
        }

        return data;
    }
};

struct VRML2Node : VRMLNode {
    VRML2Node(string type, string name = "Unnamed") : VRMLNode(type, name) { version = 2; }
    ~VRML2Node() {}

    VRML2Node* addChild(string t, string n) {
        cout << "VRML2Node::addChild '" << n << "' of type " << t << endl;
        auto c = new VRML2Node(t,n);
        c->parent = this;
        children.push_back(c);
        return c;
    }

    Matrix4d applyTransformations(Matrix4d m = Matrix4d()) {
        return m;
    }

    VRMaterialPtr applyMaterials(VRMaterialPtr m = 0) {
        return m;
    }

    VRGeoData applyGeometries(VRGeoData data = VRGeoData()) {
        return data;
    }
};

class VRMLLoader : public VRMLUtils {
    private:
        string path;
        VRTransformPtr res;
        VRProgressPtr progress;
        bool threaded = false;
        VRMLNode* tree = 0;

        enum STATE {
            HEAD,
            BODY,
            FIELD
        };

        struct Context {
            STATE state = HEAD;
            string field;
            VRMLNode* currentNode = 0;

            // node header
            bool nextNodeDEF = false;
            string nextNodeType = "Untyped";
            string nextNodeName = "Unnamed";
        };

        Context ctx;

        bool openFile(ifstream& file) {
            file.open(path);
            if (!file.is_open()) { cout << "ERROR: file '" << path << "' not found!" << endl; return false; }
            return true;
        }

        string getVersion(ifstream& file) {
            file.seekg(0, ios_base::beg);
            string line;
            getline(file, line);
            file.seekg(0, ios_base::beg);
            return line;
        }

        size_t getFileSize(ifstream& file) {
            file.seekg(0, ios_base::end);
            size_t fileSize = file.tellg();
            file.seekg(0, ios_base::beg);
            if (progress == 0) progress = VRProgress::create();
            progress->setup("load VRML " + path, fileSize);
            progress->reset();
            return fileSize;
        }

        Matrix4d handleMatrixTransform(map<string, string> data) {
            if (!data.count("matrix")) return Matrix4d();
            vector<float> v = toValue<vector<float>>( data["matrix"] );
            return Matrix4d(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]);
        }

        void handleBracket(string bracket) {
            auto open = [&]() {
                if (ctx.currentNode) {
                    ctx.currentNode = ctx.currentNode->addChild(ctx.nextNodeType, ctx.nextNodeName);
                    ctx.nextNodeType = "Untyped";
                    ctx.nextNodeName = "Unnamed";

                    if (isGroupNode(ctx.currentNode->type)) ctx.state = HEAD;
                    else                                    ctx.state = BODY;
                } else cout << "WARNING in VRML handleBracket: currentNode at opening bracket is NULL" << endl;
            };

            auto close = [&]() {
                //applyProperties();
                if (ctx.currentNode->parent) {
                    ctx.currentNode = ctx.currentNode->parent;
                } else cout << "WARNING in VRML handleBracket: currentNode at closing bracket is NULL" << endl;
            };

            if (ctx.currentNode) {
                if (version == 1 || version == 2) {
                    if (bracket == "}") close();
                    if (bracket == "{") open();
                    if (bracket == "{}") {
                        open();
                        close();
                    }
                }

                if (version == 2) { // for children node
                    if (bracket == "]") close();
                    if (bracket == "[") open();
                    if (bracket == "[]") {
                        open();
                        close();
                    }
                }
            }
        };

        string stateToString(STATE s) {
            if (s == HEAD) return "HEAD";
            if (s == BODY) return "BODY";
            if (s == FIELD) return "FIELD";
        }

        void handleToken(string token) {
            cout << " handle VRML token: " << token << endl;
            if (isBracket(token)) { handleBracket(token); return; }
            if (isNode(token)) {
                cout << "  handle VRML node: " << token << ", set as nextNodeType!" << endl;
                ctx.nextNodeType = token;
                return;
            }

            cout << "  VRML context state: " << stateToString(ctx.state) << endl;
            if (ctx.state == HEAD) {
                if (token == "DEF") ctx.nextNodeDEF = true;
                else {
                    cout << "  set '" << token << "' as nextNodeName!" << endl;
                    ctx.nextNodeName = token;
                }
                return;
            } // must be a name
            if (ctx.state == FIELD) {
                cout << "   " << token << " in field " << isBool(token) << " '" << ctx.currentNode->params[ctx.field] << "' field: " << ctx.field << endl;
                if (token == "[" || token == "]" || token == ",") return;
                if (isNumber(token) || isBool(token)) { ctx.currentNode->params[ctx.field] += token+" "; return; }
                ctx.state = BODY; // don't return here
            }
            if (ctx.state == BODY) {
                ctx.currentNode->params[token] = ""; // must be a field
                ctx.field = token;
                ctx.state = FIELD;
                return;
            }

            cout << "WARNING: VRML token not handled: " << token << endl;
        };

        bool parseFile(ifstream& file) {
            // coordinate system: OpenGL
            string line;
            while ( getline(file, line) ) {
                if (line.size() == 0) continue; // ignore empty lines
                if (line[0] == '#') continue; // ignore comments
                stringstream ss(line);
                while(!ss.eof()) {
                    string s; ss >> s;
                    if (s.size() == 0) continue; // ignore empty tokens
                    if (s[0] == '#') break; // ignore comments
                    handleToken(s);
                }
            }
            return true;
        }

    public:
        VRMLLoader(string p, VRTransformPtr r, VRProgressPtr pr, bool t) : path(p), res(r), progress(pr), threaded(t) {}

        void load() {
            ifstream file;
            if (!openFile(file)) return;
            getFileSize(file);
            string versionStr = getVersion(file);
            if (versionStr == "#VRML V1.0 ascii") version = 1;
            else version = 2;

            ctx = Context();
            if (version == 1) tree = new VRML1Node("Root", "Root");
            if (version == 2) tree = new VRML2Node("Root", "Root");
            tree->obj = res;
            ctx.currentNode = tree;
            parseFile(file);
            tree->print();
            tree->buildOSG();
            tree->applyTransformations();
            tree->applyMaterials();
            tree->applyGeometries();
            delete tree;
        }
};

void OSG::loadVRML(string path, VRTransformPtr res, VRProgressPtr p, bool thread) {
    VRMLLoader vrml(path, res, p, thread);
    vrml.load();
}