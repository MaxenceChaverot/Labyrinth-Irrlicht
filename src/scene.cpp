// scene.cpp

#include <irrlicht.h>
#include <iostream>
#include <stdlib.h>

#include "scene.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

namespace ig = irr::gui;

/**************************************************************************\
 * myScene::Constructor                                             *
\**************************************************************************/
myScene::myScene()
    :
      smgr(nullptr),
      receiver(EventReceiver()),
      driver(nullptr),
      gui(nullptr),
      mesh_scene(nullptr),
      node_scene(nullptr),
      is_ennemi_dead(false),
      time(0),
      start_timer(false)
{
}

/**************************************************************************\
 * myScene::Init                                             *
\**************************************************************************/
void myScene::scene_init()
{
    // Création de la fenêtre et du système de rendu.
    device = createDevice(iv::EDT_OPENGL,ic::dimension2d<u32>(640, 480),16, false, false, false, &receiver);

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    gui  = device->getGUIEnvironment();

    // Chargement du décors :

    // ***** DECORS *****

    // On charge le mesh (fichier .obj)
    mesh_scene = smgr->getMesh("data/Map/Labyrinth.obj");
    // On l'associe a un noeud de la scene
    node_scene = smgr->addOctreeSceneNode(mesh_scene->getMesh(0));
    node_scene->setID(2);
    // On augmente la taille du décors
    node_scene->setScale(ic::vector3df(100, 300, 100));
    // On specifie sa texture
    node_scene->setMaterialTexture(0, driver->getTexture("data/Textures/texture_lab.jpg"));
    // On rend le mesh insensible a la lumiere
    node_scene->setMaterialFlag(irr::video::EMF_LIGHTING, false);

    // Chargement de la caméra
    load_camera_gun();

    // Chargement des enemies
    load_enemies();

    // Initialisation du score
    init_score();
}

/**************************************************************************\
 * myScene::Run                                             *
\**************************************************************************/
void myScene::scene_run()
{
    while(device->run())
    {
         driver->beginScene(true, true, iv::SColor(100,150,200,255));

         // Shoot
         gun_shoot();
         if(is_ennemi_dead == false) sk1.hasPlayerInSight(node_sphere);
         display_map();

         if(receiver.get_display_menu() == false)
             smgr->drawAll();
         else
         {
             create_window();
         }

         gui->drawAll();

         driver->endScene();

    }
    device->drop();
}

void myScene::create_window()
{
     // Affichage du fond pour le menu
     iv::ITexture* TextMenu = driver->getTexture("data/Textures/background_menu.jpeg");
     ig::IGUIImage* Im_BackgroundMenu = gui->addImage(ic::rect<s32>(0,0,800,800));
     Im_BackgroundMenu->setImage(TextMenu);

     // Nom du Jeu Vidéo
     iv::ITexture* tTitle = driver->getTexture("data/Textures/menu_name.png");
     ig::IGUIImage* Im_Title = gui->addImage(ic::rect<s32>(200,20,800,800));
     Im_Title->setImage(tTitle);

     // Ajouter les menus
     irr::gui::IGUIButton *New_game_button = gui->addButton(irr::core::rect<irr::s32>(220,150,350,200), 0, -1, L"Nouvelle Partie");
}

void myScene::display_map()
{
    iv::ITexture* TextMap = driver->getTexture("screenshot/carte.png");
    ig::IGUIImage* LabMap;

    if(receiver.get_display_map() == true)
    {
        // Affichage de la carte
        LabMap = gui->addImage(ic::rect<s32>(100,10,600,600));
        LabMap->setImage(TextMap);
    }
    if(receiver.get_remove_map() == true)
    {
        std::cout<<"LALLALALALALALALAALALAL"<<std::endl;
        LabMap->remove();
    }
}

void myScene::init_score()
{
    numbers.push_back(driver->getTexture("data/Nombres/0.png"));
    numbers.push_back(driver->getTexture("data/Nombres/1.png"));
    numbers.push_back(driver->getTexture("data/Nombres/2.png"));
    numbers.push_back(driver->getTexture("data/Nombres/3.png"));
    numbers.push_back(driver->getTexture("data/Nombres/4.png"));
    numbers.push_back(driver->getTexture("data/Nombres/5.png"));
    numbers.push_back(driver->getTexture("data/Nombres/6.png"));
    numbers.push_back(driver->getTexture("data/Nombres/7.png"));
    numbers.push_back(driver->getTexture("data/Nombres/8.png"));
    numbers.push_back(driver->getTexture("data/Nombres/9.png"));

    // Création des places pour les chiffres
    score_images.push_back(gui->addImage(ic::rect<s32>(90,10,130,50)));
    score_images.push_back(gui->addImage(ic::rect<s32>(50,10,90,50)));
    score_images.push_back(gui->addImage(ic::rect<s32>(10,10,50,50)));

    score_images[0]->setScaleImage(true);
    score_images[1]->setScaleImage(true);
    score_images[2]->setScaleImage(true);

    score_images[0]->setImage(numbers[0]);
    score_images[1]->setImage(numbers[0]);
    score_images[2]->setImage(numbers[0]);
}

void myScene::load_enemies()
{
    sk1 = skeleton(device,ic::vector3df(-40, 25, 302),ic::vector3df(0, 90, 0),1);
    is::IAnimatedMeshSceneNode *node_ennemi = sk1.getNode();

    is::ITriangleSelector *selector2 = smgr->createTriangleSelector(node_ennemi);
    node_ennemi->setTriangleSelector(selector2);
}

