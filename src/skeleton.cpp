#include "skeleton.h"
#include <iostream>

skeleton::skeleton(){

}

skeleton::skeleton(IrrlichtDevice *_device, ic::vector3df position, ic::vector3df rotation, int _id):device(_device),id(_id),isRunning(false), is_dead(false), timer_death(false),time(0){

    smgr = device->getSceneManager();
    driver = device->getVideoDriver();
    // On charge le mesh (fichier .obj)
    is::IAnimatedMesh *mesh_skeleton = smgr->getMesh("data/Models/tris.md2");

    // On l'associe a un noeud de la scene
    node_skeleton = smgr->addAnimatedMeshSceneNode(mesh_skeleton);
    node_skeleton->setMaterialFlag(iv::EMF_LIGHTING, false);
    node_skeleton->setMD2Animation(is::EMAT_STAND);
    node_skeleton->setScale(core::vector3df(0.6f,0.6f,0.6f));
    node_skeleton->setPosition(position);
    node_skeleton->setRotation(rotation);


    // On specifie sa texture

    node_skeleton->setMaterialTexture(0, driver->getTexture("data/Textures/red_skeleton_texture.pcx"));

    // On rend le mesh insensible a la lumiere
    //node_perso->setMaterialFlag(irr::video::EMF_LIGHTING, false);

}

is::IAnimatedMeshSceneNode* skeleton::getNode(){
    return node_skeleton;
}

//Code fourni par Sylvain MIGUELEZ && David BAIO
void skeleton::faceModelToPlayer(){

    // Rotate node to face player
    is::ICameraSceneNode*  const cam=node_skeleton->getSceneManager()->getActiveCamera();
    core::vector3df rot (node_skeleton->getRotation().X,
                         atan2(cam->getPosition().X-node_skeleton->getPosition().X, cam->getPosition().Z-node_skeleton->getPosition().Z) / M_PI * 180, //conversion rad->degre
                         node_skeleton->getRotation().Z);
    rot.Y -= 90.0f;
    node_skeleton->setRotation(rot);
}

bool skeleton::hasPlayerInSight(is::ISceneNode *node_sphere){

    is::ISceneCollisionManager *collision_manager = smgr->getSceneCollisionManager();
    node_skeleton->updateAbsolutePosition();
    node_sphere->updateAbsolutePosition();
    ic::line3d<f32> ray(node_skeleton->getAbsolutePosition(),node_sphere->getAbsolutePosition());

    ic::vector3df intersection;
    ic::triangle3df hit_triangle;

    is::ISceneNode *selected_scene_node = collision_manager->getSceneNodeAndCollisionPointFromRay(ray,intersection,hit_triangle,node_sphere->getID());


    if (!selected_scene_node)
    {

        if(isRunning==false && is_dead == false){
            is::ISceneNodeAnimator *anim = smgr->createFlyStraightAnimator(node_skeleton->getAbsolutePosition(),core::vector3df(node_sphere->getAbsolutePosition().X,node_skeleton->getAbsolutePosition().Y,node_sphere->getAbsolutePosition().Z),1000);
            node_skeleton->addAnimator(anim);
            node_skeleton->setLoopMode(true);
            node_skeleton->setMD2Animation(is::EMAT_RUN);

            isRunning = true;

        }
        else if(is_dead){
            isRunning = false;
        }
        return true;


    }
    else{
        if(isRunning){
            stopAnimation();
            isRunning = false;
        }
    }

    return false;
}

void skeleton::stopAnimation(){
    node_skeleton->removeAnimators();
    node_skeleton->setLoopMode(false);
    node_skeleton->setMD2Animation(is::EMAT_STAND);
}

void skeleton::start_dead_animation(){
    is_dead=true;
    stopAnimation();
    node_skeleton->setLoopMode(false);
    node_skeleton->setMD2Animation(is::EMAT_DEATH_FALLBACK);
    timer_death = true;
}

void skeleton::check_death_timer()
{
    if(timer_death){
        if(time == 52)
        {
            node_skeleton->remove();
            timer_death = false;
        }
        else time++;
    }
}

bool skeleton::isDead(){ return is_dead;}
