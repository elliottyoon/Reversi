#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height})
{
    // TODO: initialize `next_moves_` to `turn_`'s available moves
    compute_next_moves_();
}

Model::Rectangle
Model::board() const
{
    return board_.all_positions();
}

Player
Model::operator[](Position pos) const
{
    return board_[pos];
}

const Move*
Model::find_move(Position pos) const
{
    auto i = next_moves_.find(pos);

    if (i == next_moves_.end()) {
        // Nothing was found, so return NULL (nullptr in C++)
        return nullptr;
    } else {
        // Dereferences the iterator to get the value then returns a pointer
        // to that value. This is safe as the pointer is to a value living
        // within the `next_moves_` structure.
        return &(*i);
    }
}

void
Model::play_move(Position pos)
{
    if (is_game_over()) {
        throw Client_logic_error("Model::play_move: game over");
    }

    const Move* movep = find_move(pos);
    if (movep == nullptr) { // check if there was no such move
        throw Client_logic_error("Model::play_move: no such move");
    }

    // TODO: actually execute the move, advance the turn, refill
    // `next_moves_`, etc.
    // actually execute the move
    really_play_move_(*movep);
    // advance the turn
    advance_turn_();
    // refill next_moves_
    compute_next_moves_();
}

//
// BELOW ARE HELPER FUNCTIONS
// Our tests will run on the public functions, not the helper functions
// However, these will be extremely useful in writing the above functions
//

Position_set
Model::find_flips_(Position current, Dimensions dir) const
{
    // TODO: HELPER FUNCTION

    // Create an empty position set and start adding positions to it
    // one by one as long as the position is in bounds and contains the
    // opposite player
    Position_set pset = {};
    Position p = current;
    p.x = p.x + dir.width;
    p.y = p.y + dir.height;

    // iterate in specified direction until either
    // 1. go out of bounds
    // 2. reach a position that does not contain the other player
    /*while (board_.good_position(p) && board_[p] == other_player(board_
    [current])) { this would find all flips for both players i think. */
    while (board_.good_position(p) && board_[p] == other_player(turn())) {
        pset[p] = true;
        p.x = p.x + dir.width;
        p.y = p.y + dir.height;
    }
    return pset;
}

Position_set
Model::evaluate_position_(Position pos) const
{
    // TODO: HELPER FUNCTION

    // possible positions
    Position_set pset = {};

    // loop over each board direction
    for (auto dim : Board::all_directions()) {
        // determine the positions that would flip and keep track
        // of these positions in a position set object as you iterate
        pset|=find_flips_(pos, dim);
    }
    // if at least one viable move, add the initial input position
    if (!pset.empty()) {
        pset[pos] = true;
    }
    return pset;
}

void
Model::compute_next_moves_()
{
    // TODO: HELPER FUNCTION
    // clears out previously computed moves
    next_moves_.clear();

    // indicates phase of play
    int is_opening = 0;

    // determines phase of the game
    for (auto pos: board_.center_positions()) {
        if (board_[pos] == Player::neither) {
            is_opening = 1;
            break;
        }
    }

    // opening phase
    if (is_opening == 1) {
        // adds all unoccupied position to next_moves_
        for (auto pos: board_.center_positions()) {
            if (board_[pos] == Player::neither) {
                next_moves_[pos] = {pos};
            }
        }
    }
    // main phase
    else {
        for (auto pos : board_.all_positions()) {
            if (!evaluate_position_(pos).empty()) {
                next_moves_[pos] = evaluate_position_(pos);
            }
        }
    }
}

bool
Model::advance_turn_()
{
    // TODO: HELPER FUNCTION

    // switch to the other player
    turn_ = other_player(turn_);

    // compute the set of viable moves for the new player
    compute_next_moves_();

    // return if any moves are available
    if (next_moves_.empty()) {
        return false;
    } else {
        return true;
    }
}

void
Model::set_game_over_()
{
    // TODO: HELPER FUNCTION
    int light_tiles = 0;
    int dark_tiles = 0;

    // set player to neither
    turn_ = Player::neither;

    // count the number of tiles occupied by each player
    for (auto pos : board_.all_positions()) {
        if (board_[pos] == Player::dark) {
            dark_tiles++;
        }
        else if (board_[pos] == Player::light) {
            light_tiles++;
        }
    }
    // and set the winner accordingly
    if (light_tiles > dark_tiles) {
        winner_ = Player::light;
    } else if (dark_tiles > light_tiles) {
        winner_ = Player::dark;
    }
}

void
Model::really_play_move_(Move move)
{
    // TODO: HELPER FUNCTION
    // sets each position gained by the move to the current player
    for (auto pos : move.second) {
        board_[pos] = turn();
    }
    // check for end of game
    if (advance_turn_() == false) {
        if (advance_turn_() == false) {
            set_game_over_();
        }
    }

}
