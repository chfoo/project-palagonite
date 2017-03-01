#include "level_model.hpp"
#include "lib/util.hpp"

LevelModel::LevelModel() : player_track_index(0), player_cell_index(0),
        track_ring_cell_index(0) {

    // TODO: generate real/good tracks
    for (uint8_t track_index = 0; track_index < this->NUM_TRACKS; track_index++) {
        for (TrackCell_t cell_index = 0; cell_index < this->TRACK_WIDTH; cell_index++) {
            TrackCellType type = (TrackCellType) Lib::Util::rand_int(0, 3);
            this->track_data[track_index][cell_index] = type;
        }
    }
}

void LevelModel::move_player_track(uint8_t track_index) {
    this->player_track_index = track_index;
}

void LevelModel::move_player_next_cell() {
    this->player_cell_index += 1;
    this->track_ring_cell_index += 1;
    this->track_ring_cell_index %= this->TRACK_WIDTH;
}

TrackCellType LevelModel::get_track_cell(uint8_t track_index, TrackCell_t cell_index) {
    TrackCell_t computed_cell_index = (this->track_ring_cell_index +
            cell_index - this->player_cell_index) % this->TRACK_WIDTH;
    return this->track_data[track_index][computed_cell_index];
}

void LevelModel::get_track_cell_column(TrackCellIndex_t cell_index,
        TrackCellType (&track_cells)[4]) {
    TrackCell_t computed_cell_index = (this->track_ring_cell_index +
            cell_index - this->player_cell_index) % this->TRACK_WIDTH;
    track_cells[0] = this->track_data[0][computed_cell_index];
    track_cells[1] = this->track_data[1][computed_cell_index];
    track_cells[2] = this->track_data[2][computed_cell_index];
    track_cells[3] = this->track_data[3][computed_cell_index];
}

bool LevelModel::is_next_cell_traversable() {
    return this->get_track_cell(this->player_track_index,
            this->player_cell_index + 1) != TrackCellType::empty;
}

bool LevelModel::is_track_traversable(uint8_t track_index) {
    return this->get_track_cell(track_index,
            this->player_cell_index) != TrackCellType::empty;
}

bool LevelModel::is_player_collided() {
    return this->get_track_cell(this->player_track_index,
            this->player_cell_index) == TrackCellType::obstacle;
}
