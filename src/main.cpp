////pragma directives to ignore uneccesary warnings
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored  "-Wunused-variable"
//#pragma GCC diagnostic ignored "-Wshadow"
//#pragma GCC diagnostic ignored "-Wreorder"
////basic std includes
//#include <iostream>
//#include <memory>
////necessary includes for butano
//#include "bn_vector.h"
//#include "bn_keypad.h"
//#include "bn_fixed.h"
//#include "bn_core.h"
//#include "bn_sprite_ptr.h"
//#include "bn_sound.h"
//#include "bn_sound_items.h"
//#include "bn_rect.h"
//#include "bn_regular_bg_ptr.h"
//#include "bn_regular_bg_ptr.h"
//#include "bn_regular_bg_map_item.h"
//#include "bn_regular_bg_tiles_items_groundtile.h"
////includes for sprite items
//#include "bn_sprite_items_player.h"
//#include "bn_sprite_items_bullet.h"
//#include "bn_sprite_text_generator.h"
////includes for music
//#include "bn_music.h"
//#include "bn_music_items.h"
////custom header files for each custom class
//#include "sprites/Player.hpp"
//#include "sprites/playerProjectile.hpp"
//#include "sprites/Enemy.hpp"
//#include "sprites/enemyProjectile.hpp"
////#include "sprites/boundingBox.hpp"
//
////custom header files for utility functions
//#include "utils/collides.hpp"
////debug variables to make sound mixing easier
//float theme_volume = 0.5;
//float footstep_volume = 0.3;
////debug variables to test spawning enemies
////DONT FORGET THAT THE NEXT THING YOU SHOULD DO IS MAKE A CLASS FOR POWERUPS LIKE THE WAGON WHEEL
////classes for both players and enemies
//
////classes for both player and enemy/boss projectiles
//
//
////check if two rects collide with eachobject
//
////
//int main()
//{
//    
//    bn::core::init();
//    //initalize font
//
//    int lives = 5;
//    //bools to set which powerups are active
//    //bool upgrade_active = false;
//    //bool wagon_wheel = false;
//    //bool coffee = false;
//    //bool machine_gun = false;
//    //bool badge = false;
//    //bool gravestone = false;
//    //int stored_upgrade = 0;
//    //start theme music
//    bn::music_items::theme.play(theme_volume);
//    //create vector for player projectiles
//    bn::vector<playerProjectile, 30> projectiles;
//    //create vector for enemy projectiles
//    bn::vector<enemyProjectile, 10> enemy_projectiles;
//    //create vector for enemies
//    bn::vector<Enemy, 30> enemies;
//    enemies.emplace_back(Enemy(0,bn::sprite_items::player.create_sprite(0,0)));
//    //create vector for bounding boxes and add a test to the vector
//    //initalize sprite for player and create actual object
//    bn::sprite_ptr player_sprite = bn::sprite_items::player.create_sprite(50, 50);
//    Player player(player_sprite);
//    //
//    //tracker to keep track of how many frames are left uintil the player is able to shoot again
//    int player_shooting_cooldown = 0;
//    //tracker for how often the footstep sound plays
//    int footstep_cooldown = 0;
//    //tracker to keep track of when we should play the footstep sound
//    //core update loop 
//    while(true)
//    {    
//        //play footstep noise if any of the dpad buttons are held
//        if(bn::keypad::left_held() || bn::keypad::right_held() || bn::keypad::up_held() || bn::keypad::down_held()) {
//            if(footstep_cooldown == 0){
//                bn::sound_items::footstep.play(footstep_volume);
//                footstep_cooldown = 20;
//            }
//        }
//        //move the player if any dpad buttons are held
//        player.move_check();
//        //player fire checks
//        if(bn::keypad::a_held())
//        {
//            //check if the player is ready to fire anobject shot
//            if(player_shooting_cooldown == 0){
//                
//                // check if vector is full before creating a new projectile to avoid overflow
//                if(!projectiles.full())
//                {
//                    bn::sprite_ptr projectile_sprite = bn::sprite_items::bullet.create_sprite(player.getX(), player.getY());
//                    projectiles.emplace_back(playerProjectile(projectile_sprite,player.direction));
//                }
//                //set shoot cooldown to forty frames to stagger the amount of projectiles
//                player_shooting_cooldown = 30;
//            }
//        }
//        //check if any enemies are colliding with the player
//        
//        
//        //check to see if any projectiles are off screen and remove them from the vector if they are
//        //if theyre not move them
//        auto projectile = projectiles.begin();
//        while(projectile != projectiles.end())
//        {
//            projectile->move();
//            
//            if(projectile->is_off_screen())
//            {
//                projectile = projectiles.erase(projectile); 
//            }
//            else
//            {
//                ++projectile;
//            }
//        }
//        
//        //enemy collision checks
//        for (int e = enemies.size() - 1; e >= 0; --e) {
//            bool enemy_destroyed = false;
//            
//            // Check every bullet against the current enemy
//            for (int b = projectiles.size() - 1; b >= 0; --b) {
//                
//                bn::rect bullet_bounds = projectiles[b].get_bounds();
//                bn::rect enemy_bounds = enemies[e].get_bounds();
//                
//                // AABB collision check between bullets and
//                if (collides(bullet_bounds, enemy_bounds)) {
//                    
//                    projectiles.erase(projectiles.begin() + b);
//                    
//                    enemies.erase(enemies.begin() + e);
//                    
//                    enemy_destroyed = true;
//                    //break to save compute time 
//                    break; 
//                }
//            }
//            
//            // If this enemy died, jump immediately to evaluating the next enemy
//            if (enemy_destroyed) {
//                continue;
//            }
//            
//            //if its still alive check to see if it collides with the player
//            if (player.alive && collides(player.get_bounds(), enemies[e].get_bounds())) {
//                lives--;
//                if (lives <= 0) {
//                    player.kill();
//                }
//            }
//        }
//        //move all the enemies if they arent dead
//        for(int e = enemies.size()-1; e >=0;--e){
//            enemies[e].move(player.player_sprite);
//        }
//        //decrement frame delay variables if they aren't at 0 already
//        if(player_shooting_cooldown != 0){
//            player_shooting_cooldown--;
//        }
//        if(footstep_cooldown != 0){
//            footstep_cooldown --;
//        }
//        bn::core::update();
//        
//    }
//}