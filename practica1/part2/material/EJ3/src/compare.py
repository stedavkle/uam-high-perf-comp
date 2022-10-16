#%%
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


data = [('SD', 640, 360, 0.004281, 0.003204), ('HD', 1280, 720, 0.014192, 0.011680), ('FHD', 1920, 1080, 0.031264, 0.024813), ('4K', 3840, 2160, 0.151777, 0.094582), ('8K', 7680, 4320, 0.837083, 0.457242)]
# Create the pandas DataFrame
df = pd.DataFrame(data, columns = ['Resolution', 'Width', 'Height', 'Time_woI', 'Time_wI'])
# calculate the speedup for each resolution and add it to the dataframe as a new column
df['Speedup'] = df['Time_woI'] / df['Time_wI']
# calculate the FPS for each resolution and add it to the dataframe as a new column
df['FPS_wI'] = round(1 / df['Time_wI'], 1)
df['FPS_woI'] = round(1 / df['Time_woI'], 1)
#%%
# plot a table with the data from the dataframe and save it to a png file
fig, ax = plt.subplots(figsize=(5, 2)) # set size frame
ax.axis('off')
ax.axis('tight')
ax.table(cellText=df[['Resolution','Time_woI', 'FPS_woI', 'Time_wI', 'FPS_wI']].values, colLabels=df[['Resolution','Time_woI', 'FPS_woI', 'Time_wI', 'FPS_wI']].columns, loc='center')
fig.tight_layout()
plt.savefig('table.jpg')

# %%
# plot a bar chart with the speedup for each resolution and save it to a png file
fig, ax = plt.subplots(figsize=(8, 6))
ax.bar(df['Resolution'], df['Speedup'])
ax.set_xlabel('Resolution')
ax.set_ylabel('Speedup')
ax.set_title('Speedup for each resolution')
plt.savefig('speedup.jpg')

# %%
