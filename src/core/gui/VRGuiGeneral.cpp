
#include "VRGuiGeneral.h"
#include "VRGuiUtils.h"
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/textview.h>
#include <gtkmm/combobox.h>
#include <gtkmm/cellrenderercombo.h>

#include <OpenSG/OSGSceneFileHandler.h>
#include "core/scene/VRScene.h"
#include "core/setup/VRSetup.h"
#include "core/setup/windows/VRView.h"
#include "core/scene/rendering/VRRenderStudio.h"
#include "core/objects/OSGObject.h"
#include "core/objects/material/VRMaterial.h"


using namespace OSG;

// --------------------------
// ---------Main-------------
// --------------------------

VRGuiGeneral::VRGuiGeneral() {
    setCheckButtonCallback("radiobutton5", sigc::mem_fun(*this, &VRGuiGeneral::setMode) );
    setCheckButtonCallback("radiobutton18", sigc::mem_fun(*this, &VRGuiGeneral::setMode) );
    setCheckButtonCallback("radiobutton4", sigc::mem_fun(*this, &VRGuiGeneral::setMode) );
    setCheckButtonCallback("checkbutton_01", sigc::mem_fun(*this, &VRGuiGeneral::toggleFrustumCulling) );
    setCheckButtonCallback("checkbutton_02", sigc::mem_fun(*this, &VRGuiGeneral::toggleOcclusionCulling) );
    setCheckButtonCallback("checkbutton_2", sigc::mem_fun(*this, &VRGuiGeneral::toggleTwoSided) );
    setCheckButtonCallback("checkbutton_3", sigc::mem_fun(*this, &VRGuiGeneral::toggleDeferredShader) );
    setCheckButtonCallback("checkbutton_4", sigc::mem_fun(*this, &VRGuiGeneral::toggleSSAO) );
    setCheckButtonCallback("checkbutton_5", sigc::mem_fun(*this, &VRGuiGeneral::toggleCalib) );
    setCheckButtonCallback("checkbutton_6", sigc::mem_fun(*this, &VRGuiGeneral::toggleHMDD) );
    setCheckButtonCallback("checkbutton_7", sigc::mem_fun(*this, &VRGuiGeneral::toggleMarker) );
    setCheckButtonCallback("checkbutton_8", sigc::mem_fun(*this, &VRGuiGeneral::toggleFXAA) );
    setSliderCallback("hscale1", sigc::mem_fun(*this, &VRGuiGeneral::setSSAOradius) );
    setSliderCallback("hscale2", sigc::mem_fun(*this, &VRGuiGeneral::setSSAOkernel) );
    setSliderCallback("hscale3", sigc::mem_fun(*this, &VRGuiGeneral::setSSAOnoise) );
    setColorChooser("bg_solid", sigc::mem_fun(*this, &VRGuiGeneral::setColor) );
    setEntryCallback("entry42", sigc::mem_fun(*this, &VRGuiGeneral::setPath));
    setEntryCallback("entry14", sigc::mem_fun(*this, &VRGuiGeneral::setExtension));
    setButtonCallback("button22", sigc::mem_fun(*this, &VRGuiGeneral::dumpOSG));
    setRadioButtonCallback("radiobutton13", sigc::mem_fun(*this, &VRGuiGeneral::toggleDRendChannel));
    setRadioButtonCallback("radiobutton14", sigc::mem_fun(*this, &VRGuiGeneral::toggleDRendChannel));
    setRadioButtonCallback("radiobutton15", sigc::mem_fun(*this, &VRGuiGeneral::toggleDRendChannel));
    setRadioButtonCallback("radiobutton16", sigc::mem_fun(*this, &VRGuiGeneral::toggleDRendChannel));
}

bool VRGuiGeneral::setSSAOradius( int st, double d ) {
    if (updating) return false;
    auto scene = VRScene::getCurrent();
    if (scene) scene->setSSAOradius( getSliderState("hscale1") );
    return true;
}

bool VRGuiGeneral::setSSAOkernel( int st, double d ) {
    if (updating) return false;
    auto scene = VRScene::getCurrent();
    if (scene) scene->setSSAOkernel( getSliderState("hscale2") );
    return true;
}

bool VRGuiGeneral::setSSAOnoise( int st, double d ) {
    if (updating) return false;
    auto scene = VRScene::getCurrent();
    if (scene) scene->setSSAOnoise( getSliderState("hscale3") );
    return true;
}

void VRGuiGeneral::dumpOSG() {
    auto scene = VRScene::getCurrent();
    if (scene == 0) return;

    string pg = scene->getName() + "_osg_dump.osg";
    string pb = scene->getName() + "_osg_dump.osb";
    cout << "dump OSG " << pg << endl;

    SceneFileHandler::the()->write( scene->getRoot()->getNode()->node, pg.c_str() );
    SceneFileHandler::the()->write( scene->getRoot()->getNode()->node, pb.c_str() );

    auto setup = VRSetup::getCurrent();
    for (auto v : setup->getViews()) {
        auto rL = v->getRenderingL();
        auto rR = v->getRenderingR();
        if (rL) { cout << "\nrLEFT " << endl; VRObject::printOSGTree( rL->getRoot()->getNode() ); }
        if (rR) { cout << "\nrRIGHT " << endl; VRObject::printOSGTree( rR->getRoot()->getNode() ); }
    }
}

