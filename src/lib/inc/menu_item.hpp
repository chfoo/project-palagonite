#pragma once

namespace Lib {
namespace Inc {

struct MenuItem {
    const char * label;
    const unsigned char * icon_map;
    const char * description;
};
typedef unsigned char MenuItemID_t;

}
}
