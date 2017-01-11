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

    // Fenêtre et système de rendu
    is::ISceneManager   *smgr;
    EventReceiver receiver;
    iv::IVideoDriver    *driver ;
    irr::IrrlichtDevice *device;

    ig::IGUIEnvironment *gui;

    // Timer pour gérer le temps global de jeu
    std::vector<iv::ITexture*> numbers;
    std::vector<ig::IGUIImage*> timer_images;

    // Temps de jeu
    irr::ITimer* timer;
    int remaining_time;
    int tps;
    int time_minute;
    bool is_finished;

    bool is_needed_to_reset;

    // Elements du décor
    is::IAnimatedMesh *mesh_scene;
    is::IMeshSceneNode *node_scene;

    is::ICameraSceneNode* camera;
    is::IAnimatedMeshSceneNode *node_gun;
    //Sphere invisible entourant l'arme pour detection de colision
    is::IMeshSceneNode *node_sphere;

    //Les ennemis
    std::vector<skeleton> vec_ennemy;

    // Timer pour mourir
    int time;
    bool start_timer;
    bool is_ennemi_dead;

    //Position initiale des ennemis et fonction de remplissage
    std::vector<ic::vector3df> vec_ennemi_pos;
    std::vector<ic::vector3df> load_ennemi_pos();

public:
    /** METHODS **/

    myScene();

    /** Initialise la scene **/
    void scene_init();

    /** Déroulement du jeu **/
    void scene_run();

    /** Charge la camera et le gun **/
    void load_camera_gun();

    /** Charge les ennemis **/
    void load_enemies();

    /** Fonction de tir **/
    void gun_shoot();

    /** Fonction de check timer pour l'animation de mort du zombie **/
    void check_timer();

    /** Fonctions pour gérer le temps global du jeu **/
    void init_timer(); // initialisation du timer
    bool check_if_minute_past(); // vérifie toutes les minutes qui passent
    bool check_global_time(); // vérification du timer
    void update_timer(); // mise à jour de l'affichage du timer

    /** Affichage de la carte **/
    void display_map();

    /** Affichage de boites de dialogues **/
    void game_over();
    void check_if_game_is_won();

};

#endif
