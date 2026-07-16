//basic std includes
#include <iostream>
#include <memory>
//necessary includes for butano
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

//includes for background sprite tiles
#include "bn_regular_bg_tiles_items_groundtile.h"
//includes for sprite items
#include "bn_sprite_items_player.h"
#include "bn_sprite_items_bullet.h"

//includes for music
#include "bn_music.h"
#include "bn_music_items.h"
//#include "bn_sprite_items_enemy.h"
//debug variables to make sound mixing easier
float theme_volume = 0.5;
float footstep_volume = 0.3;
//DONT FORGET THAT THE NEXT THING YOU SHOULD DO IS MAKE A CLASS FOR POWERUPS LIKE THE WAGON WHEEL
//classes for both players and enemies
class Player{
    public:
    bn::sprite_ptr player_sprite;
    int width = 32;
    int height = 32;
    int direction = 0;
    bool alive = true;
    Player(bn::sprite_ptr player_sprite) : player_sprite(player_sprite)
    {
        
    }
    void move_check(){
        
        //player movement checks
        if(bn::keypad::right_held())
        {
            player_sprite.set_x(player_sprite.x() + 1);   
            direction = 0;
        }
        if(bn::keypad::left_held())
        {
            player_sprite.set_x(player_sprite.x() - 1);
            direction = 1;
        }
        
        if(bn::keypad::up_held())
        {
            player_sprite.set_y(player_sprite.y() - 1);
            direction = 2;
        }
        if(bn::keypad::down_held())
        {
            player_sprite.set_y(player_sprite.y() + 1);   
            direction = 3;
        }
        
    }
    bn::rect get_bounds() const {
        
        bn::fixed_point pos = player_sprite.position();
        return bn::rect(pos.x().integer() - (width / 2), pos.y().integer() - (height / 2), width, height);
    }
    bn::fixed getX(){
        return player_sprite.x();
    }
    bn::fixed getY(){
        return player_sprite.y();
    }
    void kill(){
        alive = false;
        player_sprite.set_visible(false);
    }
};
//classes for both player and enemy/boss projectiles
class playerProjectile
{
    public:
    int direction;
    bn::sprite_ptr projectile_sprite;
    int width = 4;
    int height = 4;
    
    playerProjectile(int direction, bn::sprite_ptr sprite) : 
    direction(direction), projectile_sprite(bn::move(sprite))
    {
        projectile_sprite.set_scale(.5);
    }
    
    // Move constructor
    playerProjectile(playerProjectile&& other) noexcept :
    direction(other.direction),
    projectile_sprite(bn::move(other.projectile_sprite)),
    width(other.width),
    height(other.height)
    {}
    
    // Move assignment
    playerProjectile& operator=(playerProjectile&& other) noexcept {
        if (this != &other) {
            direction = other.direction;
            projectile_sprite = bn::move(other.projectile_sprite);
            width = other.width;
            height = other.height;
        }
        return *this;
    }
    
    // fix to allow the safe use of smart pointer as object properties
    playerProjectile(const playerProjectile&) = delete;
    playerProjectile& operator=(const playerProjectile&) = delete;
    
    void move(){
        if(direction == 0)      projectile_sprite.set_x(projectile_sprite.x() + 2);
        else if(direction == 1) projectile_sprite.set_x(projectile_sprite.x() - 2);
        else if(direction == 2) projectile_sprite.set_y(projectile_sprite.y() - 2);
        else if(direction == 3) projectile_sprite.set_y(projectile_sprite.y() + 2);
    }
    
    bool is_off_screen() {
        return (projectile_sprite.x() > 128 || projectile_sprite.x() < -128 || 
        projectile_sprite.y() > 88  || projectile_sprite.y() < -88);
    }
    
    bn::rect get_bounds() const {
        bn::fixed_point pos = projectile_sprite.position();
        return bn::rect(pos.x().integer() - (width / 2), pos.y().integer() - (height / 2), width, height);
    }
};

class enemyProjectile{
    public:
    int direction;
    bn::sprite_ptr projectile_sprite;
    
    enemyProjectile(int direction, bn::sprite_ptr projectile_sprite) : direction(direction), projectile_sprite(projectile_sprite)
    {
        projectile_sprite.set_scale(.5);
    }
    void move(){
        if(direction == 0)
        {
            projectile_sprite.set_x(projectile_sprite.x() + 1);
        }
        else if(direction == 1)
        {
            projectile_sprite.set_x(projectile_sprite.x() - 1);
        }
        else if(direction == 2){
            projectile_sprite.set_y(projectile_sprite.y() - 1);
        }
        else if(direction == 3){
            projectile_sprite.set_y(projectile_sprite.y() + 1);
        }
    }
    
};
//class for enemies
class Enemy {
    public:
    bn::sprite_ptr enemy_sprite;
    int type; 
    int height;
    int width;
    
    // Standard constructor using bn::move to safely accept the sprite handle
    Enemy(int type, bn::sprite_ptr enemy_sprite) : 
    type(type), enemy_sprite(bn::move(enemy_sprite)) 
    {
        this->enemy_sprite.set_scale(.5);
        height = 16; // Adjusted bounds to match the half-scale sprite projection
        width = 16;
    }
    
