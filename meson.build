# This file is part of g810-led.

# g810-led is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3 of the License.

# g810-led is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with g810-led.  If not, see <https://www.gnu.org/licenses/>.

project(
  'g810-led',
  'cpp',
  version : '0.4.1',
  meson_version : '>= 0.52.0',
  default_options : ['cpp_std=gnu++11'],
)

extra_cpp_args = []
if get_option('usb') == 'hidapi'
  dep_usb = dependency('hidapi-hidraw')
  extra_cpp_args = ['-Dhidapi']
else
  dep_usb = dependency('libusb-1.0')
endif

add_project_arguments(
  [
    '-DVERSION="@0@"'.format(meson.project_version()),
  ],
  language : 'cpp'
)

libg810_led = library(
  meson.project_name(),
  [
    'src/classes/Keyboard.cpp',
  ],
  cpp_args : extra_cpp_args,
  version : meson.project_version(),
  dependencies : [dep_usb],
  install : true,
)

pkg = import('pkgconfig')
pkg.generate(
  libg810_led,
  extra_cflags : extra_cpp_args,
  description : 'library to control various logitech keyboard backlights',
)

g810_ld = executable(
  meson.project_name(),
  [
    'src/main.cpp',
    'src/helpers/help.cpp',
    'src/helpers/utils.cpp',
  ],
  link_with : libg810_led,
  install : true,
)

installed_name = get_option('prefix') / get_option('bindir') / meson.project_name()

foreach name : ['g213-led', 'g410-led', 'g413-led', 'g512-led', 'g513-led',
                'g610-led', 'g815-led', 'g910-led', 'gpro-led']
  meson.add_install_script('install_link.sh', installed_name,
                           get_option('bindir') / name)
endforeach

install_headers(
  'src/classes/Keyboard.h',
  subdir : meson.project_name()
)

install_data(
  'systemd/@0@-reboot.service'.format(meson.project_name()),
  install_dir : get_option('prefix') / get_option('libdir') / 'systemd' / 'system',
  install_mode : 'r--r--r--',
)

install_data(
  'udev/@0@.rules'.format(meson.project_name()),
  install_dir : get_option('sysconfdir') / 'udev' / 'rules.d',
  install_mode : 'r--r--r--',
)

install_data(
  'sample_profiles/all_blue',
  'sample_profiles/all_green',
  'sample_profiles/all_red',
  'sample_profiles/fx_breathing_red',
  'sample_profiles/fx_cwave',
  'sample_profiles/fx_hwave',
  'sample_profiles/group_keys',
  'sample_profiles/all_blue_fxl_breathing_red',
  'sample_profiles/all_off',
  'sample_profiles/colors',
  'sample_profiles/fx_color_green',
  'sample_profiles/fx_cycle',
  'sample_profiles/fx_vwave',
  'sample_profiles/keys_v_gradiant_fr_ch-latin1',
  install_dir : get_option('sysconfdir') / meson.project_name() / 'samples',
  install_mode : 'r--r--r--',
)
