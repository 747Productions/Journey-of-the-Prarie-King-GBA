//pragma directives to ignore uneccesary warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored  "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wreorder"
//basic std includes
#include <iostream>
#include <memory>
#include <format>
//butano type/utility includes
#include "bn_string.h" // Required for string utilities
#include "bn_vector.h"
#include "bn_keypad.h"
#include "bn_fixed.h"
#include "bn_core.h"
#include "bn_sprite_ptr.h"
#include "bn_sound.h"
#include "bn_sound_items.h"
#include "bn_rect.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_tiles_items_groundtile.h"
#include "bn_random.h"
#include "bn_camera_ptr.h"
//includes for sprite items
#include "bn_sprite_items_player.h"
#include "bn_sprite_items_bullet.h"
#include "bn_sprite_text_generator.h"
#include "unifont_sprite_font.h"
//includes for background items
#include "bn_regular_bg_items_desertmap.h"
//includes for music
#include "bn_music.h"
#include "bn_music_items.h"
//custom header files for each custom class
#include "sprites/Player.hpp"
#include "sprites/playerProjectile.hpp"
#include "sprites/Enemy.hpp"
#include "sprites/enemyProjectile.hpp"
//#include "sprites/boundingBox.hpp"
//includes for fonts
#include "unifont_sprite_font.h"
//custom header files for utility functions
#include "utils/collides.hpp"
#include "utils/spawnEnemy.hpp"
#include "utils/coord_inside.hpp"
//utility functions for graphics
#include "graphics/text.hpp"
//debug variables to make sound mixing easier
float theme_volume = 0.5;
float footstep_volume = 0.3;
//create random number generator
bn::random rng;
//screen height and width are 240 and 160 respectively

//create text generator for font and a text container vector to hold the text sprites
bn::sprite_text_generator text_generator(unifont_sprite_font);

bn::vector<bn::sprite_ptr, 32> text_sprites;

int lives = 5;
int score = 0;
//bools to set which powerups are active
//bool upgrade_active = false;
//bool wagon_wheel = false;
//bool coffee = false;
//bool machine_gun = false;
//bool badge = false;
//bool gravestone = false;
//int stored_upgrade = 0;
//start theme music
//create vector for player projectiles
bn::vector<playerProjectile, 30> projectiles;
//create vector for enemy projectiles
bn::vector<enemyProjectile, 10> enemy_projectiles;
//create vector for enemies
bn::vector<Enemy, 30> enemies;
//create vector for bounding boxes and add a test to the vector
//initalize sprite for player and create actual object

//tracker to keep track of how many frames are left uintil the player is able to shoot again
int player_shooting_cooldown = 0;
//tracker for how often the footstep sound plays
int footstep_cooldown = 0;
int enemy_spawn_cooldown = 0;

bool game_paused = false;
void unloadGame() {
    // Clear all vectors to free up memory
    projectiles.clear();
    enemy_projectiles.clear();
    enemies.clear();
    game_paused = true;
    
    
}
void loadArea(){
    
}

//game to reload the scene after the player dies
void reloadGame() {
    // Reset game state variables
    lives = 5;
    score = 0;
    player_shooting_cooldown = 0;
    footstep_cooldown = 0;
    enemy_spawn_cooldown = 0;
    
    // Clear all vectors to free up memory
    projectiles.clear();
    enemy_projectiles.clear();
    enemies.clear();
    
    game_paused = false;
}
//prevent the camera from moving out of
#include "bn_regular_bg_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_math.h"
#include "bn_size.h"

//adjust the camera and player sprite to prevent them from moving out of bounds of the background
void adjustSprites(const bn::regular_bg_ptr& bg, bn::camera_ptr& camera, bn::sprite_ptr& player_sprite) {
    //get backgrond dimensions and half width/height for
    bn::size bg_size = bg.dimensions();
    bn::fixed bg_half_width = bg_size.width() / 2;
    bn::fixed bg_half_height = bg_size.height() / 2;
    
    // get coordinates of the background edges
    bn::fixed map_left   = bg.x() - bg_half_width;
    bn::fixed map_right  = bg.x() + bg_half_width;
    bn::fixed map_top    = bg.y() - bg_half_height;
    bn::fixed map_bottom = bg.y() + bg_half_height;
    
    // clamp players position to prevent walking out of bounds
    player_sprite.set_x(bn::clamp(player_sprite.x(), map_left, map_right));
    player_sprite.set_y(bn::clamp(player_sprite.y(), map_top, map_bottom));
    
    //do the same for the camera
    bn::fixed cam_left_bound   = map_left + (240 / 2);
    bn::fixed cam_right_bound  = map_right - (240 / 2);
    bn::fixed cam_top_bound    = map_top + (160 / 2);
    bn::fixed cam_bottom_bound = map_bottom - (160 / 2);
    
    // 5. Update camera position to follow the player, confined to bounds
    camera.set_x(bn::clamp(player_sprite.x(), cam_left_bound, cam_right_bound));
    camera.set_y(bn::clamp(player_sprite.y(), cam_top_bound, cam_bottom_bound));
}


