import pandas as pd
import matplotlib.pyplot as plt

# Read data from CSV files
data1 = pd.read_csv('output_Ssend.csv')
data2 = pd.read_csv('output_Bsend.csv')

# Plot data
plt.plot(data1['Array Size'], data1['Average Time'], label='Data 1')
plt.plot(data2['Array Size'], data2['Average Time'], label='Data 2')

# Add plot labels and legend
plt.xlabel('Array Size')
plt.ylabel('Average Time')
plt.title('Data Comparison')
plt.legend()

# Display plot
plt.show()
