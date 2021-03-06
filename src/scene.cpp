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
      tps(0),
      is_map_draw(false)
{
    //Chargement des postions initiales des ennemis
    vec_ennemi_pos = load_ennemi_pos();
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

    /** Chargement du décors */

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

    /** Initialisation du timer qui gère le temps global d'exécution du jeu */

    // Initialisation du temps
    init_timer();

    // Initialisation des variables pour gérer le temps global
    timer = device->getTimer();
    remaining_time = 10;
    timer->start();

    time_minute = 60000; // Une minute
    is_finished = false;

    // Creation de l'affichage de la carte
    iv::ITexture* TextMap = driver->getTexture("screenshot/carte.png");
    LabMap = gui->addImage(ic::rect<s32>(100,10,600,600));
    LabMap->setImage(TextMap);
    LabMap->setVisible(false);
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

        // Affichage de la map
        display_map();

        // Mise à jour du temps restant du jeu
        update_timer();
        bool is_minute_past = check_if_minute_past();
        if(is_minute_past == true)
        {
            if(remaining_time != 0) remaining_time = remaining_time - 1;
        }

        // Si le jeu est perdu car le temps est écoulé
        is_finished = check_global_time();
        if (is_finished == true)
        {
            game_over();
        }

        check_if_game_is_won();

        smgr->drawAll();

        //pour chaque ennemi
        for(int i = 0; i < vec_ennemy.size(); ++i){
            //S'il n'est pas mort
            if(vec_ennemy[i].isDead ()== false){
                //Orienter l'ennemi face au joueur
                vec_ennemy[i].faceModelToPlayer();
                //Gerer les animations et le deplacement si l'ennemi a le joueur en vu
                vec_ennemy[i].hasPlayerInSight(node_sphere);

            }
            //Timer pour supprimer le node de l'ennemi une fois vaincu
            vec_ennemy[i].check_death_timer();
        }

        gui->drawAll();

        driver->endScene();

    }
    device->drop();
}

/**************************************************************************\
 * myScene::Affichage de la carte du labyrinthe                                       *
\**************************************************************************/
void myScene::display_map()
{


    if(receiver.get_display_map() == true && !is_map_draw)
    {
        // Affichage de la carte
        LabMap->setVisible(true);
        is_map_draw = true;
    }
    if(receiver.get_display_map() == false && is_map_draw)
    {
        //Desaffichage de la carte
       LabMap->setVisible(false);
       is_map_draw = false;
    }
}
/**************************************************************************\
 * myScene::Init global timer (the player has 15 minutes to play)                                   *
\**************************************************************************/
void myScene::init_timer()
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
    timer_images.push_back(gui->addImage(ic::rect<s32>(570,10,610,50)));
    timer_images.push_back(gui->addImage(ic::rect<s32>(530,10,570,50)));

    timer_images[0]->setScaleImage(true);
    timer_images[1]->setScaleImage(true);
}

/**************************************************************************\
 * myScene::Check minute past                                   *
\**************************************************************************/
bool  myScene::check_if_minute_past()
{
    bool result = false;

    tps = timer->getTime();
    if(tps >= time_minute)
    {
        tps = 0;
        timer->setTime(0);
        result = true;
    }

    return result;
}

/**************************************************************************\
 * myScene::Check global timer (the player has 15 minutes)                                      *
\**************************************************************************/
bool myScene::check_global_time()
{
    bool result = false;
    if(remaining_time == 0) result = true;

    return result;
}

/**************************************************************************\
 * myScene::Update global timer                                     *
\**************************************************************************/
void myScene::update_timer()
{
    if(remaining_time == 10)
    {
        timer_images[1]->setImage(numbers[1]);
        timer_images[0]->setImage(numbers[0]);
    }
    else
    {
        timer_images[1]->setImage(numbers[0]);
        timer_images[0]->setImage(numbers[remaining_time]);
    }

}

/**************************************************************************\
 * myScene::Game OVER                                        *
\**************************************************************************/
void myScene::game_over()
{
    gui->addMessageBox(L"GAME OVER", L"Your game time is over and you have not managed to get out of the labyrinth. \n You lost the game. \n You have to close the window and relaunch the game to replay.");
}

/**************************************************************************\
 * myScene::Check if game is won                                     *
\**************************************************************************/
void myScene::check_if_game_is_won()
{
    // Si le joueur est à la sortie du labyrinthe
    if(camera->getPosition().X  >= 900   &&   camera->getPosition().X  <= 990    &&  camera->getPosition().Z  >= -950   &&   camera->getPosition().Z  <= -930)
    {
        gui->addMessageBox(L"CONGRATULATION", L"Congratulations, you managed to find the exit of the labyrinth in less than ten minutes. \n You won the game. \n You have to close the window and relaunch the game to replay");
    }

}

