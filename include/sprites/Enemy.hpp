class Enemy {
    public:
    bn::sprite_ptr enemy_sprite;
    int type; 
    int height;
    int width;
    
    // move constructor that accepts the sprite smart pointer
    Enemy(int type, bn::sprite_ptr enemy_sprite) : 
    type(type), enemy_sprite(bn::move(enemy_sprite)) 
    {
        this->enemy_sprite.set_scale(.5);
        height = 16; 
        width = 16;
    }
    
    // move constructor
    Enemy(Enemy&& object) noexcept :
    enemy_sprite(bn::move(object.enemy_sprite)),
    type(object.type),
    height(object.height),
    width(object.width)
    {}
    
    //move assignment for safe use in vectors
    Enemy& operator=(Enemy&& object) noexcept {
        if (this != &object) {
            enemy_sprite = bn::move(object.enemy_sprite);
            type = object.type;
            height = object.height;
            width = object.width;
        }
        return *this;
    }
    
    // safe use smart pointer fix that prevents shallow copy pointer issues 
    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;
    
    
    //move
    void move(bn::sprite_ptr playerSprite) {
        //ai for normal enemies pathfinding around walls can be implemented later
        if(type == 0){
            bn::fixed_point player_pos = playerSprite.position();
            bn::fixed_point enemy_pos = enemy_sprite.position();
            if(enemy_pos.x().integer() < player_pos.x().integer()){
                enemy_sprite.set_x(enemy_sprite.x() +.3);
            }
            if(enemy_pos.x().integer() > player_pos.x().integer()){
                enemy_sprite.set_x(enemy_sprite.x() -.3);
                
            }
            if(enemy_pos.y().integer() < player_pos.y().integer()){
                enemy_sprite.set_y(enemy_sprite.y()+.3);
            }           
            if(enemy_pos.y().integer() > player_pos.y().integer()){
                enemy_sprite.set_y(enemy_sprite.y()-.3);
            }
        }
    }
    
    bn::rect get_bounds() const {
        bn::fixed_point pos = enemy_sprite.position();
        return bn::rect(pos.x().integer() - (width / 2), pos.y().integer() - (height / 2), width, height);
    }
};
