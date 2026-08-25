class playerProjectile
{
    public:
    int direction;
    bn::sprite_ptr projectile_sprite;
    int width = 4;
    int height = 4;
    
    playerProjectile( bn::sprite_ptr sprite,int direction) :
    direction(direction),
    projectile_sprite(bn::move(sprite))
    {
        
        projectile_sprite.set_scale(.5);
    }
    //0 1 2
    //3 X 4
    //5 6 7
    // Move constructor
    playerProjectile(playerProjectile&& object) noexcept :
    direction(object.direction),
    projectile_sprite(bn::move(object.projectile_sprite)),
    width(object.width),
    height(object.height)
    {
        
    }
    
    // Move assignment
    playerProjectile& operator=(playerProjectile&& object) noexcept {
        if (this != &object) {
            direction = object.direction;
            projectile_sprite = bn::move(object.projectile_sprite);
            width = object.width;
            height = object.height;
        }
        return *this;
    }
    
    // fix to allow the safe use of smart pointer as object properties
    playerProjectile(const playerProjectile&) = delete;
    playerProjectile& operator=(const playerProjectile&) = delete;
    
    void move(){
        switch(direction){
            case 0:
            projectile_sprite.set_x(projectile_sprite.x()-1);
            projectile_sprite.set_y(projectile_sprite.y()-1);
            break;
            
            case 1:
            projectile_sprite.set_y(projectile_sprite.y()-1);
            break;
            
            case 2:
            projectile_sprite.set_x(projectile_sprite.x()+1);
            projectile_sprite.set_y(projectile_sprite.y()-1);
            break;
            
            case 3:
            projectile_sprite.set_x(projectile_sprite.x()-1);
            break;
            
            case 4:
            projectile_sprite.set_x(projectile_sprite.x()+1);
            break;
            
            case 5:
            projectile_sprite.set_x(projectile_sprite.x()-1);
            projectile_sprite.set_y(projectile_sprite.y()+1);
            break;
            case 6:
            projectile_sprite.set_y(projectile_sprite.y()+1);
            break;
            case 7:
            projectile_sprite.set_x(projectile_sprite.x()+1);
            projectile_sprite.set_y(projectile_sprite.y()+1);
            break;
            default:
            break;
            
        }
    }
    //0 1 2
    //3 X 4
    //5 6 7
    bool is_off_screen() {
        return (projectile_sprite.x() > 128 || projectile_sprite.x() < -128 || 
        projectile_sprite.y() > 88  || projectile_sprite.y() < -88);
    }
    
    bn::rect get_bounds() const {
        bn::fixed_point pos = projectile_sprite.position();
        return bn::rect(pos.x().integer() - (width / 2), pos.y().integer() - (height / 2), width, height);
    }
};
