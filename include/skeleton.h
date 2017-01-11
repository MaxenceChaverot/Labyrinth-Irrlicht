#ifndef SKELETON_H
#define SKELETON_H


#include <irrlicht.h>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;




class skeleton {
    /** ATTRIBUTS **/

    //système de rendu
    IrrlichtDevice *device;
    is::ISceneManager   *smgr;
    iv::IVideoDriver    *driver ;

    //Node du model
    is::IAnimatedMeshSceneNode *node_skeleton;
    int id;

    //Variable pour l'animation
    bool isRunning;


    // Timer pour mourir
    int time;
    bool timer_death;
    bool is_dead;

public:

    /** METHODS **/

    /** Constructeurs **/
    skeleton();
    skeleton(IrrlichtDevice *_device,ic::vector3df position, ic::vector3df rotation, int _id);
    is::IAnimatedMeshSceneNode* getNode();

    /** Orienter le model face au joueur **/
    //Code fourni par Sylvain MIGUELEZ && David BAIO
    void faceModelToPlayer();

    /** Le model a-t-il le joeur en vu **/
    bool hasPlayerInSight(is::ISceneNode *node_gun);

    /** Method Utilitaire pour l'animation **/
    void stopAnimation();
    void start_dead_animation();

    /** Getter pour la variable is_dead **/

    bool isDead();


    /** Fonction de check timer pour l'animation de mort du zombie **/
    void check_death_timer();




};



#endif
