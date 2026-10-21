# Bauer Reproduction Schema Simulator

A simulation of Otto Bauer's two-department model of expanded reproduction (1913), written in C++.

I wanted to see if I could take an economic model that's usually worked through by hand in tables and turn it into a program that runs it year by year, and shows where it stops working.

## The model

The economy is split into two departments:

- **Department I** produces means of production (machines, raw materials).
- **Department II** produces consumer goods.

Each year, for each department, the program tracks:

| Symbol | Meaning |
|---|---|
| `c` | constant capital (means of production used up) |
| `v` | variable capital (wages) |
| `s` | surplus value |
| `k` | capitalists' consumption |
| `a_c` | surplus reinvested as new constant capital |
| `a_v` | surplus reinvested as new variable capital |
| product | annual product, `c + v + s` |

Default assumptions, following Bauer:

- constant capital grows 10% per year
- variable capital grows 5% per year
- rate of surplus value is 100% (`s = v`)
- starting values (thousands): Dept I `c = 120, v = 50`, Dept II `c = 80, v = 50`

Each year the surplus is split between reinvestment (`a_c + a_v`) and capitalists' consumption (`k`), the new capital is split between the two departments (Department I gets the larger share, since it has to supply the extra means of production), and the next year starts from the new totals. Because `c` grows faster than `v`, the share of surplus left for consumption keeps shrinking. The run stops automatically when `k` turns negative, which is when the model can no longer be reproduced. With the default values that happens in year 35.

## How the C++ version is built

- All values sit in one `double table[3][7]`: rows are Dept I, Dept II and totals; columns are `c, v, s, k, a_c, a_v, product`. Named references (`constant1`, `surplus2`, `total_*` …) point into the table so the formulas stay readable.
- `input_table()` reads starting values from the keyboard, or takes them as arguments for testing.
- `compute_table_values()` fills in surplus, accumulation, consumption and annual product.
- `turn()` applies the year-to-year update.
- `output_table()` prints the table to 3 decimal places, plus the rate of profit for that year.

## Build and run

Open `Bauer.sln` in Visual Studio 2022 and press **Ctrl+F5**. It builds with MSVC and has no external libraries.

## Input example

Henryk Grossman (1929) extended Bauer's table to 35 years using Bauer's own figures: 200,000 constant capital and 100,000 variable capital for the whole economy, constant capital growing 10% and variable capital 5% a year, and a 100% rate of surplus value. Split by department as in Bauer's table and entered in thousands, these are the program's default values, so you can load them by typing `1` at the menu.

To type them in yourself, choose `2` and answer the prompts in this order. Growth rates and the surplus rate are entered as decimals (`0.10` means 10%):

```
Input '1' for default values,'2' for custom values or '0' to exit: 2
Input value of constant capital for sector 1: 120
Input value of variable capital for sector 1: 50
Input value of constant capital for sector 2: 80
Input value of variable capital for sector 2: 50
Input value of constant capital growth rate: 0.10
Input value of variable capital growth rate: 0.05
Input value of surplus rate: 1
```

Grossman's result with these inputs is that capitalists' consumption starts falling in absolute terms after year 21 and disappears entirely in year 35. In this program, total `k` peaks in year 20, falls from year 21 onward, and turns negative in year 35, where the stop condition ends the run.

## Example output

First two years and last two years of the run above (`AV` is the annual product):

```
Year 1    c         v         s         k         ac        av        AV
Sector 1  120.000   50.000    50.000    37.500    10.000    2.500     220.000
Sector 2  80.000    50.000    50.000    37.500    10.000    2.500     180.000
Total     200.000   100.000   100.000   75.000    20.000    5.000     400.000
Rate of Profit: 33.333%

Year 2    c         v         s         k         ac        av        AV
Sector 1  134.667   53.667    53.667    39.739    11.244    2.683     242.000
Sector 2  85.333    51.333    51.333    38.011    10.756    2.567     188.000
Total     220.000   105.000   105.000   77.750    22.000    5.250     430.000
Rate of Profit: 32.308%

...

Year 34   c         v         s         k         ac        av        AV
Sector 1  4279.516  415.009   415.009   8.958     385.300   20.750    5109.534
Sector 2  365.515   85.310    85.310    1.841     79.203    4.265     536.135
Total     4645.031  500.319   500.319   10.800    464.503   25.016    5645.669
Rate of Profit: 9.724%

Year 35   c         v         s         k         ac        av        AV
Sector 1  4740.786  439.851   439.851   -9.951    427.810   21.993    5620.487
Sector 2  368.748   85.484    85.484    -1.934    83.144    4.274     539.716
Total     5109.534  525.335   525.335   -11.885   510.953   26.267    6160.204
Rate of Profit: 9.323%
```

## References

- Otto Bauer, "Die Akkumulation des Kapitals", *Die Neue Zeit*, 1913.
- Henryk Grossman, *Das Akkumulations- und Zusammenbruchsgesetz des kapitalistischen Systems* (The Law of Accumulation and Breakdown of the Capitalist System), 1929.