bool VRGuiGeneral::setColor(GdkEventButton* b) {
    if (updating) return true;

    auto scene = VRScene::getCurrent();
    Color3f col = scene->getBackgroundColor();
    Color4f c = chooseColor("bg_solid", toColor4f(col));
    scene->setBackgroundColor(toColor3f(c));
    return true;
}

void VRGuiGeneral::setPath() {
    if (updating) return;
    auto scene = VRScene::getCurrent();
    if (scene == 0) return;
    scene->setBackgroundPath( getTextEntry("entry42") );
}

void VRGuiGeneral::setExtension() {
    if (updating) return;
    auto scene = VRScene::getCurrent();
    if (scene == 0) return;
    scene->setSkyBGExtension( getTextEntry("entry14") );
}

void VRGuiGeneral::setMode() {
    if (updating) return;

    VRBackground::TYPE t = VRBackground::SOLID;
    if ( getCheckButtonState("radiobutton4") ) t = VRBackground::IMAGE;
    if ( getCheckButtonState("radiobutton5") ) t = VRBackground::SKYBOX;
    if ( getCheckButtonState("radiobutton18") ) t = VRBackground::SKY;
    auto scene = VRScene::getCurrent();
    scene->setBackground( t );

    setEntrySensitivity("entry14", t == VRBackground::SKYBOX);
    setEntrySensitivity("entry42", t == VRBackground::SKYBOX || t == VRBackground::IMAGE);
}

void VRGuiGeneral::toggleDeferredShader() {
    if (updating) return;
    auto scene = VRScene::getCurrent();
    if (scene) scene->setDeferredShading( getCheckButtonState("checkbutton_3") );
}

void VRGuiGeneral::toggleDRendChannel() {
    if (updating) return;
    auto scene = VRScene::getCurrent();
    if (!scene) return;
    int channel = 0;
    if ( getRadioButtonState("radiobutton14") ) channel = 1;
    if ( getRadioButtonState("radiobutton15") ) channel = 2;
    if ( getRadioButtonState("radiobutton16") ) channel = 3;
    if ( getRadioButtonState("radiobutton17") ) channel = 4;
    scene->setDeferredChannel(channel);
}

void VRGuiGeneral::toggleSSAO() {
    if (updating) return;
    auto scene = VRScene::getCurrent();
    if (scene) scene->setSSAO( getCheckButtonState("checkbutton_4") );
}

void VRGuiGeneral::toggleHMDD() {
    if (updating) return;
    auto scene = VRScene::getCurrent();
    if (scene) scene->setHMDD( getCheckButtonState("checkbutton_6") );
}

void VRGuiGeneral::toggleFXAA() {
    if (updating) return;
    auto scene = VRScene::getCurrent();
    if (scene) scene->setFXAA( getCheckButtonState("checkbutton_8") );
}

void VRGuiGeneral::toggleCalib() {
    if (updating) return;
    auto scene = VRScene::getCurrent();
    if (scene) scene->setCalib( getCheckButtonState("checkbutton_5") );
}

void VRGuiGeneral::toggleMarker() {
    if (updating) return;
    auto scene = VRScene::getCurrent();
    if (scene) scene->setMarker( getCheckButtonState("checkbutton_7") );
}

void VRGuiGeneral::toggleFrustumCulling() {
    if (updating) return;
    auto scene = VRScene::getCurrent();
    if (scene) scene->setFrustumCulling( getCheckButtonState("checkbutton_01") );
}

void VRGuiGeneral::toggleOcclusionCulling() {
    if (updating) return;
    auto scene = VRScene::getCurrent();
    if (scene) scene->setOcclusionCulling( getCheckButtonState("checkbutton_02") );
}

void VRGuiGeneral::toggleTwoSided() {
    if (updating) return;
    auto scene = VRScene::getCurrent();
    if (scene) scene->setTwoSided( getCheckButtonState("checkbutton_2") );
}

void VRGuiGeneral::updateScene() {
    auto scene = VRScene::getCurrent();
    if (scene == 0) return;

    updating = true;

    // background
    Color3f col = scene->getBackgroundColor();
    VRBackground::TYPE t = scene->getBackgroundType();

    setColorChooserColor("bg_solid", Color3f(col[0], col[1], col[2]));
    setTextEntry("entry42", scene->getBackgroundPath());
    setEntrySensitivity("entry14", t == VRBackground::SKYBOX);
    if (t == VRBackground::SKYBOX) setTextEntry("entry14", scene->getSkyBGExtension());

    setCheckButton("radiobutton18", t == VRBackground::SKY);
    setCheckButton("radiobutton5", t == VRBackground::SKYBOX);
    setCheckButton("radiobutton4", t == VRBackground::IMAGE);

    // rendering
    setCheckButton("checkbutton_01", scene->getFrustumCulling() );
    setCheckButton("checkbutton_02", scene->getOcclusionCulling() );
    setCheckButton("checkbutton_2", scene->getTwoSided() );
    setCheckButton("checkbutton_3", scene->getDefferedShading() );
    setCheckButton("checkbutton_4", scene->getSSAO() );
    setCheckButton("checkbutton_6", scene->getHMDD() );
    setCheckButton("checkbutton_8", scene->getFXAA() );

    updating = false;
}