void myScene::load_enemies()
{
    //Pour chaque position initiale chargée
    for(int i = 0; i < vec_ennemi_pos.size(); ++i){
        //Créer un ennemi a la position
        skeleton sk_current = skeleton(device,vec_ennemi_pos[i],ic::vector3df(0, 0, 0),i+1);

        is::IAnimatedMeshSceneNode *node_ennemi = sk_current.getNode();

        //Selector pour le node
        is::ITriangleSelector *selector_ennemy = smgr->createTriangleSelector(node_ennemi);
        node_ennemi->setTriangleSelector(selector_ennemy);

        //Colision
        scene::ISceneNodeAnimator *anim;
        anim = smgr->createCollisionResponseAnimator(node_scene->getTriangleSelector(),
                                                     node_ennemi,  // Le noeud que l'on veut gérer
                                                     ic::vector3df(25, 25, 25), // "rayons" de la caméra
                                                     ic::vector3df(0, -9.81, 0),  // gravité
                                                     ic::vector3df(0, 0, 0));  // décalage du centre
        node_ennemi->addAnimator(anim);

        vec_ennemy.push_back(sk_current);
    }
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
    node_sphere->setVisible(false);

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
        ic::line3d<f32> ray;
        ray = collision_manager->getRayFromScreenCoordinates(ic::position2d<s32>(click_x, click_y));

        ic::vector3df intersection;
        ic::triangle3df hit_triangle;

        for(int i = 0; i < vec_ennemy.size(); ++i){
            if(vec_ennemy[i].isDead() == false){
                is::IAnimatedMeshSceneNode *node_ennemi = vec_ennemy[i].getNode();
                is::ISceneNode *selected_scene_node = collision_manager->getSceneNodeAndCollisionPointFromRay(ray,intersection,hit_triangle,node_ennemi->getID());

                if (selected_scene_node)
                {
                    if(node_ennemi == selected_scene_node)
                    {
                        vec_ennemy[i].start_dead_animation();
                    }
                }

            }
        }
    }
}

/**************************************************************************\
 * myScene::load_ennemi_pos : chargement des positions initiales des ennemis                                          *
\**************************************************************************/
std::vector<ic::vector3df> myScene::load_ennemi_pos(){

    std::vector<ic::vector3df> vec;

    vec.push_back(ic::vector3df(ic::vector3df(-40, 17, 302))); //1
    vec.push_back(ic::vector3df(ic::vector3df(122, 17, 61))); //2
    vec.push_back(ic::vector3df(ic::vector3df(53, 17, -130))); //3
    vec.push_back(ic::vector3df(ic::vector3df(-236, 17, 363))); //4
    vec.push_back(ic::vector3df(ic::vector3df(-355, 17, 151))); //5
    vec.push_back(ic::vector3df(ic::vector3df(297,17,-334))); //6
    vec.push_back(ic::vector3df(ic::vector3df(-342, 17, -383))); //7
    vec.push_back(ic::vector3df(ic::vector3df(-44, 17, -926))); //8
    vec.push_back(ic::vector3df(ic::vector3df(-273, 17, -914))); //9
    vec.push_back(ic::vector3df(ic::vector3df(-551, 17, -918))); //10
    vec.push_back(ic::vector3df(ic::vector3df(-730, 17, -730))); //11
    vec.push_back(ic::vector3df(ic::vector3df(448, 17, 110))); //12
    vec.push_back(ic::vector3df(ic::vector3df(640, 17, -232))); //13
    vec.push_back(ic::vector3df(ic::vector3df(734, 17, -526))); //14
    vec.push_back(ic::vector3df(ic::vector3df(248, 17, -787 ))); //15
    vec.push_back(ic::vector3df(ic::vector3df(530, 17, -918))); //16
    vec.push_back(ic::vector3df(ic::vector3df(738, 17, -926))); //17
    vec.push_back(ic::vector3df(ic::vector3df(893, 17, -910))); //18
    vec.push_back(ic::vector3df(ic::vector3df(930, 17, -306))); //19
    vec.push_back(ic::vector3df(ic::vector3df(926, 17, 359 ))); //20
    vec.push_back(ic::vector3df(ic::vector3df(840, 17, 163))); //21
    vec.push_back(ic::vector3df(ic::vector3df(832, 17, 555))); //22
    vec.push_back(ic::vector3df(ic::vector3df(640, 17, 293))); //23
    vec.push_back(ic::vector3df(ic::vector3df(546, 17, 942))); //24
    vec.push_back(ic::vector3df(ic::vector3df(440, 17, 534))); //25
    vec.push_back(ic::vector3df(ic::vector3df(-44, 17, 644))); //26
    vec.push_back(ic::vector3df(ic::vector3df(-253, 17, 742))); //27
    vec.push_back(ic::vector3df(ic::vector3df(-604, 17, 942))); //28
    vec.push_back(ic::vector3df(ic::vector3df(-551, 17, 563))); //29
    vec.push_back(ic::vector3df(ic::vector3df(-832, 17, 551))); //30
    vec.push_back(ic::vector3df(ic::vector3df(-930, 17, 546))); //31
    vec.push_back(ic::vector3df(ic::vector3df(-926, 17, -191))); //32
    vec.push_back(ic::vector3df(ic::vector3df(-628, 17, -138))); //33
    vec.push_back(ic::vector3df(ic::vector3df(-828, 17, -330))); //34
    vec.push_back(ic::vector3df(ic::vector3df(836, 17, -828))); //35

    return vec;
}
