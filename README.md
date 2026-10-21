# Bauer Reproduction Schema Simulator

A simulation of Otto Bauer's two-department model of expanded reproduction (1913), written in C++, with an earlier version in Python.

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

Each year the surplus is split between reinvestment (`a_c + a_v`) and capitalists' consumption (`k`), part of Department II's surplus is transferred to Department I so both can keep growing, and the next year starts from the new totals. Because `c` grows faster than `v`, the share of surplus left for consumption keeps shrinking. The run stops automatically when `k` turns negative, which is when the model can no longer be reproduced. With the default values that happens in year 35.

## How the C++ version is built

- All values sit in one `double table[3][7]`: rows are Dept I, Dept II and totals; columns are `c, v, s, k, a_c, a_v, product`. Named references (`constant1`, `surplus2`, `total_*` …) point into the table so the formulas stay readable.
- `input_table()` reads starting values from the keyboard, or takes them as arguments for testing.
- `compute_table_values()` fills in surplus, accumulation, consumption and annual product.
- `turn()` applies the year-to-year update.
- `output_table()` prints the table to 3 decimal places.

## Build and run

**C++:** open `Bauer.sln` in Visual Studio 2022 and press **Ctrl+F5**. It builds with MSVC and has no external libraries.

**Python:** the earlier version is in `python/`.

```
pip install pandas
python python/main.py
```

## Example output

```
[paste the first 2–3 years and the final year of a run here]
```

## Reference

Otto Bauer, "Die Akkumulation des Kapitals", *Die Neue Zeit*, 1913.
