import argparse
import sys
import json
import collections
from typing import List
import io
import itertools
import os
import glob

import PIL.Image
import PIL.ImageOps

def grouper(iterable, n, fillvalue=None):
    "Collect data into fixed-length chunks or blocks"
    # grouper('ABCDEFG', 3, 'x') --> ABC DEF Gxx"
    args = [iter(iterable)] * n
    return itertools.zip_longest(*args, fillvalue=fillvalue)


def generate_palette():
    for index in range(0, 256):
        red = ((index >> 0) & 7) * 255 // 7
        green = ((index >> 3) & 7) * 255 // 7
        blue = ((index >> 5) & 6) * 255 // 7

        yield (red, green, blue)


TRANSPARENT_COLOR = 0xdaffda

class TileAdder:
    def __init__(self, image_tile_indexes: List[List[int]], 
                 tile_names: dict, tile_maps: dict):
        self._image_tile_indexes = image_tile_indexes
        self.tile_names = tile_names
        self.tile_maps = tile_maps
    
    def add_tile(self, name: str, row: int, col: int):
        if name in self.tile_names:
            raise ValueError('Tile {} already defined'.format(name))
        
        try:
            self.tile_names[name] = self._image_tile_indexes[row][col]
        except IndexError as error:
            raise ValueError('tile {} location not in range'.name) from error
    
    def add_map(self, name: str, row: int, col: int, width: int, height: int):
        if name in self.tile_maps:
            raise ValueError('Tile {} already defined'.format(name))
        
        indexes = []
        
        for row_index in range(row, row + height):
            for col_index in range(col, col + width):
                try:
                    indexes.append(self._image_tile_indexes[row_index][col_index])
                except IndexError as error:
                     raise ValueError('map {} sizes not in range'
                                      .format(name)) from error
        
        self.tile_maps[name] = (width, height, indexes)
    
