#include "level_model.hpp"

LevelModel::LevelModel() : player_track_index(0), player_cell_index(0),
        player_cell_sub_index(0), track_ring_index(0) {

}

void LevelModel::move_player_track(uint8_t track_index) {
    this->player_track_index = track_index;
}

void LevelModel::move_player_cell(TrackCell_t cell_index,
        TrackCellSubIndex_t cell_sub_index) {
    this->player_cell_index = cell_index;
    this->player_cell_sub_index = cell_sub_index;
}

void LevelModel::get_track_cell_column(TrackCellIndex_t cell_index,
        TrackCellType (&track_cells)[4]) {
    // TODO: this is a stub
    if (cell_index & 0x01 == 1) {
        track_cells[0] = TrackCellType::ground;
        track_cells[1] = TrackCellType::empty;
        track_cells[2] = TrackCellType::ground;
        track_cells[3] = TrackCellType::ground;
    } else {
        track_cells[0] = TrackCellType::ground;
        track_cells[1] = TrackCellType::ground;
        track_cells[2] = TrackCellType::empty;
        track_cells[3] = TrackCellType::empty;
    }
}
