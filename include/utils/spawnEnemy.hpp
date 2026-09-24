void spawnEnemy(bn::vector<Enemy, 30>& enemies, bn::random& rng,int amount) {
    //choose which side of the screen to spawn an enemy on and spawn it at a random location on that side
    //check if creating a new enemy would exceed the vector's capacity
    if(enemies.size() + amount > enemies.max_size()){
        //if it would exceed the capacity, only spawn enough enemies to fill the vector to its max size
        amount = enemies.max_size() - enemies.size();
        return; //exit the function early to avoid exceeding the vector's capacity
    }
    for(int i = 0;i<amount;i++){
        int side = rng.get_int(0, 3);
        
        switch(side){
            case 0:
            //spawn on left side of screen
            enemies.emplace_back(Enemy(0, bn::sprite_items::player.create_sprite(-120,0)));
            break;
            case 1:
            //spawn on right side of screen
            enemies.emplace_back(Enemy(0, bn::sprite_items::player.create_sprite(120, 0)));
            break;
            case 2:
            //spawn on top side of screen
            enemies.emplace_back(Enemy(0, bn::sprite_items::player.create_sprite(0, -80)));
            break;
            case 3:
            //spawn on bottom side of screen
            enemies.emplace_back(Enemy(0, bn::sprite_items::player.create_sprite(0, 80)));
            break;
            default:
            //default to spawning on left side of screen if logic fails for some reason
            enemies.emplace_back(Enemy(0, bn::sprite_items::player.create_sprite(-120, rng.get_int(-80, 80))));
            break;
        };
    }
    
}