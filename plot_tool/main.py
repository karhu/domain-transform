import numpy as np
import re
import matplotlib.pyplot as plt
import matplotlib.cm as matplotlib_cm
from matplotlib.font_manager import FontProperties

from PIL import Image
import pickle

#REPORT_FILE = "run_giauzar.local_13_05_01__17_04.pickle"
REPORT_FILE = "run_giauzar.local_13_05_02__15_03.pickle"
f = open(REPORT_FILE, "r")
DATA = pickle.load(f)

BRANCHES = ['master', 'inplace_image_transform']
#DATA = {'local': {'results': [{'total_cycles': 109811000.0, 'benchmark_warmup': 0, 'description': 'NC (Normalized Convolution)', 'sigma_s': 20, 'sigma_r': 0.8, 'sum': [81938.5, 123906, 150657], 'iterations': 2, 'runtime_data': [{'total_cycles': 109789000.0, 'percentage': 100, 'cycles_call': 109789000.0, 'data': 'all', 'calls': 1}, {'total_cycles': 5712450.0, 'percentage': 5.20311, 'cycles_call': 5712450.0, 'data': 'diffX', 'calls': 1}, {'total_cycles': 5469500.0, 'percentage': 4.98182, 'cycles_call': 5469500.0, 'data': 'diffY', 'calls': 1}, {'total_cycles': 5415030.0, 'percentage': 4.93221, 'cycles_call': 5415030.0, 'data': 'domainTransform', 'calls': 1}, {'total_cycles': 1781570.0, 'percentage': 1.62272, 'cycles_call': 1781570.0, 'data': 'transpose_float', 'calls': 1}, {'total_cycles': 15055000.0, 'percentage': 13.7126, 'cycles_call': 3763750.0, 'data': 'transpose_rgb', 'calls': 4}, {'total_cycles': 1224380.0, 'percentage': 1.11521, 'cycles_call': 612190, 'data': 'cumsumX', 'calls': 2}, {'total_cycles': 39008200.0, 'percentage': 35.5301, 'cycles_call': 9752040.0, 'data': 'BoxFilterBounds', 'calls': 4}, {'total_cycles': 13188300.0, 'percentage': 12.0124, 'cycles_call': 3297070.0, 'data': 'computeRowSAT', 'calls': 4}, {'total_cycles': 18221100.0, 'percentage': 16.5964, 'cycles_call': 4555280.0, 'data': 'boxFilter', 'calls': 4}], 'benchmark_iterations': 1, 'input': './testimages/bhudda_640.png', 'cycles': 109811000.0, 'method': 'nc'}, {'total_cycles': 372372000.0, 'benchmark_warmup': 0, 'description': 'NC (Normalized Convolution)', 'sigma_s': 20, 'sigma_r': 0.8, 'sum': [283518, 410472, 486114], 'iterations': 2, 'runtime_data': [{'total_cycles': 372342000.0, 'percentage': 100, 'cycles_call': 372342000.0, 'data': 'all', 'calls': 1}, {'total_cycles': 16682400.0, 'percentage': 4.4804, 'cycles_call': 16682400.0, 'data': 'diffX', 'calls': 1}, {'total_cycles': 16223700.0, 'percentage': 4.3572, 'cycles_call': 16223700.0, 'data': 'diffY', 'calls': 1}, {'total_cycles': 14443900.0, 'percentage': 3.87921, 'cycles_call': 14443900.0, 'data': 'domainTransform', 'calls': 1}, {'total_cycles': 6674460.0, 'percentage': 1.79256, 'cycles_call': 6674460.0, 'data': 'transpose_float', 'calls': 1}, {'total_cycles': 55209200.0, 'percentage': 14.8276, 'cycles_call': 13802300.0, 'data': 'transpose_rgb', 'calls': 4}, {'total_cycles': 4293990.0, 'percentage': 1.15324, 'cycles_call': 2147000.0, 'data': 'cumsumX', 'calls': 2}, {'total_cycles': 128281000.0, 'percentage': 34.4525, 'cycles_call': 32070300.0, 'data': 'BoxFilterBounds', 'calls': 4}, {'total_cycles': 42299000.0, 'percentage': 11.3603, 'cycles_call': 10574700.0, 'data': 'computeRowSAT', 'calls': 4}, {'total_cycles': 59671100.0, 'percentage': 16.0259, 'cycles_call': 14917800.0, 'data': 'boxFilter', 'calls': 4}], 'benchmark_iterations': 1, 'input': './testimages/bhudda_720p.png', 'cycles': 372372000.0, 'method': 'nc'}, {'total_cycles': 285170000.0, 'benchmark_warmup': 0, 'description': 'NC (Normalized Convolution)', 'sigma_s': 20, 'sigma_r': 0.8, 'sum': [209560, 317067, 385548], 'iterations': 2, 'runtime_data': [{'total_cycles': 285142000.0, 'percentage': 100, 'cycles_call': 285142000.0, 'data': 'all', 'calls': 1}, {'total_cycles': 12637700.0, 'percentage': 4.43207, 'cycles_call': 12637700.0, 'data': 'diffX', 'calls': 1}, {'total_cycles': 12521400.0, 'percentage': 4.39127, 'cycles_call': 12521400.0, 'data': 'diffY', 'calls': 1}, {'total_cycles': 13464300.0, 'percentage': 4.72195, 'cycles_call': 13464300.0, 'data': 'domainTransform', 'calls': 1}, {'total_cycles': 5090610.0, 'percentage': 1.78529, 'cycles_call': 5090610.0, 'data': 'transpose_float', 'calls': 1}, {'total_cycles': 43234400.0, 'percentage': 15.1624, 'cycles_call': 10808600.0, 'data': 'transpose_rgb', 'calls': 4}, {'total_cycles': 3153400.0, 'percentage': 1.1059, 'cycles_call': 1576700.0, 'data': 'cumsumX', 'calls': 2}, {'total_cycles': 94707000.0, 'percentage': 33.2139, 'cycles_call': 23676700.0, 'data': 'BoxFilterBounds', 'calls': 4}, {'total_cycles': 33713100.0, 'percentage': 11.8232, 'cycles_call': 8428270.0, 'data': 'computeRowSAT', 'calls': 4}, {'total_cycles': 45589800.0, 'percentage': 15.9884, 'cycles_call': 11397400.0, 'data': 'boxFilter', 'calls': 4}], 'benchmark_iterations': 1, 'input': './testimages/bhudda_1024.png', 'cycles': 285170000.0, 'method': 'nc'}, {'total_cycles': 883744000.0, 'benchmark_warmup': 0, 'description': 'NC (Normalized Convolution)', 'sigma_s': 20, 'sigma_r': 0.8, 'sum': [637619, 923548, 1093750.0], 'iterations': 2, 'runtime_data': [{'total_cycles': 883721000.0, 'percentage': 100, 'cycles_call': 883721000.0, 'data': 'all', 'calls': 1}, {'total_cycles': 36959000.0, 'percentage': 4.18221, 'cycles_call': 36959000.0, 'data': 'diffX', 'calls': 1}, {'total_cycles': 35161900.0, 'percentage': 3.97885, 'cycles_call': 35161900.0, 'data': 'diffY', 'calls': 1}, {'total_cycles': 33495700.0, 'percentage': 3.7903, 'cycles_call': 33495700.0, 'data': 'domainTransform', 'calls': 1}, {'total_cycles': 16955800.0, 'percentage': 1.91869, 'cycles_call': 16955800.0, 'data': 'transpose_float', 'calls': 1}, {'total_cycles': 128424000.0, 'percentage': 14.5322, 'cycles_call': 32106100.0, 'data': 'transpose_rgb', 'calls': 4}, {'total_cycles': 10179600.0, 'percentage': 1.1519, 'cycles_call': 5089780.0, 'data': 'cumsumX', 'calls': 2}, {'total_cycles': 298454000.0, 'percentage': 33.7725, 'cycles_call': 74613600.0, 'data': 'BoxFilterBounds', 'calls': 4}, {'total_cycles': 105770000.0, 'percentage': 11.9688, 'cycles_call': 26442600.0, 'data': 'computeRowSAT', 'calls': 4}, {'total_cycles': 146403000.0, 'percentage': 16.5667, 'cycles_call': 36600900.0, 'data': 'boxFilter', 'calls': 4}], 'benchmark_iterations': 1, 'input': './testimages/bhudda_1080p.png', 'cycles': 883744000.0, 'method': 'nc'}, {'total_cycles': 1161160000.0, 'benchmark_warmup': 0, 'description': 'NC (Normalized Convolution)', 'sigma_s': 20, 'sigma_r': 0.8, 'sum': [835630, 1265350.0, 1538990.0], 'iterations': 2, 'runtime_data': [{'total_cycles': 1161140000.0, 'percentage': 100, 'cycles_call': 1161140000.0, 'data': 'all', 'calls': 1}, {'total_cycles': 52725600.0, 'percentage': 4.54085, 'cycles_call': 52725600.0, 'data': 'diffX', 'calls': 1}, {'total_cycles': 49942500.0, 'percentage': 4.30116, 'cycles_call': 49942500.0, 'data': 'diffY', 'calls': 1}, {'total_cycles': 43811500.0, 'percentage': 3.77314, 'cycles_call': 43811500.0, 'data': 'domainTransform', 'calls': 1}, {'total_cycles': 23459200.0, 'percentage': 2.02035, 'cycles_call': 23459200.0, 'data': 'transpose_float', 'calls': 1}, {'total_cycles': 182660000.0, 'percentage': 15.731, 'cycles_call': 45664900.0, 'data': 'transpose_rgb', 'calls': 4}, {'total_cycles': 13284100.0, 'percentage': 1.14406, 'cycles_call': 6642050.0, 'data': 'cumsumX', 'calls': 2}, {'total_cycles': 374084000.0, 'percentage': 32.217, 'cycles_call': 93521100.0, 'data': 'BoxFilterBounds', 'calls': 4}, {'total_cycles': 133015000.0, 'percentage': 11.4556, 'cycles_call': 33253800.0, 'data': 'computeRowSAT', 'calls': 4}, {'total_cycles': 188409000.0, 'percentage': 16.2262, 'cycles_call': 47102300.0, 'data': 'boxFilter', 'calls': 4}], 'benchmark_iterations': 1, 'input': './testimages/bhudda_2048.png', 'cycles': 1161160000.0, 'method': 'nc'}, {'total_cycles': 3861430000.0, 'benchmark_warmup': 0, 'description': 'NC (Normalized Convolution)', 'sigma_s': 20, 'sigma_r': 0.8, 'sum': [2966210.0, 3891280.0, 4364080.0], 'iterations': 2, 'runtime_data': [{'total_cycles': 3861410000.0, 'percentage': 100, 'cycles_call': 3861410000.0, 'data': 'all', 'calls': 1}, {'total_cycles': 162333000.0, 'percentage': 4.20399, 'cycles_call': 162333000.0, 'data': 'diffX', 'calls': 1}, {'total_cycles': 160276000.0, 'percentage': 4.1507, 'cycles_call': 160276000.0, 'data': 'diffY', 'calls': 1}, {'total_cycles': 153743000.0, 'percentage': 3.98153, 'cycles_call': 153743000.0, 'data': 'domainTransform', 'calls': 1}, {'total_cycles': 72499200.0, 'percentage': 1.87753, 'cycles_call': 72499200.0, 'data': 'transpose_float', 'calls': 1}, {'total_cycles': 619277000.0, 'percentage': 16.0376, 'cycles_call': 154819000.0, 'data': 'transpose_rgb', 'calls': 4}, {'total_cycles': 43295400.0, 'percentage': 1.12123, 'cycles_call': 21647700.0, 'data': 'cumsumX', 'calls': 2}, {'total_cycles': 1272250000.0, 'percentage': 32.9478, 'cycles_call': 318063000.0, 'data': 'BoxFilterBounds', 'calls': 4}, {'total_cycles': 429431000.0, 'percentage': 11.1211, 'cycles_call': 107358000.0, 'data': 'computeRowSAT', 'calls': 4}, {'total_cycles': 624277000.0, 'percentage': 16.1671, 'cycles_call': 156069000.0, 'data': 'boxFilter', 'calls': 4}], 'benchmark_iterations': 1, 'input': './testimages/bhudda_4k.png', 'cycles': 3861430000.0, 'method': 'nc'}], 'revision': 'local'}, 'computer_name': 'giauzar.local'}
# plt.rc('text', usetex=True)
# plt.rc('font', family='serif')


