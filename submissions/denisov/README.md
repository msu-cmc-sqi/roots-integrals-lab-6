# Lab 6, variant 8

Variant:

- `f1(x) = exp(x) + 2`
- `f2(x) = -2x + 8`
- `f3(x) = -5 / x`
- type: `long double`
- root methods: Newton and chord, selected at build time with `-D`
- integration method: Simpson formula

Build Newton version:

```sh
make
```

Build chord version:

```sh
make chord
```

Run:

```sh
./lab6
./lab6 --roots
./lab6 --iterations
./lab6 --test
./lab6 --help
```

Reference values:

- `x13 = -2.390536703909`
- `x23 = -0.549509756796`
- `x12 = 1.251757931391`
- `area = 9.806944933573`

The report source is in `report/report.tex`.
Build the PDF report in an environment with LaTeX installed:

```sh
make report
```
