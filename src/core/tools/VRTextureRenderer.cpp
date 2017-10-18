#include "VRTextureRenderer.h"
#include "core/objects/OSGObject.h"
#include "core/objects/object/OSGCore.h"
#include "core/objects/VRCamera.h"
#include "core/objects/OSGCamera.h"
#include "core/objects/material/VRMaterial.h"
#include "core/objects/material/VRTexture.h"
#include "core/objects/material/VRTextureGenerator.h"
#include "core/scene/VRScene.h"

#include <OpenSG/OSGBackground.h>
#include <OpenSG/OSGSimpleStage.h>
#include <OpenSG/OSGFrameBufferObject.h>
#include <OpenSG/OSGTextureBuffer.h>
#include <OpenSG/OSGRenderBuffer.h>
#include <OpenSG/OSGTextureObjChunk.h>
#include <OpenSG/OSGTextureEnvChunk.h>
#include <OpenSG/OSGTexGenChunk.h>
#include <OpenSG/OSGGeometry.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGSimpleTexturedMaterial.h>

#include <OpenSG/OSGPassiveWindow.h>
#include <OpenSG/OSGViewport.h>
#include <OpenSG/OSGRenderAction.h>
#include <OpenSG/OSGSolidBackground.h>

using namespace std;
using namespace OSG;

OSG_BEGIN_NAMESPACE;
struct VRTextureRenderer::Data {
    int fboWidth = 256;
    int fboHeight = 256;
    FrameBufferObjectRefPtr fbo;
    TextureObjChunkRefPtr   fboTex;
    ImageRefPtr             fboTexImg;
    TextureObjChunkRefPtr   fboDTex;
    ImageRefPtr             fboDTexImg;
    SimpleStageRefPtr stage;
};
OSG_END_NAMESPACE;

void VRTextureRenderer::test() {
    NodeRefPtr     flagScene = makeCoredNode<Group>();
    GeometryRefPtr flagGeo   = makePlaneGeo(4, 2, 1, 1);
    flagGeo->setDlistCache(false);

    SimpleMaterialRefPtr  flagMat   = SimpleMaterial ::create();
    TextureEnvChunkRefPtr fboTexEnv = TextureEnvChunk::create();

    fboTexEnv->setEnvMode(GL_REPLACE);
    flagMat->addChunk(data->fboTex);
    flagMat->addChunk(fboTexEnv);
    flagGeo->setMaterial(flagMat);
    flagScene->addChild(NodeRefPtr(makeNodeFor(flagGeo)));

    auto scene = VRScene::getCurrent();
    auto hlight = scene->getRoot()->find("Headlight");
    hlight->addChild( OSGObject::create(flagScene) );
}

VRTextureRenderer::VRTextureRenderer(string name) : VRObject(name) {
    data = new Data();

    data->fboTex = TextureObjChunk::create();
    data->fboTexImg = Image::create();
    data->fboTexImg->set(Image::OSG_RGB_PF, data->fboWidth, data->fboHeight);
    data->fboTex->setImage(data->fboTexImg);
    TextureBufferRefPtr texBuf = TextureBuffer::create();
    texBuf->setTexture(data->fboTex);

    data->fboDTexImg = Image::create();
    data->fboDTexImg->set(Image::OSG_RGB_PF, data->fboWidth, data->fboHeight);
    data->fboDTex = TextureObjChunk::create();
    data->fboDTex->setImage(data->fboDTexImg);
    data->fboDTex->setMinFilter(GL_NEAREST);
    data->fboDTex->setMagFilter(GL_NEAREST);
    data->fboDTex->setWrapS(GL_CLAMP_TO_EDGE);
    data->fboDTex->setWrapT(GL_CLAMP_TO_EDGE);
    data->fboDTex->setExternalFormat(GL_DEPTH_COMPONENT);
    data->fboDTex->setInternalFormat(GL_DEPTH_COMPONENT32);
    data->fboDTex->setCompareMode(GL_NONE);
    data->fboDTex->setCompareFunc(GL_LEQUAL);
    data->fboDTex->setDepthMode(GL_INTENSITY);
    TextureBufferRefPtr texDBuf = TextureBuffer::create();
    texDBuf->setTexture(data->fboDTex);

    RenderBufferRefPtr depthBuf = RenderBuffer::create();
    depthBuf->setInternalFormat(GL_DEPTH_COMPONENT24);

    data->fbo = FrameBufferObject::create();
    data->fbo->setColorAttachment(texBuf, 0);
    data->fbo->setDepthAttachment(texDBuf);
    data->fbo->editMFDrawBuffers()->push_back(GL_DEPTH_ATTACHMENT_EXT);
    data->fbo->editMFDrawBuffers()->push_back(GL_COLOR_ATTACHMENT0_EXT);
    data->fbo->setWidth (data->fboWidth );
    data->fbo->setHeight(data->fboHeight);
    data->fbo->setPostProcessOnDeactivate(true);
    texBuf->setReadBack(true);
    texDBuf->setReadBack(true);

    mat = VRMaterial::create("VRTextureRenderer");
    mat->setTexture(data->fboTex, 0);
    mat->setTexture(data->fboDTex, 1);
    //mat->setShaderParameter<int>("tex0", 0); // TODO: will fail because shader not yet defined..
    //mat->setShaderParameter<int>("tex1", 1);

    auto scene = VRScene::getCurrent();

    // Stage
    data->stage = SimpleStage::create();
    data->stage->setRenderTarget(data->fbo);
    data->stage->setSize(0.0f, 0.0f, 1.0f, 1.0f);
    data->stage->setBackground( scene->getBackground() );

    setCore(OSGCore::create(data->stage), "TextureRenderer");
}

VRTextureRenderer::~VRTextureRenderer() { delete data; }
VRTextureRendererPtr VRTextureRenderer::create(string name) { return VRTextureRendererPtr( new VRTextureRenderer(name) ); }

void VRTextureRenderer::setup(VRCameraPtr c, int width, int height, bool alpha) {
    cam = c;
    data->fboWidth = width;
    data->fboHeight = height;
    data->fbo->setWidth (data->fboWidth );
    data->fbo->setHeight(data->fboHeight);
    if (alpha) data->fboTexImg->set(Image::OSG_RGBA_PF, data->fboWidth, data->fboHeight);
    else data->fboTexImg->set(Image::OSG_RGB_PF, data->fboWidth, data->fboHeight);
    data->stage->setCamera(cam->getCam()->cam);
}

VRMaterialPtr VRTextureRenderer::getMaterial() { return mat; }
VRCameraPtr VRTextureRenderer::getCamera() { return cam; }

void VRTextureRenderer::setActive(bool b) {
    if (b) setCore(OSGCore::create(data->stage), "TextureRenderer", true);
    else setCore(OSGCore::create(Group::create()), "TextureRenderer", true);
}

VRTexturePtr VRTextureRenderer::renderOnce() {
    auto scene = VRScene::getCurrent();

    RenderActionRefPtr ract = RenderAction::create();
    PassiveWindowRecPtr win = PassiveWindow::create();
    ViewportRecPtr view = Viewport::create();
    SolidBackgroundRecPtr bg = SolidBackground::create();
    bg->setAlpha(0);
    mat->enableTransparency();
    data->stage->setBackground(bg);

    win->addPort(view);
    view->setRoot(getNode()->node);
    view->setCamera(cam->getCam()->cam);
    view->setBackground(scene->getBackground());
    win->render(ract);
    data->stage->setBackground(scene->getBackground());

    ImageRecPtr img = Image::create();
    img->set( data->fboTexImg );
    return VRTexture::create( img );
}





