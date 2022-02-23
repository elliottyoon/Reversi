#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 36;

View::View(Model const& model)
        : model_(model)
        // You may want to add sprite initialization here
        , tile_sprite{{grid_size - (grid_size / 10), grid_size - (grid_size
        / 10)}, Color::medium_green()}
        , light_sprite{{grid_size / 2}, Color::black()}
        , dark_sprite{grid_size / 2, Color::medium_cyan()}
{ }

void View::draw(Sprite_set& set)
{
    // adds grid boxes
    for (Position posn : model_.board()) {
        set.add_sprite(tile_sprite, board_to_screen(posn), 0);
    }

    // current turn

    // viable vs invalid moves




}

View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    return grid_size * model_.board().dimensions();
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Reversi";
}

View::Position
View::board_to_screen(Model::Position logical) const {
    View::Position p = {logical.x * grid_size , logical.y * grid_size};
    return p;
}

Model::Position
View::screen_to_board(View::Position physical) const {
    Model::Position p = {physical.x / grid_size, physical.y / grid_size};
    return p;
}

void
View::add_player_sprite_ (
        ge211::Sprite_set& sprites,
        Player            which,
        ge211::Posn<int>  physical,
        int               z_layer) const {
    // end of game
    if (model_.turn() == Player::neither) {
        // turn non-winning players' tiles gray
        for (Position posn : model_.board()) {
            if (model_[posn] == which && which == Player::dark) {
                    sprites.add_sprite(dark_sprite, physical, z_layer + 1);
            }
            else if (model_[posn] == which && which == Player::light) {
                sprites.add_sprite(light_sprite, physical, z_layer + 1);
            }
        }
    }
    // normal turn
    else {
        if (which == Player::dark) {
            sprites.add_sprite(dark_sprite, physical, z_layer);
        }
        else {
            sprites.add_sprite(light_sprite, physical, z_layer);
        }
    }

}




