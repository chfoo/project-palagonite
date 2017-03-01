#pragma once

#include <stdint.h>

typedef uint8_t TrackCell_t;
typedef uint16_t TrackCellIndex_t;

enum class TrackCellType : TrackCell_t {
    empty = 0,
    ground = 1,
    obstacle = 2,
    treasure = 3,
};


class LevelModel {
public:
    static const uint8_t NUM_TRACKS = 4;
    static const uint8_t TRACK_WIDTH = 16;
    TrackCellType track_data[NUM_TRACKS][TRACK_WIDTH] = {};
    uint8_t player_track_index;
    TrackCellIndex_t player_cell_index;

protected:
    uint8_t track_ring_cell_index;

public:
    LevelModel();

    void move_player_track(uint8_t track_index);
    void move_player_next_cell();
    TrackCellType get_track_cell(uint8_t track_index, TrackCell_t cell_index);
    void get_track_cell_column(TrackCellIndex_t cell_index, TrackCellType (&track_cells)[4]);
    bool is_next_cell_traversable();
    bool is_track_traversable(uint8_t track_index);
    bool is_player_collided();
};
