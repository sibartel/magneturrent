# magneturrent
Magnetic current sensing based on a LSM303DLHC sensor and a TM4C123G board.

## Documentation
This project was realised as part of a course @ University of Bayreuth.
A german documentation is available [here](Ausarbeitung_Microcontroller_Praktikum.pdf).

## Supported Hardware
* TM4C123G board
* LSM303DLHC sensor, connected to i2c2

## Compiling
```bash
> meson build --cross-file dk-tm4c123g.cross
> cd build
> ninja
```

## License
magneturrent is licensed under the [MIT](LICENSE) license.