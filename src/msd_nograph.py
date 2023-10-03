import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

inputfile = 'data/msd_beta010_10together.csv'
singlefile = 'data/msd_beta010_100together.csv'
thirdfile = 'data/msd_beta010_1000together.csv'

data = pd.read_csv(inputfile,names=['Time','MSD','Sites'])
single = pd.read_csv(singlefile,names=['Time','MSD','Sites'])
many = pd.read_csv(thirdfile,names=['Time','MSD','Sites'])

total_sites = 1024*1024

data['Fraction'] = data['Sites']/total_sites

fig, ax1 = plt.subplots()
	
ax1.set_xlabel('Time')
ax1.set_ylabel('Mean squared displacement')
ax1.scatter(data['Time'],data['MSD'],s=1,color='r')
ax1.scatter(single['Time'],single['MSD'],s=1,color='b')
ax1.scatter(many['Time'],many['MSD'],s=1,color='g')

ax1.set_xscale('log')
ax1.set_yscale('log')



ax1.set_title('4096 walkers starting in the same place, beta = 0.10')
fig.tight_layout()


plt.show()