IMAGE_REGEX = '.+\/([\w]+)'
COMPILED_REGEX = re.compile(IMAGE_REGEX)

# Will plot cycles per function
def plot_branch_with_functions(name, data, mode="cycles_pixel"):
    fig = plt.figure()

    results = data["results"]
    # Plot data
    plot_data = {}
    # Functions
    tested_functions = []
    # Loop over all results and get results
    image_size = []

    # Iterate over all images in resultset
    for resultset in results:
        method = resultset["description"]
        runtime_data = resultset["runtime_data"]
        input_file = COMPILED_REGEX.search(resultset["input"]).group(1)
        input_file = input_file.replace("_", " ")

        im_handle = Image.open(resultset["input"])
        im_size = im_handle.size[0]*im_handle.size[1]
        image_size.append((input_file, im_size))

        data_dict = {}
        # Iterate over all functions
        for r in runtime_data:
            fname = r["data"]
            perc  = r["percentage"]
            cycles = r["total_cycles"]
            # Skip all functions containing all
            if fname == "all":
                continue

            val = cycles
            if (mode == "cycles_pixel"):
                val = cycles/im_size
            if (mode == "percentage"):
                val = perc
            data_dict[fname] = val
            if not (fname in tested_functions):
                tested_functions.append(fname)
            #rtuple += [(fname, perc)]

        plot_data[input_file] = data_dict

    # Sort tested functions
    tested_functions = sorted(tested_functions)

    image_size = sorted(image_size, key=lambda l: l[1])
    sorted_img_list, sorted_img_size_list = zip(*image_size)
    # Now we have a dict with function names
    # and a dict with images names and corresponding cycles
    # we now map these values according to http://matplotlib.org/examples/pylab_examples/bar_stacked.html

    plt_bar_handles = []
    total_images = len(sorted_img_list)
    ind = range(0, total_images)
    bar_width = 0.3

    colors = matplotlib_cm.rainbow(np.linspace(0, 1, len(tested_functions)))

    # Loop over all tested functions
    bottom_values = [0]*total_images
    for fn, color in zip(tested_functions, colors):
        values = [0]*total_images
        counter = 0
        # Loop over all images and add the to the plot
        # for img, mapping in plot_data.iteritems():
        for img in sorted_img_list:

            # Check if image from is in the data values
            # there's probably a better function for this
            if not (img in plot_data.keys()):
                continue

            # Ok the image exists
            mapping = plot_data[img]
            # Check if function was executed with the image
            if fn in mapping.keys():
                values[counter] = mapping[fn]
            counter += 1

        print fn+str(values)
        pl = None

        pl = plt.bar(ind, values, width=bar_width, color=color, bottom=bottom_values)
        plt_bar_handles.append((pl, fn))
        bottom_values = map(lambda l: l[0]+l[1], zip(bottom_values, values))

    plt_handles, plt_names = zip(*plt_bar_handles)

    megapixels = map(lambda l: l/1e6, sorted_img_size_list)
    # Limit values to two decimal places
    megapixels = map(lambda l: "%.2f" % l, megapixels)
    plt.xticks(map(lambda l: l+bar_width/2.0, ind), megapixels)
    #plt.legend(reversed(plt_handles), reversed(plt_names), loc='center left', bbox_to_anchor=(1, 1))
    plt.legend(reversed(plt_handles), reversed(plt_names), loc='bottom right', bbox_to_anchor=(0.72,0.6)).draggable()
    plt.xlabel("Image Size (Megapixels)")

    plt.title("Cycles")

    if (mode == "cycles_pixel"):
        plt.title("Cycles/Pixel")
    if (mode == "percentage"):
        plt.title("Runtime (Percentage)")
    plt.grid(True)


    plt.show()

