project('magneturrent', 'c',
          default_options : ['b_lto=false',
                             'b_asneeded=false'])

linkfiles   = files(['dk-tm4c123g.ld'])
startupfile = files(['startup_gcc.c'])

inc = include_directories('.')
subdir('driverlib')

link_args = []
foreach linkfile : linkfiles
 link_args += ['-Wl,-T,@0@/@1@'.format(meson.current_source_dir(), linkfile)]
endforeach

main = executable(
            'main.elf',
            [
                'src/main.c',
                'src/i2c.c',
                'src/lsm303dlhc.c',
                'src/watchdog.c',
                'src/heartbeat.c',
                'src/uart.c',
                startupfile
            ],
            include_directories : [inc],
            link_args            : [link_args, '-Wl,--gc-sections'],
            link_with            : driverlib)

maindump = custom_target(
                        'flashable binary',
      output           : ['main.bin'],
      command          : ['arm-none-eabi-objcopy', '-S', '-Obinary', 'main.elf', '@OUTPUT@'],
      depends          : main)