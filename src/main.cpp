//basic std includes
#include <iostream>
//necessary includes for butano
#include "bn_vector.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_fixed.h"
#include "bn_sprite_ptr.h"
//includes for sprite items
#include "bn_sprite_items_player.h"
#include "bn_sprite_items_bullet.h"
#include "bn_music.h"
#include "bn_music_items.h"
//#include "bn_sprite_items_enemy.h"
class playerProjectile
{
    public:
    int direction;
    bn::sprite_ptr projectile_sprite;
    
    //bullet constructor
    playerProjectile(int direction, bn::sprite_ptr projectile_sprite) : direction(direction), projectile_sprite(projectile_sprite)
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
    
    bool is_off_screen()
    {
        // Butano Screen space center is (0,0). 
        // Horizontal range: -120 to 120 | Vertical range: -80 to 80
        //offset by half of the sprites side length to account for the sprite size
        
        if(projectile_sprite.x() > 120 || projectile_sprite.x() < -120 || 
        projectile_sprite.y() > 80 || projectile_sprite.y() < -80)
        {
            return true;
        }
        return false;
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
class enemy{
    public:
    bn::sprite_ptr enemy_sprite;
    //we will implement enemy types later
    int type; // the only enemy type we will implement is an enemy that runs towards you
    
    enemy(int type, bn::sprite_ptr enemy_sprite) : type(type), enemy_sprite(enemy_sprite)
    {
        //set the scale of the enemy sprite
        enemy_sprite.set_scale(.5);
        
    }
    
    void move(){
        //move the enemy towards the player
        //we will implement this later
    }
};
int main()
{
    bn::core::init();
    //bools to set which powerups are active
    bool upgrade_active = false;
    bool wagon_wheel = false;
    bool coffee = false;
    bool machine_gun = false;
    bool badge = false;
    int stored_upgrade = 0;
    //start theme music
    //bn::music_items::theme.play();
    //create vector for player projectiles
    bn::vector<playerProjectile, 30> projectiles;
    //create vector for enemy projectiles
    //im probably making this wayyyyy to big since almost none of the enemies actually shoot
    bn::vector<enemyProjectile, 10> enemy_projectiles;
    //create vector for enemies
    bn::vector<enemy, 30> enemies;
    //initalize sprite for player
    bn::sprite_ptr player = bn::sprite_items::player.create_sprite(0, 0);
    int player_direction = 0; //0 = right, 1 = left, 2 = up, 3 = down, 4
    //tracker to keep track of how many frames are left uintil the player is able to shoot again
    //the gameboy runs at 60 fps so the shooting cooldown should probably be around 30
    int player_shooting_cooldown = 0;
    //core update loop 
    while(true)
    {    
        //player movement checks
        if(bn::keypad::right_held())
        {
            player.set_x(player.x() + 1);   
            player_direction = 0;
        }
        if(bn::keypad::left_held())
        {
            player.set_x(player.x() - 1);
            player_direction = 1;
        }
        
        if(bn::keypad::up_held())
        {
            player.set_y(player.y() - 1);
            player_direction = 2;
        }
        if(bn::keypad::down_held())
        {
            player.set_y(player.y() + 1);   
            player_direction = 3;
        }
        
        //player fire checks
        if(bn::keypad::a_held())
        {
            //check if the player is ready to fire another shot
            if(player_shooting_cooldown == 0){
                
                // check if vector is full before creating a new projectile to avoid overflow
                if(!projectiles.full())
                {
                    bn::sprite_ptr projectile_sprite = bn::sprite_items::bullet.create_sprite(player.x(), player.y());
                    projectiles.push_back(playerProjectile(player_direction, projectile_sprite));
                }
                //set shoot cooldown to forty frames to stagger the amount of projectiles
                player_shooting_cooldown = 40;
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
        
        auto enemy_projectile = projectiles.begin();
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
        //decrement frame delay variables if they aren't at 0 already
        if(player_shooting_cooldown != 0){
            player_shooting_cooldown--;
        }
        bn::core::update();
        
    }
}