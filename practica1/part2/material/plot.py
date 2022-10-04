import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Read data
s2 = pd.read_csv('./simple2.csv', names=['iterations','time','value'])
s2i = pd.read_csv('./simple2_intrinsics.csv', names=['iterations','time','value'])

# Plot
sns.set_style("whitegrid")
# Plot the data on a single plot
sns.lineplot(x="iterations", y="time", data=s2, label="simple2")
sns.lineplot(x="iterations", y="time", data=s2i, label="simple2_intrinsics", )
#show the plot
plt.show()
plt.savefig('comparison.png')