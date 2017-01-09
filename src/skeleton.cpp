#include "skeleton.h"
#include <iostream>

skeleton::skeleton(){

}

skeleton::skeleton(IrrlichtDevice *_device, ic::vector3df position, ic::vector3df rotation, int _id):device(_device),id(_id){

    smgr = device->getSceneManager();
    driver = device->getVideoDriver();
    // On charge le mesh (fichier .obj)
    is::IAnimatedMesh *mesh_skeleton = smgr->getMesh("data/Models/tris.md2");

    // On l'associe a un noeud de la scene
    node_skeleton = smgr->addAnimatedMeshSceneNode(mesh_skeleton);
    node_skeleton->setMaterialFlag(iv::EMF_LIGHTING, false);
    node_skeleton->setMD2Animation(is::EMAT_STAND);
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

/*    std::cout<<node_skeleton->getAbsolutePosition().X<<" , ";
    std::cout<<node_skeleton->getAbsolutePosition().Y<<" , ";
    std::cout<<node_skeleton->getAbsolutePosition().Z<<" : ";
    std::cout<<node_sphere->getAbsolutePosition().X<<" , ";
    std::cout<<node_sphere->getAbsolutePosition().Y<<" , ";
    std::cout<<node_sphere->getAbsolutePosition().Z<<std::endl;*/

    ic::vector3df intersection;
    ic::triangle3df hit_triangle;

    is::ISceneNode *selected_scene_node = collision_manager->getSceneNodeAndCollisionPointFromRay(ray,intersection,hit_triangle,node_sphere->getID());

    std::cout<<"Test"<<std::endl;

    if (selected_scene_node)
    {
        std::cout<<"test1 :"<<selected_scene_node->getID()<<std::endl;
            if(node_sphere == selected_scene_node)
            {
                std::cout<<"test2"<<std::endl;
                node_skeleton->setLoopMode(true);
                node_skeleton->setMD2Animation(is::EMAT_JUMP);
                //node_ennemi->drop();
                return true;
            }
    }

    return false;
}

