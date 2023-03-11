import pandas as pd
import matplotlib.pyplot as plt

# read data from CSV file into a dataframe
df = pd.read_csv('output_Bsend.csv')

# plot data
plt.plot(df['Array Size'], df['Average Time'])

# set axis labels and title
plt.xlabel('Array Size')
plt.ylabel('x')
plt.title('Line Plot')

# display the plot
plt.show()
