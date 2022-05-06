project(
	'wlroots',
	'c',
	version: '0.16.0',
	license: 'MIT',
	meson_version: '>=0.58.1',
	default_options: [
		'c_std=c11',
		'warning_level=2',
		'werror=true',
	],
)
soversion = 11

little_endian = target_machine.endian() == 'little'
big_endian = target_machine.endian() == 'big'

add_project_arguments([
	'-DWLR_USE_UNSTABLE',
	'-DWLR_LITTLE_ENDIAN=@0@'.format(little_endian.to_int()),
	'-DWLR_BIG_ENDIAN=@0@'.format(big_endian.to_int()),
], language: 'c')

cc = meson.get_compiler('c')

add_project_arguments(cc.get_supported_arguments([
	'-Wundef',
	'-Wlogical-op',
	'-Wmissing-include-dirs',
	'-Wold-style-definition',
	'-Wpointer-arith',
	'-Winit-self',
	'-Wstrict-prototypes',
	'-Wimplicit-fallthrough=2',
	'-Wendif-labels',
	'-Wstrict-aliasing=2',
	'-Woverflow',
	'-Wmissing-prototypes',
	'-Walloca',

	'-Wno-missing-braces',
	'-Wno-missing-field-initializers',
	'-Wno-unused-parameter',
]), language: 'c')

# Compute the relative path used by compiler invocations.
source_root = meson.current_source_dir().split('/')
build_root = meson.global_build_root().split('/')
relative_dir_parts = []
i = 0
in_prefix = true
foreach p : build_root
	if i >= source_root.length() or not in_prefix or p != source_root[i]
		in_prefix = false
		relative_dir_parts += '..'
	endif
	i += 1
endforeach
i = 0
in_prefix = true
foreach p : source_root
        if i >= build_root.length() or not in_prefix or build_root[i] != p
                in_prefix = false
                relative_dir_parts += p
        endif
        i += 1
endforeach
relative_dir = join_paths(relative_dir_parts) + '/'

# Strip relative path prefixes from the code if possible, otherwise hide them.
if cc.has_argument('-fmacro-prefix-map=/prefix/to/hide=')
        add_project_arguments(
                '-fmacro-prefix-map=@0@='.format(relative_dir),
                language: 'c',
        )
else
        add_project_arguments(
                '-DWLR_REL_SRC_DIR="@0@"'.format(relative_dir),
                language: 'c',
        )
endif

features = {
        'drm-backend': false,
        'x11-backend': false,
        'libinput-backend': false,
        'xwayland': false,
        'gles2-renderer': false,
        'vulkan-renderer': false,
}
internal_features = {
        'xcb-errors': false,
}

wayland_project_options = ['tests=false', 'documentation=false']
wayland_server = dependency('wayland-server',
	version: '>=1.20',
	fallback: 'wayland',
	default_options: wayland_project_options,
)


drm = dependency('libdrm',
	version: '>=2.4.109',
	fallback: 'libdrm',
	default_options: [
		'libkms=false',
		'intel=false',
		'radeon=false',
		'amdgpu=false',
		'nouveau=false',
		'vmwgfx=false',
		'omap=false',
		'exynos=false',
		'freedreno=false',
		'tegra=false',
		'vc4=false',
		'etnaviv=false',
		'cairo-tests=false',
		'man-pages=false',
		'valgrind=false',
	],
)
gbm = dependency('gbm', version: '>=17.1.0')
xkbcommon = dependency('xkbcommon')
udev = dependency('libudev')
pixman = dependency('pixman-1')
math = cc.find_library('m')
rt = cc.find_library('rt')

wlr_files = []
wlr_deps = [
	wayland_server,
	drm,
	gbm,
	xkbcommon,
	udev,
	pixman,
	math,
	rt,
]

subdir('protocol')
proto_inc = include_directories('protocol')


threads = dependency('threads')
wayland_egl = dependency('wayland-egl')
wayland_cursor = dependency('wayland-cursor')
wayland_client = dependency('wayland-client')
libpng = dependency('libpng', required: false, disabler: true)
egl = dependency('egl', required: false, disabler: true)
glesv2 = dependency('glesv2', required: false, disabler: true)
# These versions correspond to ffmpeg 4.0
libavutil = dependency('libavutil', version: '>=56.14.100', required: false, disabler: true)
libavcodec = dependency('libavcodec', version: '>=58.18.100', required: false, disabler: true)
libavformat = dependency('libavformat', version: '>=58.12.100', required: false, disabler: true)
# Only needed for drm_fourcc.h
libdrm = dependency('libdrm').partial_dependency(compile_args: true, includes: true)

# epoll is a separate library in FreeBSD
if host_machine.system() == 'freebsd'
	libepoll = dependency('epoll-shim')
else
	libepoll = dependency('', required: false)
endif

if not cc.has_header('libavutil/hwcontext_drm.h', dependencies: libavutil)
	libavutil = disabler()
endif

compositors = {
}

clients = {
	
	'swayvkshot': {
		'src': 'swayvkshot.c',
		'dep': [libpng, rt, gbm, drm],
		'proto': [
			'wlr-screencopy-unstable-v1',
			'linux-dmabuf-unstable-v1',
		],
	},
}

foreach name, info : compositors
	extra_src = []
	foreach p : info.get('proto', [])
		extra_src += protocols_server_header[p]
	endforeach

	executable(
		name,
		[info.get('src'), extra_src],
		dependencies: [wlroots, libdrm],
		include_directories: [wlr_inc, proto_inc],
		build_by_default: get_option('examples'),
	)
endforeach

foreach name, info : clients
	extra_src = []
	foreach p : info.get('proto')
		extra_src += protocols_code[p]
		extra_src += protocols_client_header[p]
	endforeach

	executable(
		name,
		[info.get('src'), extra_src],
		dependencies: [wayland_client, info.get('dep', [])],
		build_by_default: get_option('examples'),
	)
endforeach
