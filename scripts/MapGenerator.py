import json
import os
import subprocess
import re
import numpy as np
import sympy as sp
from scipy.interpolate import interp1d

tiled_background_maker = r"C:\Program Files\Tiled\tmxrasterizer.exe"
tiled_backgrounds_dir = os.path.join(os.path.dirname(__file__), r"..\Resources\Tiled\Backgrounds")
backgrounds_dir = os.path.join(os.path.dirname(__file__), r"..\Resources\Backgrounds")
parallaxs_dir = os.path.join(os.path.dirname(__file__), r"..\Resources\Parallaxs")
map_details_dir = os.path.join(os.path.dirname(__file__), r"..\Resources\Details\Maps")


def create_background_without_image_layers(file_path):
    file_name = str(os.path.splitext(os.path.basename(file_path))[0]) + '.png'
    with open(file_path, 'r', encoding='utf-8') as f:
        data = json.load(f)
    data['layers'] = [layer for layer in data['layers'] if layer['type'] != 'imagelayer']
    temp_file = file_path.replace(".json", "_temp.json")
    with open(temp_file, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2)
    subprocess.run([tiled_background_maker, "--hide-object-layers", temp_file, os.path.join(backgrounds_dir, file_name)])
    os.remove(temp_file)


def catmull_rom_equations(control_points, samples_per_segment=100):
    equations = []

    for i in range(1, len(control_points) - 2):
        p0, p1, p2, p3 = control_points[i - 1:i + 3]

        # Parametric spline function
        t_vals = np.linspace(0, 1, samples_per_segment)

        def spline(p0, p1, p2, p3, t):
            return 0.5 * (
                    2 * p1 +
                    (-p0 + p2) * t +
                    (2 * p0 - 5 * p1 + 4 * p2 - p3) * t ** 2 +
                    (-p0 + 3 * p1 - 3 * p2 + p3) * t ** 3
            )

        x_vals = spline(p0[0], p1[0], p2[0], p3[0], t_vals)
        y_vals = spline(p0[1], p1[1], p2[1], p3[1], t_vals)

        # Check monotonicity in x to allow inversion
        if not np.all(np.diff(x_vals) > 0):
            print(f"Segment {i}: x(t) is not monotonic, skipping equation.")
            continue

        # Fit cubic polynomial: f(x) ≈ y
        f_interp = interp1d(x_vals, y_vals, kind='cubic', fill_value="extrapolate")
        xs_sym = sp.Symbol('x')
        # Use numpy polyfit and convert to sympy expression
        coeffs = np.polyfit(x_vals, y_vals, 3)
        poly_expr = sum(sp.Float(c) * xs_sym ** p for p, c in enumerate(coeffs[::-1]))

        # Print equation
        x_start = round(min(x_vals), 2)
        x_end = round(max(x_vals), 2)
        a = sp.simplify(poly_expr)

        arr = []
        arr.insert(0, float(a.args[0]))
        try:
            arr.insert(a.args[1].args[1].args[1], float(a.args[1].args[0]))
        except:
            arr.insert(1, float(a.args[1].args[0]))
        try:
            arr.insert(a.args[2].args[1].args[1], float(a.args[2].args[0]))
        except:
            arr.insert(1, float(a.args[2].args[0]))
        try:
            arr.insert(a.args[3].args[1].args[1], float(a.args[3].args[0]))
        except:
            arr.insert(1, float(a.args[3].args[0]))
        arr.append(x_start)
        arr.append(x_end)

        equations.append(arr)

    return equations


def get_platform_equations(platforms):
    result = []
    for platform in platforms:
        equations = []
        for i in range(len(platform) - 1):
            m = (platform[i][1] - platform[i + 1][1]) / (platform[i][0] - platform[i + 1][0])
            n = platform[i][1] - m * (platform[i][0])
            equations.append([m, n, platform[i][0], platform[i + 1][0]])
        result.append(equations)
    return result


def get_objects_from_map(map_data):
    map_details = map_data['layers']
    for layer in map_details:
        if layer['name'] == 'objects':
            map_objects = layer['objects']
            break
    objects = {'Platforms': [], 'Climbables': [], 'Walls': [], 'Curves': {}, "Portals": []}
    curves = {}
    platforms = []
    for map_object in map_objects:
        if map_object['name'].startswith('platform'):
            x = int(map_object['x'])
            y = int(map_object['y'])
            platform = []
            for dot in map_object['polyline']:
                platform.append([x + int(dot['x']),
                                 y + int(dot['y'])])
            platforms.append(platform)
        elif map_object['name'].startswith('curve'):
            curve_num = int(map_object['name'][len('curve'):])
            if curve_num not in curves:
                curves[curve_num] = []
            curves[curve_num].append([map_object['x'], map_object['y']])
        elif map_object['name'].startswith('climbable'):
            objects['Climbables'].append({"ClimbableID": 1,
                                          "Pos": [int(map_object['x']), int(map_object['y'])],
                                          "Size": int(map_object['polyline'][1]['y'])})
        elif map_object['name'].startswith('wall'):
            objects['Walls'].append({"Pos": [int(map_object['x']), int(map_object['y'])],
                                    "Size": int(map_object['polyline'][1]['y'])})
        elif map_object['name'].startswith('portal'):
            map_id = int(map_object['name'][len('portal'):])
            objects['Portals'].append({"PortalID": 1,
                                       "Pos": [int(map_object['x']), int(map_object['y'])],
                                       "MapID": map_id})

    for key, curve in curves.items():
        curve_equations = catmull_rom_equations(curve)
        objects['Curves'][key] = curve_equations
    objects['Platforms'] = get_platform_equations(platforms)

    return objects


def get_parallax_images(tiled_map):
    parallaxs_data = [layer for layer in tiled_map['layers'] if layer['type'] == 'imagelayer']
    parallaxs = []
    for parallax in parallaxs_data:
        if not parallax["visible"]:
            continue
        x = parallax["offsetx"] if "offsetx" in parallax else 0
        y = parallax["offsety"] if "offsety" in parallax else 0
        parallaxs.append({"Pos": [x, y],
                          "Factor": parallax["parallaxx"] if "parallaxx" in parallax else 1,
                          "Image": int(re.search(r"(\d+)\.png", os.path.basename(parallax["image"])).group(1)),
                          "Repeated": parallax["repeatx"] if "repeatx" in parallax else False})
    return parallaxs


def modify_map_objects(map_details, tiled_map):
    objects = get_objects_from_map(tiled_map)
    map_details['Platforms'] = objects['Platforms']
    map_details['Walls'] = objects['Walls']
    map_details['Climbables'] = objects['Climbables']
    map_details['Curves'] = objects['Curves']
    map_details['Portals'] = objects['Portals']
    map_details['Parallax'] = get_parallax_images(tiled_map)


def create_map(file_path):
    create_background_without_image_layers(file_path)
    map_details_path = os.path.join(map_details_dir, re.sub(r'^[^\d]+', 'map', os.path.basename(file_path)))
    with open(map_details_path, "rb") as f:
        map_details = json.load(f)
    with open(file_path, "rb") as f:
        tiled_map = json.load(f)
    modify_map_objects(map_details, tiled_map)

    with open(map_details_path, "w") as f:
        json.dump(map_details, f, indent=2)


def main():
    for file_path in os.listdir(tiled_backgrounds_dir):
        if file_path.endswith('.json'):
            create_map(os.path.join(tiled_backgrounds_dir, file_path))


if __name__ == '__main__':
    main()