int main()
{
    bn::core::init();
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
    bn::regular_bg_ptr bg = bn::regular_bg_items::desertmap.create_bg(0, -0);
    bn::music_items::theme.play(theme_volume);
    bn::sprite_ptr player_sprite = bn::sprite_items::player.create_sprite(50, 50);
    Player player(player_sprite);
    bn::sprite_text_generator text_generator(unifont_sprite_font);
    //the game requires very few text sprites so four should be enough
    bn::vector<bn::sprite_ptr, 10> text_sprites;
    //test sprites delete before pushing
    bg.set_camera(camera);
    while(true)
    {    
        adjustSprites(bg,camera,player.player_sprite);
        updateLabels(text_sprites, text_generator, lives, score);
        //play footstep noise if any of the dpad buttons are held
        if(bn::keypad::left_held() || bn::keypad::right_held() || bn::keypad::up_held() || bn::keypad::down_held()) {
            if(footstep_cooldown == 0){
                bn::sound_items::footstep.play(footstep_volume);
                footstep_cooldown = 20;
            }
        }
        //spawn an enemy every 300 frames
        if(enemy_spawn_cooldown == 0){
            spawnEnemy(enemies, rng,3);
            enemy_spawn_cooldown = 600;
        }
        //move the player if any dpad buttons are held and the player is still alive
        if(player.alive){
            player.move_check();
        }
        //player fire checks
        if(bn::keypad::a_held())
        {
            //check if the player is ready to fire anobject shot
            if(player_shooting_cooldown == 0){
                
                // check if vector is full before creating a new projectile to avoid overflow
                if(!projectiles.full())
                {
                    bn::sprite_ptr projectile_sprite = bn::sprite_items::bullet.create_sprite(player.getX(), player.getY());
                    projectiles.emplace_back(playerProjectile(projectile_sprite,player.direction));
                }
                //set shoot cooldown to forty frames to stagger the amount of projectiles
                player_shooting_cooldown = 30;
            }
        }
        //check if any enemies are colliding with the player
        
        
        //check to see if any projectiles are off screen and remove them from the vector if they are
        //if theyre not move them
        auto projectile = projectiles.begin();
        while(projectile != projectiles.end())
        {
            projectile->move();
            
            if(projectile->is_off_screen())
            {
                projectile = projectiles.erase(projectile); 
            }
            else
            {
                ++projectile;
            }
        }
        
        //enemy collision checks
        for (int e = enemies.size() - 1; e >= 0; --e) {
            bool enemy_destroyed = false;
            
            // Check every bullet against the current enemy
            for (int b = projectiles.size() - 1; b >= 0; --b) {
                
                bn::rect bullet_bounds = projectiles[b].get_bounds();
                bn::rect enemy_bounds = enemies[e].get_bounds();
                
                // AABB collision check between bullets and
                if (collides(bullet_bounds, enemy_bounds)) {
                    
                    projectiles.erase(projectiles.begin() + b);
                    
                    enemies.erase(enemies.begin() + e);
                    score += 10; // Increment score for destroying an enemy
                    enemy_destroyed = true;
                    //break to save compute time 
                    break; 
                }
            }
            
            // If this enemy died, jump immediately to evaluating the next enemy
            if (enemy_destroyed) {
                continue;
            }
            
            //if its still alive check to see if it collides with the player
            if (player.alive && collides(player.get_bounds(), enemies[e].get_bounds())) {
                lives--;
                if (lives <= 0) {
                    player.kill();
                }
            }
        }
        //move all the enemies if they arent dead
        for(int e = enemies.size()-1; e >=0;--e){
            enemies[e].move(player.player_sprite);
        }
        //decrement frame delay variables if they aren't at 0 already
        if(player_shooting_cooldown != 0){
            player_shooting_cooldown--;
        }
        if(footstep_cooldown != 0){
            footstep_cooldown --;
        }
        if(enemy_spawn_cooldown != 0){
            enemy_spawn_cooldown --;
        }
        bn::core::update();
        
    }
}