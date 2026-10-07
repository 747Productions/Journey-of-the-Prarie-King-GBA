bool coordInside(bn::rect bb, bn::fixed x, bn::fixed y) {
    return (x >= bb.left() && x <= bb.right() && y >= bb.top() && y <= bb.bottom());
}