def plot_dataset(dataset, branches):

    for branch in branches:
        name = branch
        data = dataset[name]
    #for name, data in dataset.iteritems():
        print "Plotting branch: "+name
        plot_branch_with_functions(name, data)

## Sorry, it makes no sense to split the plotting into smaller trunks
def plot_with_branches(dataset, branches, mode="cycles_pixel"):
    plot_data = []
    tested_functions = []

    for branch in branches:
        # Get data for individual branch
        data = dataset[branch]
        # Results is storing the individual functions
        results = data["results"]
        #Loop over results
        for resultset in results:
            imgfile = COMPILED_REGEX.search(resultset["input"]).group(1)
            imgfile = imgfile.replace("_", " ")
            imgsize = resultset["height"]*resultset["width"]
            imgsize_scaled = imgsize/1e6
            function_dict = {}
            # Get runtime data
            runtime_data = resultset["runtime_data"]
            # Loop over all functions stored
            for f in runtime_data:
                fname = f["data"]
                perc  = f["percentage"]
                cycles = f["total_cycles"]
                # Skip all functions containing all
                if fname == "all":
                    continue

                val = cycles
                if (mode == "cycles_pixel"):
                    val = cycles/imgsize
                if (mode == "percentage"):
                    val = perc
                function_dict[fname] = val
                if not (fname in tested_functions):
                    tested_functions.append(fname)
            # Store evaluated data
            plot_data.append((("%.2fM\n" % imgsize_scaled)+imgfile+"\n"+branch, function_dict))

    # Sort tested function
    tested_functions = sorted(tested_functions)
    # Sort plot_data
    plot_data = sorted(plot_data, key=lambda l: l[0])

    # Now we have a list with function names
    # and a list with images names and a dict as tuple
    # we now map these values according to http://matplotlib.org/examples/pylab_examples/bar_stacked.html

    plt_bar_handles = []
    total_bars = len(plot_data)
    ind = range(0, total_bars)
    bar_width = 0.3

    # Assign colors
    colors = matplotlib_cm.rainbow(np.linspace(0, 1, len(tested_functions)))

    # Create bottom values index:
    bottom_values = [0]*total_bars

    # Now loop over the tested functions to create the plot
    for fn, color in zip(tested_functions, colors):
        # Create values
        values = [0]*total_bars

        counter = 0

        for t, function_dict in plot_data:

            # Check if fn is in function dict and add it to the plot
            # in the correct position
            if fn in function_dict.keys():
                values[counter] = function_dict[fn]

            # increase counter
            counter += 1

        pl = plt.bar(ind, values, width=bar_width, color=color, bottom=bottom_values)
        plt_bar_handles.append((pl, fn))

        # Add values to bottom values
        bottom_values = map(lambda l: l[0]+l[1], zip(bottom_values, values))

    # Okay everything is plotted assign legends
    plt_handles, plt_names = zip(*plt_bar_handles)

    bar_titles, function_dicts = zip(*plot_data)

    plt.xticks(map(lambda l: l+bar_width/2.0, ind), bar_titles)
    #plt.legend(reversed(plt_handles), reversed(plt_names), loc='center left', bbox_to_anchor=(1, 1))
    plt.legend(reversed(plt_handles), reversed(plt_names), loc='lower right')
    plt.xlabel("Image with branch")

    plt.title("Cycles")

    if (mode == "cycles_pixel"):
        plt.title("Cycles/Pixel")
    if (mode == "percentage"):
        plt.title("Runtime (Percentage)")
    plt.grid(True)

    plt.show()

def main():
    plot_with_branches(DATA, BRANCHES)

if __name__ == "__main__":
    main()
