import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Read data
s2 = pd.read_csv('./data.csv', names=['arraylength','time'])

# Plot
sns.set_style("whitegrid")
# Plot the data on a single plot
sns.lineplot(x="arraylength", y="time", data=s2, label="simple2")
# set max value for y axis to 2
plt.ylim(0, 0.03)
# set y axis label to "time (s)"
plt.ylabel("time (s)")
# set x axis label to "array length"
plt.xlabel("array length")
#show the plot
plt.show()
plt.savefig('comparison.png')