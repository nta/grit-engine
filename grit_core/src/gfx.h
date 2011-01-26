/* Copyright (c) David Cunningham and the Grit Game Engine project 2010
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

#if 0
some way of distinguishing sky from everything else

manobj
clutter
rclutter
psys

mat
tex
mesh
skel
gpuprog
rtex
rwin


Script or user customisable graphical elements within a typical grit game:

Clutter
Lights (high level lights with coronas etc)
Particles
Material definitions
Shaders?

no need to touch meshes/textures except to reload -- probably will have reload_mesh(string) or similar

#endif

#include "SharedPtr.h"

struct GfxCallback;
class GfxBody;
typedef SharedPtr<GfxBody> GfxBodyPtr;
class GfxLight;
typedef SharedPtr<GfxLight> GfxLightPtr;
class GfxMaterial;
struct GfxLastRenderStats;
struct GfxLastFrameStats;
struct GfxRunningFrameStats;
struct GfxPaintColour;


#ifndef gfx_h
#define gfx_h

#include <string>

#include "HUD.h"
#include "math_util.h"

struct GfxCallback {
    virtual ~GfxCallback (void) { }
    virtual void clickedClose (void) = 0;
    virtual void windowResized (int width, int height) = 0;
    virtual void messageLogged (const std::string &msg) = 0;
};

size_t gfx_init (GfxCallback &cb);

void gfx_render (float elapsed, const Vector3 &cam_pos, const Quaternion &cam_dir);
enum GfxBoolOption {
    GFX_AUTOUPDATE,
    GFX_SHADOW_RECEIVE,
    GFX_SHADOW_CAST,
    GFX_VSYNC,
    GFX_FULLSCREEN,
    GFX_FOG,
    GFX_DEFERRED,
    GFX_WIREFRAME,
    GFX_ANAGLYPH,
    GFX_CROSS_EYE,
    GFX_SHADOW_SIMPLE_OPTIMAL_ADJUST,
    GFX_SHADOW_AGGRESSIVE_FOCUS_REGION,
    GFX_SKY
};

enum GfxIntOption {
    GFX_FULLSCREEN_WIDTH,
    GFX_FULLSCREEN_HEIGHT,
    GFX_SHADOW_RES,
    GFX_RAM
};

enum GfxFloatOption {
    GFX_FOV,
    GFX_NEAR_CLIP,
    GFX_FAR_CLIP,
    GFX_EYE_SEPARATION, // FOR 'real' 3d
    GFX_MONITOR_HEIGHT, // FOR 'real' 3d
    GFX_MONITOR_EYE_DISTANCE, // FOR 'real' 3d
    GFX_MIN_PERCEIVED_DEPTH, // FOR 'real' 3d -- distance from eyes of the front clip plane
    GFX_MAX_PERCEIVED_DEPTH, // FOR 'real' 3d -- distance from eyes of the rear clip plane
    GFX_SHADOW_START,
    GFX_SHADOW_END0,
    GFX_SHADOW_END1,
    GFX_SHADOW_END2,
    GFX_SHADOW_OPTIMAL_ADJUST0,
    GFX_SHADOW_OPTIMAL_ADJUST1,
    GFX_SHADOW_OPTIMAL_ADJUST2,
    GFX_SHADOW_LIGHT_DIRECTION_THRESHOLD,
    GFX_SHADOW_PADDING,
    GFX_ANAGLYPH_LEFT_RED_MASK,
    GFX_ANAGLYPH_LEFT_GREEN_MASK,
    GFX_ANAGLYPH_LEFT_BLUE_MASK,
    GFX_ANAGLYPH_RIGHT_RED_MASK,
    GFX_ANAGLYPH_RIGHT_GREEN_MASK,
    GFX_ANAGLYPH_RIGHT_BLUE_MASK,
    GFX_ANAGLYPH_DESATURATION
};

std::string gfx_option_to_string (GfxBoolOption o);
std::string gfx_option_to_string (GfxIntOption o);
std::string gfx_option_to_string (GfxFloatOption o);

// set's t to either 0,1,2 and fills in the approriate argument
void gfx_option_from_string (const std::string &s,
                             int &t,
                             GfxBoolOption &o0,
                             GfxIntOption &o1,
                             GfxFloatOption &o2);

void gfx_option (GfxBoolOption o, bool v);
bool gfx_option (GfxBoolOption o);
void gfx_option (GfxIntOption o, int v);
int gfx_option (GfxIntOption o);
void gfx_option (GfxFloatOption o, float v);
float gfx_option (GfxFloatOption o);

typedef SharedPtr<GfxMaterial> GfxMaterialPtr;
typedef std::vector<GfxMaterialPtr> GfxMaterialPtrs;
class GfxMaterial {
    public:
    Ogre::MaterialPtr regularMat;     // no suffix
    Ogre::MaterialPtr fadingMat;      // ' can be NULL
    //Ogre::MaterialPtr shadowMat;      // ! can be simply a link to the default
    //Ogre::MaterialPtr worldMat;       // & 
    //Ogre::MaterialPtr worldShadowMat; // % can be simply a link to the default

    protected:
    float alpha;
    bool alphaBlend;

    GfxMaterial (const std::string &name);

    bool delayedDeferred (void) { return alphaBlend; }

    public:

    float getAlpha (void) { return alpha; }
    void setAlpha (float v);
    bool getAlphaBlend (void) { return alphaBlend; }
    void setAlphaBlend (bool v);
    const std::string name;

    friend GfxMaterialPtr gfx_material_add(const std::string &);
    friend class GfxBody;
};

GfxMaterialPtr gfx_material_add (const std::string &name);

GfxMaterialPtr gfx_material_add_or_get (const std::string &name);

GfxMaterialPtr gfx_material_get (const std::string &name);

bool gfx_material_has (const std::string &name);

struct GfxPaintColour {
    Vector3 diff, spec;
    float met; // metallic paint (0 -> 1)
};


// this should rarely need to be used by users of this API
class GfxNode {
    protected:
    static const std::string className;
    Vector3 pos; Quaternion quat; Vector3 scl;
    GfxBodyPtr par;
    public: // HACK
    Ogre::SceneNode *node;
    protected:
    bool dead;

    GfxNode (const GfxBodyPtr &par_);

    void notifyParentDead (void);
    void ensureNotChildOf (GfxBody *leaf) const;

    public:
    virtual ~GfxNode ();

    const GfxBodyPtr &getParent (void) const;
    virtual void setParent (const GfxBodyPtr &par_);

    Vector3 transformPosition (const Vector3 &v);
    Quaternion transformOrientation (const Quaternion &v);
    Vector3 transformScale (const Vector3 &v);

    const Vector3 &getLocalPosition (void);
    void setLocalPosition (const Vector3 &p);
    Vector3 getWorldPosition (void);

    const Quaternion &getLocalOrientation (void);
    void setLocalOrientation (const Quaternion &q);
    Quaternion getWorldOrientation (void);

    const Vector3 &getLocalScale (void);
    void setLocalScale (const Vector3 &s);
    Vector3 getWorldScale (void);

    virtual void destroy (void);

    friend class GfxBody; // otherwise it cannot access our protected stuff
};

class GfxBody : public GfxNode {
    protected:
    static const std::string className;
    std::vector<GfxNode*> children; // caution!
    public: // HACK
    Ogre::Entity *ent;
    protected:
    float fade;
    GfxMaterialPtrs materials;
    GfxPaintColour colours[4];

    GfxBody (const std::string &mesh_name, const GfxBodyPtr &par_);
    GfxBody (const GfxBodyPtr &par_);

    public:
    static GfxBodyPtr make (const std::string &mesh_name, const GfxBodyPtr &par_=GfxBodyPtr(NULL))
    { return GfxBodyPtr(new GfxBody(mesh_name, par_)); }

    static GfxBodyPtr make (const GfxBodyPtr &par_=GfxBodyPtr(NULL))
    { return GfxBodyPtr(new GfxBody(par_)); }
    
    ~GfxBody ();

    void notifyLostChild (GfxNode *child);
    void notifyGainedChild (GfxNode *child);
    void setParent (const GfxBodyPtr &par_);

    unsigned getBatches (void) const;
    unsigned getBatchesWithChildren (void) const;

    unsigned getTriangles (void) const;
    unsigned getTrianglesWithChildren (void) const;

    float getFade (void);
    void setFade (float f, int transition);

    bool getCastShadows (void);
    void setCastShadows (bool v);

    GfxPaintColour getPaintColour (int i);
    void setPaintColour (int i, const GfxPaintColour &c);

    unsigned getNumBones (void);
    unsigned getBoneId (const std::string name);
    const std::string &getBoneName (unsigned n);

    bool getBoneManuallyControlled (unsigned n);
    void setBoneManuallyControlled (unsigned n, bool v);
    void setAllBonesManuallyControlled (bool v);

    Vector3 getBoneInitialPosition (unsigned n);
    Vector3 getBoneWorldPosition (unsigned n);
    Vector3 getBoneLocalPosition (unsigned n);
    Quaternion getBoneInitialOrientation (unsigned n);
    Quaternion getBoneWorldOrientation (unsigned n);
    Quaternion getBoneLocalOrientation (unsigned n);

    void setBoneLocalPosition (unsigned n, const Vector3 &v);
    void setBoneLocalOrientation (unsigned n, const Quaternion &v);
    
    void destroy (void);
};

class GfxLight : public GfxNode {
    protected:
    static const std::string className;
    public: // HACK
    Ogre::Light *light;
    protected:

    GfxLight (const GfxBodyPtr &par_);


    public:
    static GfxLightPtr make (const GfxBodyPtr &par_=GfxBodyPtr(NULL))
    { return GfxLightPtr(new GfxLight(par_)); }
    
    ~GfxLight ();

    Vector3 getDiffuseColour (void);
    Vector3 getSpecularColour (void);
    void setDiffuseColour (const Vector3 &v);
    void setSpecularColour (const Vector3 &v);
    Vector3 getAim (void);
    void setAim (const Vector3 &v);
    float getRange (void);
    void setRange (float v);
    Degree getInnerAngle (void);
    void setInnerAngle (Degree v);
    Degree getOuterAngle (void);
    void setOuterAngle (Degree v);

    bool isEnabled (void);
    void setEnabled (bool v);

    void destroy (void);
};
    

Vector3 gfx_sun_get_diffuse (void);
void gfx_sun_set_diffuse (const Vector3 &v);
Vector3 gfx_sun_get_specular (void);
void gfx_sun_set_specular (const Vector3 &v);
Vector3 gfx_sun_get_direction (void);
void gfx_sun_set_direction (const Vector3 &v);

Vector3 gfx_get_scene_ambient (void);
void gfx_set_scene_ambient (const Vector3 &v);

Vector3 gfx_fog_get_colour (void);
void gfx_fog_set_colour (const Vector3 &v);
float gfx_fog_get_density (void);
void gfx_fog_set_density (float v);

Quaternion gfx_get_celestial_orientation (void);
void gfx_set_celestial_orientation (const Quaternion &v);

void gfx_screenshot (const std::string &filename);

struct GfxLastRenderStats {
    float batches;
    float triangles;
    float micros;
    GfxLastRenderStats (const GfxLastRenderStats &o)
        : batches(o.batches), triangles(o.triangles), micros(o.micros) { }
    GfxLastRenderStats (void) : batches(0), triangles(0), micros(0) { }
    GfxLastRenderStats &operator+= (const GfxLastRenderStats &o) {
        batches += o.batches; triangles += o.triangles; micros += o.micros;
        return *this;
    }
    GfxLastRenderStats &operator/= (float time) {
        batches /= time; triangles /= time; micros /= time;
        return *this;
    }
};

struct GfxLastFrameStats {
    GfxLastRenderStats shadow[3];
    GfxLastRenderStats left;
    GfxLastRenderStats right;
};

struct GfxRunningFrameStats {
    GfxLastFrameStats min;
    GfxLastFrameStats avg;
    GfxLastFrameStats max;
};

GfxLastFrameStats gfx_last_frame_stats (void);
GfxRunningFrameStats gfx_running_frame_stats (void);

void gfx_reload_resources (void);

HUD::RootPtr gfx_init_hud (void);

void gfx_shutdown (void);

// FIXME: everything below here is a horrible hack
#include <OgreRoot.h>
#include <OgreOctreeSceneManager.h>
#include <OgreRenderWindow.h>
extern Ogre::Root *ogre_root; // FIXME: hack
extern Ogre::OctreeSceneManager *ogre_sm; // FIXME: hack
extern Ogre::RenderWindow *ogre_win; // FIXME: hack


static inline Ogre::Vector3 to_ogre (const Vector3 &v)
{ return Ogre::Vector3(v.x,v.y,v.z); }
static inline Ogre::Quaternion to_ogre (const Quaternion &v)
{ return Ogre::Quaternion(v.w,v.x,v.y,v.z); }
static inline Ogre::ColourValue to_ogre_cv (const Vector3 &v)
{ return Ogre::ColourValue(v.x,v.y,v.z); }
static inline Ogre::Degree to_ogre (const Degree &v)
{ return Ogre::Degree(v.inDegrees()); }
static inline Ogre::Radian to_ogre (const Radian &v)
{ return Ogre::Degree(v.inRadians()); }

static inline Vector3 from_ogre (const Ogre::Vector3 &v)
{ return Vector3(v.x,v.y,v.z); }
static inline Quaternion from_ogre (const Ogre::Quaternion &v)
{ return Quaternion(v.w,v.x,v.y,v.z); }
static inline Vector3 from_ogre_cv (const Ogre::ColourValue &v)
{ return Vector3(v.r, v.g, v.b); }
static inline Degree from_ogre (const Ogre::Degree &v)
{ return Degree(v.valueDegrees()); }
static inline Radian from_ogre (const Ogre::Radian &v)
{ return Degree(v.valueRadians()); }

#endif 