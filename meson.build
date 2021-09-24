project('libssd1306',
        'c',
        default_options: [
            'c_std=c99',
            'warning_level=3',
            'default_library=static',
        ],
)

inc_dir = include_directories('include')

# provides:
#       * src_files
subdir('src')

library('ssd1306', src_files, include_directories: inc_dir)
