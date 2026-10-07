void updateLabels(bn::vector<bn::sprite_ptr, 10>& text_sprites, bn::sprite_text_generator text_generator,int lives, int score) {
    //clear sprite vector in order to replace the text with updated values
    text_sprites.clear();
    // 1. Create a persistent named buffer
    bn::string<32> score_string;
    bn::string<32> lives_string;
    // 2. Pass the buffer by reference into the stream
    bn::ostringstream score_stream(score_string);
    bn::ostringstream lives_stream(lives_string);
    // 3. Populate the stream
    score_stream << "SCORE: " << score;
    lives_stream << "LIVES: " << lives;

    // 4. Wipe old sprites and generate new ones (using the buffer we just filled)
    text_generator.generate(-120, -60, score_string, text_sprites);
    text_generator.generate(-30, -60, lives_string, text_sprites);
}