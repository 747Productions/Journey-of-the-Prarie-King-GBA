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
        }
        if(bn::keypad::left_held())
        {
            player_sprite.set_x(player_sprite.x() - 1);
        }
        
        if(bn::keypad::up_held())
        {
            player_sprite.set_y(player_sprite.y() - 1);
        }
        if(bn::keypad::down_held())
        {
            player_sprite.set_y(player_sprite.y() + 1);   
        }
        //update direction
        if(bn::keypad::up_held() && !bn::keypad::down_held() && bn::keypad::left_held() && !bn::keypad::right_held()){
            direction = 0;
        }
        else if(bn::keypad::up_held() && !bn::keypad::down_held() && !bn::keypad::left_held() && !bn::keypad::right_held()){
            direction = 1;
        }
        else if(bn::keypad::up_held() && !bn::keypad::down_held() && !bn::keypad::left_held() && bn::keypad::right_held()){
            direction = 2;
        }
        else if(!bn::keypad::up_held() && !bn::keypad::down_held() && bn::keypad::left_held() && !bn::keypad::right_held()){
            direction = 3;
        }
        
        else if(!bn::keypad::up_held() && !bn::keypad::down_held() && !bn::keypad::left_held() && bn::keypad::right_held()){
            direction = 4;
        }
        else if(!bn::keypad::up_held() && bn::keypad::down_held() && bn::keypad::left_held() && !bn::keypad::right_held()){
            direction = 5;
        }
        else if(!bn::keypad::up_held() && bn::keypad::down_held() && !bn::keypad::left_held() && !bn::keypad::right_held()){
            direction = 6;
        }
        else if(!bn::keypad::up_held() && bn::keypad::down_held() && !bn::keypad::left_held() && bn::keypad::right_held()){
            direction = 7;
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