#pragma once

#include <stdint.h>

typedef uint8_t TrackCell_t;
typedef uint16_t TrackCellIndex_t;
typedef uint8_t TrackCellSubIndex_t;

enum class TrackCellType : TrackCell_t {
    empty = 0,
    ground = 1,
};


class LevelModel {
public:
    static const uint8_t NUM_TRACKS = 4;
    static const uint8_t TRACK_WIDTH = 16;
    uint8_t track_data[NUM_TRACKS][TRACK_WIDTH] = {};
    uint8_t player_track_index;
    TrackCellIndex_t player_cell_index;
    TrackCellSubIndex_t player_cell_sub_index;

protected:
    uint8_t track_ring_index;

public:
    LevelModel();

    void move_player_track(uint8_t track_index);
    void move_player_cell(TrackCell_t cell_index, TrackCellSubIndex_t cell_sub_index);
    void get_track_cell_column(TrackCellIndex_t cell_index, TrackCellType (&track_cells)[4]);
};