    // move constructor
    Enemy(Enemy&& other) noexcept :
    enemy_sprite(bn::move(other.enemy_sprite)),
    type(other.type),
    height(other.height),
    width(other.width)
    {}
    
    //move assignment for save use in vectors
    Enemy& operator=(Enemy&& other) noexcept {
        if (this != &other) {
            enemy_sprite = bn::move(other.enemy_sprite);
            type = other.type;
            height = other.height;
            width = other.width;
        }
        return *this;
    }
    
    // safe use smart pointer fix that prevents shallow copy pointer issues 
    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;
    
    void move() {
        //DO SOON
    }
    
    bn::rect get_bounds() const {
        bn::fixed_point pos = enemy_sprite.position();
        return bn::rect(pos.x().integer() - (width / 2), pos.y().integer() - (height / 2), width, height);
    }
};

//check if two rects collide with eachother
bool collides(bn::rect b1, bn::rect b2){
    return b1.x() < b2.x() + b2.width() &&
    b1.x() + b1.width() > b2.x() &&
    b1.y() < b2.y() + b2.height() &&
    b1.y() + b1.height() > b2.y();
}
//
int main()
{
    
    bn::core::init();
    //create background
    
    int lives = 5;
    //bools to set which powerups are active
    //bool upgrade_active = false;
    //bool wagon_wheel = false;
    //bool coffee = false;
    //bool machine_gun = false;
    //bool badge = false;
    bool gravestone = false;
    //int stored_upgrade = 0;
    //start theme music
    bn::music_items::theme.play(theme_volume);
    //create vector for player projectiles
    bn::vector<playerProjectile, 30> projectiles;
    //create vector for enemy projectiles
    bn::vector<enemyProjectile, 10> enemy_projectiles;
    //create vector for enemies
    bn::vector<Enemy, 30> enemies;
    bn::sprite_ptr test_enemy = bn::sprite_items::player.create_sprite(0,0);
    //initalize sprite for player and create actual object
    bn::sprite_ptr player_sprite = bn::sprite_items::player.create_sprite(50, 50);
    Player player(player_sprite);
    //
    //tracker to keep track of how many frames are left uintil the player is able to shoot again
    int player_shooting_cooldown = 0;
    //tracker for how often the footstep sound plays
    int footstep_cooldown = 0;
    //tracker to keep track of when we should play the footstep sound
    //core update loop 
    while(true)
    {    
        //play footstep noise if any of the dpad buttons are held
        if(bn::keypad::left_held() || bn::keypad::right_held() || bn::keypad::up_held() || bn::keypad::down_held()) {
            if(footstep_cooldown == 0){
                bn::sound_items::footstep.play(footstep_volume);
                footstep_cooldown = 20;
            }
        }
        //move the player if any dpad buttons are held
        player.move_check();
        //player fire checks
        if(bn::keypad::a_held())
        {
            //check if the player is ready to fire another shot
            if(player_shooting_cooldown == 0){
                
                // check if vector is full before creating a new projectile to avoid overflow
                if(!projectiles.full())
                {
                    bn::sprite_ptr projectile_sprite = bn::sprite_items::bullet.create_sprite(player.getX(), player.getY());
                    projectiles.emplace_back(playerProjectile(player.direction, projectile_sprite));
                }
                //set shoot cooldown to forty frames to stagger the amount of projectiles
                player_shooting_cooldown = 30;
            }
        }
        //check if any enemies are colliding with the enemy
        auto enemy = enemies.begin();
        while(enemy != enemies.end()){
            enemy->move();
            bn::rect playerBB = player.get_bounds();
            bn::rect enemyBB = enemy->get_bounds();
            if(collides(playerBB,enemyBB)){
                enemies.erase(enemy);
                player.kill();
            }
        }
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
        
        //check to see if any projectiles collide with an enemy
        //if so destroy both the  enemy and bullet objects
        for (int e = enemies.size() - 1; e >= 0; --e) {
            bool enemy_destroyed = false;
            
            // Check every bullet against this specific enemy
            for (int b = projectiles.size() - 1; b >= 0; --b) {
                
                // Get bounding boxes using our fixed .set_scale(.5) adjustments
                bn::rect bullet_bounds = projectiles[b].get_bounds();
                bn::rect enemy_bounds = enemies[e].get_bounds();
                
                // Run the AABB intersection check
                if (collides(bullet_bounds, enemy_bounds)) {
                    
                    // 1. Remove the bullet from the hardware vector list
                    projectiles.erase(projectiles.begin() + b);
                    
                    // 2. Remove the enemy from the hardware vector list
                    enemies.erase(enemies.begin() + e);
                    
                    // 3. Mark true so we stop checking bullets for this dead enemy
                    enemy_destroyed = true;
                    break; 
                }
            }
            
            // If this enemy died, jump immediately to evaluating the next enemy
            if (enemy_destroyed) {
                continue;
            }
            
            // Optional: Check if this surviving enemy touches the player
            if (player.alive && collides(player.get_bounds(), enemies[e].get_bounds())) {
                lives--;
                if (lives <= 0) {
                    player.kill();
                    gravestone = true;
                }
            }
        }
        //decrement frame delay variables if they aren't at 0 already
        if(player_shooting_cooldown != 0){
            player_shooting_cooldown--;
        }
        if(footstep_cooldown != 0){
            footstep_cooldown --;
        }
        bn::core::update();
        
    }
}