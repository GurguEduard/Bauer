import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("bauer_run.csv")

df["acc"] = df["ac"] + df["av"]
df["profit_rate"] = 100 * df["s"] / (df["c"] + df["v"])
df["organic"] = df["c"] / df["v"]

fig, ax = plt.subplots(2, 2, figsize=(12, 8), sharex=True)
fig.suptitle("Bauer reproduction schema")

a = ax[0, 0]
a.plot(df["year"], df["s"], label="surplus s")
a.plot(df["year"], df["acc"], label="accumulation ac+av")
a.plot(df["year"], df["k"], label="consumption k")
a.axhline(0, color="black", linewidth=0.5)
a.set_title("Economy totals")
a.grid(True)
a.legend()

a = ax[0, 1]
a.plot(df["year"], df["c1"], label="c1")
a.plot(df["year"], df["c2"], label="c2")
a.plot(df["year"], df["v1"], label="v1")
a.plot(df["year"], df["v2"], label="v2")
a.set_title("Capital stocks by department")
a.grid(True)
a.legend()

a = ax[1, 0]
a.plot(df["year"], df["k1"], label="k1")
a.plot(df["year"], df["k2"], label="k2")
a.axhline(0, color="black", linewidth=0.5)
a.set_title("Capitalist consumption by department")
a.set_xlabel("year")
a.grid(True)
a.legend()

a = ax[1, 1]
a.plot(df["year"], df["profit_rate"], color="tab:red")
a.set_ylabel("profit rate s/(c+v)  [%]", color="tab:red")
a.tick_params(axis="y", labelcolor="tab:red")
b = a.twinx()
b.plot(df["year"], df["organic"], color="tab:blue")
b.set_ylabel("organic composition c/v", color="tab:blue")
b.tick_params(axis="y", labelcolor="tab:blue")
a.set_title("Profit rate and organic composition")
a.set_xlabel("year")
a.grid(True)

fig.tight_layout()
plt.show()
