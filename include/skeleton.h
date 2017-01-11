#ifndef SKELETON_H
#define SKELETON_H


#include <irrlicht.h>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;




class skeleton {
        IrrlichtDevice *device;
        is::ISceneManager   *smgr;
        iv::IVideoDriver    *driver ;
        is::IAnimatedMeshSceneNode *node_skeleton;
        int id;
        bool isRunning;


        // Timer pour mourir
        int time;
        bool timer_death;
        bool is_dead;

public:
        skeleton();
        skeleton(IrrlichtDevice *_device,ic::vector3df position, ic::vector3df rotation, int _id);
        is::IAnimatedMeshSceneNode* getNode();

        bool hasPlayerInSight(is::ISceneNode *node_gun);

        void stopAnimation();
        bool isDead();

        void start_dead_animation();

        /** Fonction de check timer pour l'animation de mort du zombie **/
        void check_death_timer();

        //Code fourni par Sylvain MIGUELEZ && David BAIO
        void faceModelToPlayer();


};



#endif
