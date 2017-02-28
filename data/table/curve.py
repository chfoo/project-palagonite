import itertools

import scipy.interpolate


def cubicbezier(x0, y0, x1, y1, x2, y2, x3, y3, t):
    # https://rosettacode.org/wiki/Bitmap/B%C3%A9zier_curves/Cubic#Python
    a = (1. - t) ** 3
    b = 3.0 * t * (1.0 - t) ** 2
    c = 3.0 * t ** 2 * (1.0 - t)
    d = t ** 3
    
    x = a * x0 + b * x1 + c * x2 + d * x3
    y = a * y0 + b * y1 + c * y2 + d * y3
    
    return x, y


def main():
    # Knot points from http://easings.net/
    ease_in_out_points = ((0, 0), (0.45, 0.03), (0.51, 0.96), (1, 1))
    knot_list = tuple(itertools.chain(*ease_in_out_points))
    values = []
    
    for table_t in range(256):
        input_t = table_t / 256
        output_x, output_y = cubicbezier(*knot_list, input_t)
        table_y = int(output_y * 256)
        values.append(table_y)
        
    print('static const uint8_t EASE_IN_OUT_LOOKUP_TABLE_U8[256] PROGMEM = {')
    for index, value in enumerate(values):
        print('0x{:02x}, '.format(value), end='\n' if index % 8 == 7 else '')
        
    print('};')

if __name__ == '__main__':
    main()