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
