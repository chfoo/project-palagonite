import collections
import json
import io

MenuItem = collections.namedtuple(
    'MenuItem', ['name', 'label', 'icon_map', 'description']
)

def new_menu_item(name, label, icon_map=None, description=None):
    return MenuItem(name, label, icon_map, description) 

item_ = new_menu_item

MENU_ITEMS = (
    item_('empty', ''),
    item_('yes', 'Yes'),
    item_('no', 'No'),
)

def escape(text):
    return json.dumps(text)[1:-1]
    

def main():
    header_buffer = io.StringIO()
    header_buffer.writelines([
        '// This file was automatically generated from menu.py\n',
        '// Do not edit!\n',
        'using namespace Lib::Inc;\n',
        'namespace MenuData {\n'
    ])
    
    header_buffer.write('namespace Strings {\n')
    for menu_item in MENU_ITEMS:
        header_buffer.write(
            'static const char {name}[] PROGMEM = "{label}";\n'.format(
            name=menu_item.name, label=escape(menu_item.label)
        ))
            
        if menu_item.description:
            header_buffer.write(
                'static const char {name}_desc[] PROGMEM = "{description}";\n'
                .format(
                    name=menu_item.name, 
                    description=escape(menu_item.description)
                )
            )
    header_buffer.write('}\n')
    
    header_buffer.write('static const MenuItem MenuItems[] PROGMEM = {\n')
    header_buffer.write('{nullptr, nullptr},\n')  # Placeholder for null/0 item
    for index, menu_item in enumerate(MENU_ITEMS):
        name_text = 'Strings::{}'.format(menu_item.name)
        icon_map_text = menu_item.icon_map if menu_item.icon_map else 'nullptr'
        if menu_item.description:
            description_text = 'Strings::{name}_desc'.format(name=menu_item.name)
        else:
            description_text = 'nullptr'
        header_buffer.writelines([
            '{',
            '{}, {}, {}'.format(name_text, icon_map_text, description_text),
            '},\n'
        ])
    header_buffer.write('};\n')
    
    header_buffer.write('namespace Index {\n')
    for index, menu_item in enumerate(MENU_ITEMS, start=1):
        header_buffer.write('static const unsigned char {name} = {index};\n'
                            .format(name=menu_item.name, index=index))
    header_buffer.write('}\n')
    
    header_buffer.write('}\n')
    
    print(header_buffer.getvalue())


if __name__ == '__main__':
    main()