void myScene::load_camera_gun()
{
    // ***** CAMERA *****

    // Caméra "jeu video à la première personne"

    // Création du triangle selector
    scene::ITriangleSelector *selector;
    selector = smgr->createOctreeTriangleSelector(node_scene->getMesh(), node_scene);
    node_scene->setTriangleSelector(selector);

    // Création de la caméra
    SKeyMap keyMap[9];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_Z;
    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;
    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT ;
    keyMap[5].KeyCode = KEY_KEY_Q;
    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;
    keyMap[8].Action = EKA_JUMP_UP;
    keyMap[8].KeyCode = KEY_SPACE;

    camera = smgr->addCameraSceneNodeFPS(nullptr,
                                            100,         // Vitesse de rotation
                                            .6,          // Vitesse de déplacement
                                            11,          // Identifiant
                                            keyMap, 9,  // Table de changement de touches
                                            true,        // pas de possibilité de voler
                                            5);          // Vitesse saut
    camera->setPosition(ic::vector3df(-151, 40, 40));
    camera->setTarget(ic::vector3df(-200, 40, 40));

    scene::ISceneNodeAnimator *anim;
    anim = smgr->createCollisionResponseAnimator(selector,
                                                     camera,  // Le noeud que l'on veut gérer
                                                     ic::vector3df(25, 25, 25), // "rayons" de la caméra
                                                     ic::vector3df(0, -9.81, 0),  // gravité
                                                     ic::vector3df(0, 0, 0));  // décalage du centre
    camera->addAnimator(anim);

    // ***** GUN ***** //

    // Texture
    iv::ITexture *gun_texture;
    gun_texture = driver->getTexture("data/Textures/texture_gun.jpeg");

    // Object
    is::IAnimatedMesh *gun = smgr->getMesh("data/Models/gun.3ds");
    node_gun = smgr->addAnimatedMeshSceneNode(gun,camera);
    node_gun->setScale(ic::vector3df(2, 2, 2));
    node_gun->setPosition(ic::vector3df(0, -5, 10));
    node_gun->setRotation(ic::vector3df(-5, -15, 0));
    node_gun->setMaterialTexture(0, gun_texture);
    node_gun->setMaterialFlag(iv::EMF_LIGHTING, false);
    receiver.set_node(camera);
	
	//BoundingSphere For collision
    is::IMesh *mesh_sphere = smgr->getGeometryCreator()->createSphereMesh(5.f);
    node_sphere = smgr->addMeshSceneNode(mesh_sphere,camera);
    node_sphere->setPosition(ic::vector3df(0, -5, 10));
    node_gun->setRotation(ic::vector3df(-5, -15, 0));
    node_sphere->setID(6);
    //node_sphere->setMaterialTexture(driver->getTexture("data/textures/texture_lab"));
    //node_sphere->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node_sphere->setVisible(true);

}

void myScene::check_timer()
{
    if(time == 52)
    {
        is::IAnimatedMeshSceneNode *node_ennemi = sk1.getNode();
        node_ennemi->remove();
        start_timer = false;
        is_ennemi_dead = true;
    }
}

/**************************************************************************\
 * myScene::shoot                                                *
\**************************************************************************/
void myScene::gun_shoot()
{
    // ***** BALL SHOOT ***** //

    int click_x, click_y;
    is::ISceneCollisionManager *collision_manager = smgr->getSceneCollisionManager();

    if (receiver.is_mouse_pressed(click_x, click_y))
    {
        // Ball
        /*is::IAnimatedMesh *ball = smgr->getMesh("data/Models/Ball.obj");
        is::IAnimatedMeshSceneNode *node_ball = smgr->addAnimatedMeshSceneNode(ball, camera);
        node_ball->setPosition(ic::vector3df(-2.5, -8, 31));
        node_ball->setScale(ic::vector3df(0.05, 0.05, 0.05));

        ic::line3d<f32> ray;
        ray = collision_manager->getRayFromScreenCoordinates(ic::position2d<s32>(click_x, click_y));
        ic::vector3df intersection;
        ic::triangle3df hit_triangle;
        is::ISceneNode *selected_scene_node = collision_manager->getSceneNodeAndCollisionPointFromRay(
                            ray,
                            intersection, // On récupère ici les coordonnées 3D de l'intersection
                            hit_triangle, // et le triangle intersecté
                            node_scene->getID()); // On ne veut que des noeuds avec cet identifiant

        // Animation
        scene::ISceneNodeAnimator *shoot_anim;
        shoot_anim = smgr->createFlyStraightAnimator(    node_ball->getPosition(), // Start Point
                                                         ic::vector3df(click_x, click_y, 100),  // End Point
                                                         5, // Time for Way
                                                         false, // loop
                                                         false);  // pingpong
        node_ball->addAnimator(shoot_anim);*/

        ic::line3d<f32> ray;
        ray = collision_manager->getRayFromScreenCoordinates(ic::position2d<s32>(click_x, click_y));

        ic::vector3df intersection;
        ic::triangle3df hit_triangle;

        if(is_ennemi_dead == false)
        {
            is::IAnimatedMeshSceneNode *node_ennemi = sk1.getNode();
            is::ISceneNode *selected_scene_node = collision_manager->getSceneNodeAndCollisionPointFromRay(ray,intersection,hit_triangle,node_ennemi->getID());

            if (selected_scene_node)
            {
                    if(node_ennemi == selected_scene_node)
                    {
                        node_ennemi->setLoopMode(false);
                        node_ennemi->setMD2Animation(is::EMAT_DEATH_FALLBACK);
                        start_timer = true;
                    }
            }
        }
    }

    if(start_timer == true)
    {
         time = time + 1;
         check_timer();
    }
}