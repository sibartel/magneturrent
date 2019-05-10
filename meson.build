project('magneturrent', 'c',
          default_options : ['b_lto=false',
                             'b_asneeded=false'])

# uController / HAL Driver dependend options`
linkfiles   = files(['dk-tm4c123g.ld'])
startupfile = files(['startup_gcc.c'])

link_args = []
foreach linkfile : linkfiles
 link_args += ['-Wl,-T,@0@/@1@'.format(meson.current_source_dir(), linkfile)]
endforeach

#link_deps +=  meson.get_compiler('c').find_library('/usr/lib/gcc/arm-none-eabi/8.3.0/@0@/libgcc'.format('armv7e-m'))

main = executable(
            'main.elf',
            ['src/main.c', 'driverlib/gpio.c', 'driverlib/sysctl.c', startupfile],
            #c_args              : [c_args ],
            link_args           : [link_args, '-Wl,--gc-sections'])#,
            #dependencies        : link_deps,
            #include_directories : ['driverlib'] )

# arm-none-eabi-objcopy -S -O binary Pro.elf Pro.bin
maindump = custom_target(
                        'dump',
      output           : 'main.bin',
      command          : ['arm-none-eabi-objcopy', '-S', '-Obinary', 'main.elf', '@OUTPUT@'],
      depends          : main)