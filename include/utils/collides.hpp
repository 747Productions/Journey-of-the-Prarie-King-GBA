bool collides(bn::rect b1, bn::rect b2){
    return b1.x() < b2.x() + b2.width() &&
    b1.x() + b1.width() > b2.x() &&
    b1.y() < b2.y() + b2.height() &&
    b1.y() + b1.height() > b2.y();
}