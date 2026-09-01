//bounding boxes for collision detection with walls and such


//bounding boxes that are meant to be drawn to the screen for visual debugging
class visualBoundingBox{
    public:

    int x;
    int y;
    bn::sprite_ptr p1;
    bn::sprite_ptr p2;
    bn::sprite_ptr p3;
    bn::sprite_ptr p4;
    bn::rect bounds;
    visualBoundingBox(int width, int height, int x, int y) : x(x), y(y)
    {
        p1 = bn::sprite_items::bb.create_sprite(x - width/2, y - height/2);
        p2 = bn::sprite_items::bb.create_sprite(x + width/2, y - height/2);
        p3 = bn::sprite_items::bb.create_sprite(x + width/2, y + height/2);
        p4 = bn::sprite_items::bb.create_sprite(x - width/2, y + height/2);
        bounds = bn::rect(x-(width/2), y-(height/2), width, height);
    }
};

//bounding boxes that arent meant to actually be drawn to the screen, just for collision detection
class boundingBox{
    public:
    
    int width;
    int height;
    int x;
    int y;
    bn::rect bounds;
    boundingBox(int width, int height, int x, int y) : width(width), height(height), x(x), y(y)
    {
        width = width;
        height = height;
        x = x;
        y = y;
        bounds = bn::rect(x-(width/2), y-(height/2), width, height);
    }

};