/* Copyright (c) David Cunningham and the Grit Game Engine project 2012
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "gfx_internal.h"
#include "GfxLight.h"

static fast_erase_vector<GfxLight*> all_lights;

static bool have_init_coronas = false;

static void ensure_coronas_init (void)
{
    if (have_init_coronas) return;
    // this only happens once
    gfx_particle_define("/system/Coronas", "/system/Corona.bmp", GFX_PARTICLE_ADD, 0, true);
    have_init_coronas = true;
}

const std::string GfxLight::className = "GfxLight";

GfxLight::GfxLight (const GfxBodyPtr &par_)
  : GfxNode(par_),
    enabled(true),
    fade(1),
    coronaLocalPos(0,0,0),
    coronaSize(1),
    coronaColour(1,1,1),
    diffuse(1,1,1),
    specular(1,1,1),
    aim(1,0,0,0)
{
    light = ogre_sm->createLight();
    node->attachObject(light);
    light->setDirection(Ogre::Vector3(0,1,0));
    light->setAttenuation(10, 0, 0, 0);
    light->setSpotlightInnerAngle(Ogre::Degree(180));
    light->setSpotlightOuterAngle(Ogre::Degree(180));
    updateVisibility();
    all_lights.push_back(this);
    ensure_coronas_init();
    corona = gfx_particle_emit("/system/Coronas");
    corona.setDefaultUV();
    corona.setAlpha(1);
    corona.setAngle(0);
    updateCorona(Vector3(0,0,0));
}

GfxLight::~GfxLight (void)
{
    if (!dead) destroy();
}

void GfxLight::destroy (void)
{
    if (dead) THROW_DEAD(className);
    if (light) ogre_sm->destroyLight(light);
    light = NULL;
    all_lights.erase(this);
    corona.release();
    GfxNode::destroy();
}

void GfxLight::updateCorona (const Vector3 &cam_pos)
{
    if (dead) THROW_DEAD(className);
    coronaPos = transformPositionParent(coronaLocalPos);
    corona.setPosition(coronaPos);
    Vector3 col = enabled ? fade * coronaColour : Vector3(0,0,0);
    corona.setWidth(coronaSize);
    corona.setHeight(coronaSize);
    corona.setDepth(coronaSize);

    Vector3 light_dir_ws = (cam_pos - getWorldPosition()).normalisedCopy();
    Vector3 light_aim_ws_ = getWorldOrientation() * Vector3(0,1,0);

    float angle = light_aim_ws_.dot(light_dir_ws);
    float inner = Ogre::Math::Cos(light->getSpotlightInnerAngle());
    float outer = Ogre::Math::Cos(light->getSpotlightOuterAngle());
    if (outer != inner) {
            float occlusion = std::min(std::max((angle-inner)/(outer-inner), 0.0f), 1.0f);
            col *= (1-occlusion);
    }
    corona.setAmbient(col);
}

float GfxLight::getCoronaSize (void)
{
    if (dead) THROW_DEAD(className);
    return coronaSize;
}
void GfxLight::setCoronaSize (float v)
{
    if (dead) THROW_DEAD(className);
    coronaSize = v;
}

Vector3 GfxLight::getCoronaLocalPosition (void)
{
    if (dead) THROW_DEAD(className);
    return coronaLocalPos;
}

void GfxLight::setCoronaLocalPosition (const Vector3 &v)
{
    if (dead) THROW_DEAD(className);
    coronaLocalPos = v;
}

Vector3 GfxLight::getCoronaColour (void)
{
    if (dead) THROW_DEAD(className);
    return coronaColour;
}

void GfxLight::setCoronaColour (const Vector3 &v)
{
    if (dead) THROW_DEAD(className);
    coronaColour = v;
}

Vector3 GfxLight::getDiffuseColour (void)
{
    if (dead) THROW_DEAD(className);
    return diffuse;
}

Vector3 GfxLight::getSpecularColour (void)
{
    if (dead) THROW_DEAD(className);
    return specular;
}

void GfxLight::setDiffuseColour (const Vector3 &v)
{
    if (dead) THROW_DEAD(className);
    diffuse = v;
    updateVisibility();
}

void GfxLight::setSpecularColour (const Vector3 &v)
{
    if (dead) THROW_DEAD(className);
    specular = v;
    updateVisibility();
}

float GfxLight::getRange (void)
{
    if (dead) THROW_DEAD(className);
    return light->getAttenuationRange();
}

void GfxLight::setRange (float v)
{
    if (dead) THROW_DEAD(className);
    light->setAttenuation(v, 0, 0, 0);
}

Degree GfxLight::getInnerAngle (void)
{
    if (dead) THROW_DEAD(className);
    return from_ogre(light->getSpotlightInnerAngle());
}

void GfxLight::setInnerAngle (Degree v)
{
    if (dead) THROW_DEAD(className);
    light->setSpotlightInnerAngle(to_ogre(v));
}

Degree GfxLight::getOuterAngle (void)
{
    if (dead) THROW_DEAD(className);
    return from_ogre(light->getSpotlightOuterAngle());
}

void GfxLight::setOuterAngle (Degree v)
{
    if (dead) THROW_DEAD(className);
    light->setSpotlightOuterAngle(to_ogre(v));
}

bool GfxLight::isEnabled (void)
{
    if (dead) THROW_DEAD(className);
    return enabled;
}

void GfxLight::setEnabled (bool v)
{
    if (dead) THROW_DEAD(className);
    enabled = v;
    updateVisibility();
}

float GfxLight::getFade (void)
{
    if (dead) THROW_DEAD(className);
    return fade;
}
void GfxLight::setFade (float f)
{
    if (dead) THROW_DEAD(className);
    fade = f;
    updateVisibility();
}

void GfxLight::updateVisibility (void)
{
    light->setVisible(enabled && fade > 0.001);
    light->setDiffuseColour(to_ogre_cv(fade * diffuse));
    light->setSpecularColour(to_ogre_cv(fade * specular));
}
    
void update_coronas (const Vector3 &cam_pos)
{
    //const Ogre::LightList &ll = sm->_getLightsAffectingFrustum();
    for (unsigned long i=0 ; i<all_lights.size() ; ++i) {
        all_lights[i]->updateCorona(cam_pos);
    }

}
