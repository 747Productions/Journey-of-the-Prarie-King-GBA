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
//includes for sprite items
#include "bn_sprite_items_player.h"
#include "bn_sprite_items_bullet.h"
#include "bn_sprite_text_generator.h"
#include "unifont_sprite_font.h"
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


//update text labels on the screen to reflect the current game state
void updateLabels(bn::vector<bn::sprite_ptr, 10>& text_sprites, bn::sprite_text_generator text_generator,int lives, int score) {
    //clear sprite vector in order to replace the text with updated values
    text_sprites.clear();
    // 1. Create a persistent named buffer
    bn::string<32> score_string;
    bn::string<32> lives_string;
    // 2. Pass the buffer by reference into the stream
    bn::ostringstream score_stream(score_string);
    bn::ostringstream lives_stream(lives_string);
    // 3. Populate the stream
    score_stream << "SCORE: " << score;
    lives_stream << "LIVES: " << lives;

    // 4. Wipe old sprites and generate new ones (using the buffer we just filled)
    text_generator.generate(-120, -60, score_string, text_sprites);
    text_generator.generate(-120, -50, lives_string, text_sprites);
}

int main()
{
    bn::core::init();
    bn::music_items::theme.play(theme_volume);
    bn::sprite_ptr player_sprite = bn::sprite_items::player.create_sprite(50, 50);
    Player player(player_sprite);
    bn::sprite_text_generator text_generator(unifont_sprite_font);
    //the game requires very few text sprites so four should be enough
    bn::vector<bn::sprite_ptr, 10> text_sprites;
    

    while(true)
    {    
        text_sprites.clear();
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
            enemy_spawn_cooldown = 300;
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