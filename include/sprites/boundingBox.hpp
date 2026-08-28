//bounding boxes for collision detection with walls and such


//bounding boxes that are meant to be drawn to the screen for visual debugging
class visualBoundingBox{
    public:
    
    int width;
    int height;
    int x;
    int y;
    bn::sprite_ptr sprite;
    visualBoundingBox(int width, int height, int x, int y) : width(width), height(height), x(x), y(y)
    {
        
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