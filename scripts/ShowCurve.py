import numpy as np
import sympy as sp
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import json


def catmull_rom_equations(control_points, samples_per_segment=100):
    equations = []
    plot_xs = []
    plot_ys = []

    for i in range(1, len(control_points) - 2):
        p0, p1, p2, p3 = control_points[i-1:i+3]

        # Parametric spline function
        t_vals = np.linspace(0, 1, samples_per_segment)
        def spline(p0, p1, p2, p3, t):
            return 0.5 * (
                2*p1 +
                (-p0 + p2) * t +
                (2*p0 - 5*p1 + 4*p2 - p3) * t**2 +
                (-p0 + 3*p1 - 3*p2 + p3) * t**3
            )

        x_vals = spline(p0[0], p1[0], p2[0], p3[0], t_vals)
        y_vals = spline(p0[1], p1[1], p2[1], p3[1], t_vals)

        # Save points for plotting
        plot_xs.extend(x_vals)
        plot_ys.extend(y_vals)

        # Check monotonicity in x to allow inversion
        if not np.all(np.diff(x_vals) > 0):
            print(f"Segment {i}: x(t) is not monotonic, skipping equation.")
            continue

        # Fit cubic polynomial: f(x) ≈ y
        f_interp = interp1d(x_vals, y_vals, kind='cubic', fill_value="extrapolate")
        xs_sym = sp.Symbol('x')
        # Use numpy polyfit and convert to sympy expression
        coeffs = np.polyfit(x_vals, y_vals, 3)
        poly_expr = sum(sp.Float(c) * xs_sym**p for p, c in enumerate(coeffs[::-1]))

        # Print equation
        x_start = round(min(x_vals), 2)
        x_end = round(max(x_vals), 2)
        print(f"Segment {i}: x in [{x_start}, {x_end}]")
        print(f"f(x) = {sp.simplify(poly_expr)}\n")

        equations.append((x_vals, y_vals))

    return plot_xs, plot_ys


def get_points():
    with open(r"C:\Users\eli98\personal\projetcs\VisualStudio\Etherfall\Resources\Tiled\Backgrounds\Background1.json", "rb") as f:
        background_data = json.load(f)
    background_details = background_data['layers']
    for layer in background_details:
        if layer['name'] == 'objects':
            background_objects = layer['objects']
            break
    curves = {}
    for map_object in background_objects:

        if map_object['name'].startswith('curve'):
            curve_num = int(map_object['name'][len('curve'):])
            if curve_num not in curves:
                curves[curve_num] = []
            curves[curve_num].append([map_object['x'], map_object['y']])

    return curves[3]


def main():
    control_points = get_points()
    # Generate spline and print equations
    xs, ys = catmull_rom_equations(control_points)

    # Plot
    plt.figure("Catmull-Rom Spline")
    plt.plot(xs, ys, 'b-', label="Spline")
    # plt.plot(*zip(*control_points), 'ro-', label="Control Points")
    plt.gca().invert_yaxis()  # Invert Y-axis for SFML-like coordinate system
    plt.legend()
    plt.xlabel("X")
    plt.ylabel("Y (down)")
    plt.title("Catmull-Rom Spline with f(x) Equations")
    plt.grid(True)
    plt.show()


if __name__ == '__main__':
    main()