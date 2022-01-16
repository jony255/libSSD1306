project(
    'libssd1306',
    'c',
    default_options: [
        'c_std=c99',
        'warning_level=3',
    ],
    version: '0.1.0',
)

inc_dir = include_directories('include')

# provides:
#       * src_files
subdir('src')

add_project_arguments(
    '-U_FILE_OFFSET_BITS',
    language: ['c'],
    native: false,
)

libssd1306 = library(
    'ssd1306',
     src_files,
     include_directories: inc_dir,
)

libssd1306_dep = declare_dependency(
    link_with: libssd1306,
    include_directories: inc_dir,
)

rotate = executable(
    'rotate',
    './extra/rotate.c',
    native: true,
)