class TileBank:
    TILE_WIDTH = 8
    TILE_HEIGHT = 8
    
    def __init__(self, name: str):
        self.name = name
        self.tiles = []
        self.data_to_index_map = {}
        self.tile_names = {}
        self.tile_maps = {}
    
    def add_image(self, image: PIL.Image.Image, contiguous: bool=False) -> TileAdder:
        tile_images = self._slice_image(image)
        tile_indexes = []
        
        for row_tiles in tile_images:
            tile_index_row = []
            for tile_image in row_tiles:
                index = self._add_tile_image(tile_image, contiguous)
                tile_index_row.append(index)
            
            tile_indexes.append(tile_index_row)
        
        return TileAdder(tile_indexes, self.tile_names, self.tile_maps)
    
    def _add_tile_image(self, image: PIL.Image.Image, contiguous: bool=False) -> int:
        data = bytes(image.getdata())
        assert len(data) == self.TILE_WIDTH * self.TILE_HEIGHT
        
        if data not in self.data_to_index_map or contiguous:
            self.tiles.append(data)
            index = len(self.tiles) - 1
            self.data_to_index_map[data] = index
            return index
        else:
            return self._get_tile_image_index(image)
    
    def _get_tile_image_index(self, image: PIL.Image.Image) -> int:
        data = bytes(image.getdata())
        return self.data_to_index_map[data]
    
    @classmethod
    def _slice_image(cls, image: PIL.Image.Image) -> List[List[PIL.Image.Image]]:
        assert image.width % cls.TILE_WIDTH == 0, 'width not divisible'
        assert image.height % cls.TILE_HEIGHT == 0, 'height not divisible'
        
        image_tiles = []
        
        for row in range(image.height // cls.TILE_HEIGHT):
            row_tiles = []
            
            for col in range(image.width // cls.TILE_WIDTH):
                x1 = col * cls.TILE_WIDTH
                y1 = row * cls.TILE_HEIGHT
                x2 = x1 + cls.TILE_WIDTH
                y2 = y1 + cls.TILE_HEIGHT
                tile_image = image.crop((x1, y1, x2, y2))
                
                row_tiles.append(tile_image)
            
            image_tiles.append(row_tiles)
        
        return image_tiles


class TilesheetMaker:
    DEFAULT_BANK_NAME = 'main'
    
    def __init__(self, input_dirs: list):
        self._input_dirs = input_dirs

        self._image_infos = {}

        palette_values = bytes(itertools.chain(*generate_palette()))

        self._image_with_palette = PIL.Image.new('P', (1, 1))
        self._image_with_palette.putpalette(palette_values)

    @classmethod
    def _print(cls, *args, **kwargs):
        print(*args, file=sys.stderr)

    def run(self):
        self._load_images()
        
        banks = self._populate_banks()
        
        self._print('Loaded', len(banks), 'banks.')
        
        self._print('Generating header file')
        
        header_buffer = io.StringIO()
        header_buffer.writelines([
            '// This file was automatically generated from tilesheet.py\n',
            '// Do not edit!\n',
            '#include <avr/pgmspace.h>\n',
            'namespace TilesheetData {\n',
        ])
        
        for bank in banks:
            header_buffer.write('namespace ')
            header_buffer.write(bank.name)
            header_buffer.write(' {\n')
            header_buffer.write('namespace tiles {\n')
            
            for name, index in sorted(bank.tile_names.items()):
                header_buffer.write('static const unsigned char {} = {};\n'.format(name, index))
            
            header_buffer.write('}\n')
            header_buffer.write('namespace maps {\n')
            
            for name, info in sorted(bank.tile_maps.items()):
                width, height, indexes = info
                header_buffer.write('static const unsigned char {}_width = {};\n'.format(name, info[0]))
                header_buffer.write('static const unsigned char {}_height = {};\n'.format(name, info[1]))
                header_buffer.write('static const unsigned char {}[] PROGMEM = {{\n'.format(name))
                header_buffer.write('{}, {},\n'.format(width, height))
                header_buffer.write(', '.join(hex(index) for index in indexes))
                header_buffer.write('\n};\n')
            
            header_buffer.write('}\n')
            
            header_buffer.write('static const unsigned char tile_data[] PROGMEM = {\n')
            
            for tile_id, data in enumerate(bank.tiles):
                header_buffer.write('// tile {}\n'.format(tile_id))
                
                for data_row in grouper(data, TileBank.TILE_WIDTH):
                    header_buffer.write(', '.join(hex(value) for value in data_row))
                    header_buffer.write(',\n')
                
            header_buffer.write('};\n')
        
        header_buffer.writelines([
            '}\n',
            '}\n',
        ])
        
        print(header_buffer.getvalue())
            
    def _load_images(self):
        for input_dir in self._input_dirs:
            for config_filename in glob.glob(input_dir + '/*.json'):
                config_name = os.path.splitext(os.path.basename(config_filename))[0]

                self._print('Loading', config_name)

                image_filename = os.path.join(
                    input_dir, config_name + '.png'
                )

                with open(config_filename) as file:
                    config = json.load(file)

                image = PIL.Image.open(image_filename)
                
                if image.mode == 'RGBA':
                    image = self._apply_8bit_transparency(image)
                    image = self._quantize_image(image)
                
                assert image.mode == 'P', image.mode

                self._image_infos[config_name] = {
                    'config': config,
                    'image': image
                }
    
    def _apply_8bit_transparency(self, image: PIL.Image.Image) -> PIL.Image.Image:
        image = image.copy()
        alpha_band = image.split()[3]
        alpha_band = PIL.ImageOps.invert(alpha_band)
        transparency_color = PIL.Image.new('RGB', (image.width, image.height), color=TRANSPARENT_COLOR)
        image.paste(transparency_color, mask=alpha_band)
        return image
    
    def _quantize_image(self, image: PIL.Image.Image) -> PIL.Image.Image:
        image = image.convert('RGB')
        return image.quantize(palette=self._image_with_palette)

    def _populate_banks(self) -> List[TileBank]:
        banks = []
        bank_names = set()
        
        for image_info in self._image_infos.values():
            bank_names.update(image_info['config'].get('banks', [self.DEFAULT_BANK_NAME]))
        
        for bank_name in sorted(bank_names):
            self._print('Processing bank', bank_name)
            bank = TileBank(bank_name)
            banks.append(bank)
            
            names = sorted(
                self._image_infos.keys(), 
                key=lambda key: (self._image_infos[key]['config'].get('priority', 0), key)
            )
            
            for name in names:
                image_info = self._image_infos[name]
                config = image_info['config']
                image = image_info['image']
                
                adder = bank.add_image(image, config.get('contiguous', False))
                
                for name, info in config.get('tiles', {}).items():
                    adder.add_tile(name, info['row'], info['col'])
                
                for name, info in config.get('maps', {}).items():
                    adder.add_map(name, info['row'], info['col'], 
                                  info['width'], info['height'])
            
            self._print('Collected', len(bank.tiles), 'unique tiles.')
            
        return banks

def main():
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('input_image_dir', nargs='+')
    
    args = arg_parser.parse_args()
    
    maker = TilesheetMaker(args.input_image_dir)
    maker.run()

if __name__ == '__main__':
    main()
