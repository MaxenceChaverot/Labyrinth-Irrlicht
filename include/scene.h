// scene.h

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <iostream>

#include <irrlicht.h>
#include "events.h"
#include "skeleton.h"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

namespace ig = irr::gui;

class myScene
{
    /** ATTRIBUTS **/

    is::ISceneManager   *smgr;
    EventReceiver receiver;
    iv::IVideoDriver    *driver ;
    irr::IrrlichtDevice *device;

    ig::IGUIEnvironment *gui;

    // Score
    //int score;
    std::vector<iv::ITexture*> numbers;
    std::vector<ig::IGUIImage*> score_images;

    is::IAnimatedMesh *mesh_scene;
    is::IMeshSceneNode *node_scene;

    is::ICameraSceneNode* camera;
    is::IAnimatedMeshSceneNode *node_gun;
    is::IMeshSceneNode *node_sphere;

    skeleton sk1;

    // Timer pour mourir
    int time;
    bool start_timer;
    bool is_ennemi_dead;

public:
    /** METHODS **/

    myScene();

    void scene_init();
    void scene_run();


    void display_map();

     /** Initialise le score **/
    void init_score();

    /** Charge la camera et le gun **/
    void load_camera_gun();

     /** Charge les ennemis **/
    void load_enemies();

    /** Fonction de tire **/
    void gun_shoot();

     /** Fonction de check timer **/
    void check_timer();

    void create_window();

};

#endif
