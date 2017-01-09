#include "skeleton.h"
#include <iostream>

skeleton::skeleton(){

}

skeleton::skeleton(IrrlichtDevice *_device, ic::vector3df position, ic::vector3df rotation, int _id):device(_device),id(_id),isRunning(false), isDead(false){

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

bool skeleton::hasPlayerInSight(is::ISceneNode *node_sphere){

    is::ISceneCollisionManager *collision_manager = smgr->getSceneCollisionManager();
    node_skeleton->updateAbsolutePosition();
    node_sphere->updateAbsolutePosition();
    ic::line3d<f32> ray(node_skeleton->getAbsolutePosition(),node_sphere->getAbsolutePosition());
    node_skeleton->setRotation(core::vector3df(0.0f,(node_sphere->getAbsolutePosition().Y - node_skeleton->getAbsolutePosition().Y)*180/3.14,0.0f));
    /*    std::cout<<node_skeleton->getAbsolutePosition().X<<" , ";
    std::cout<<node_skeleton->getAbsolutePosition().Y<<" , ";
    std::cout<<node_skeleton->getAbsolutePosition().Z<<" : ";
    std::cout<<node_sphere->getAbsolutePosition().X<<" , ";
    std::cout<<node_sphere->getAbsolutePosition().Y<<" , ";
    std::cout<<node_sphere->getAbsolutePosition().Z<<std::endl;*/

    ic::vector3df intersection;
    ic::triangle3df hit_triangle;

    is::ISceneNode *selected_scene_node = collision_manager->getSceneNodeAndCollisionPointFromRay(ray,intersection,hit_triangle,node_sphere->getID());


    if (!selected_scene_node)
    {

        if(isRunning==false && isDead == false){
            std::cout<<"J'anniùme"<<std::endl;
        is::ISceneNodeAnimator *anim = smgr->createFlyStraightAnimator(node_skeleton->getAbsolutePosition(),core::vector3df(node_sphere->getAbsolutePosition().X,node_skeleton->getAbsolutePosition().Y,node_sphere->getAbsolutePosition().Z),1000);
        node_skeleton->addAnimator(anim);
        node_skeleton->setLoopMode(true);
        node_skeleton->setMD2Animation(is::EMAT_RUN);

        isRunning = true;

        }
        else if(isDead){
           // stopAnimation();
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

