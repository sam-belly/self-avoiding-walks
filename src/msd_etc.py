import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

inputfile = 'data/msd_beta010_4096lattice1024.csv'

data = pd.read_csv(inputfile,names=['Time','MSD','Sites','Connected'])
total_sites = 1024*1024

data['Fraction'] = data['Sites']/total_sites

col = np.where(data['Connected'] == 0, 'g','r')
times_fitfraction = np.linspace(200,5000,num=100)
fit_diffusion = np.linspace(10000,175000,num=100)

# Now to write a figure, looking at the MSD. I hate doing this shit.

fig, ax1 = plt.subplots()
	
ax1.set_xlabel('Time')
ax1.set_ylabel('Mean squared displacement',color='r')
ax1.scatter(data['Time'],data['MSD'],s=1,color=col)
ax1.plot(fit_diffusion,fit_diffusion,color='k',alpha=0.5)
ax1.set_xscale('log')
ax1.set_yscale('log')

ax2 = ax1.twinx()

color = 'tab:blue'
ax2.set_ylabel('Fraction of sites visited',color=color)
ax2.scatter(data['Time'],data['Fraction'],s=1,color=color)
ax2.plot(times_fitfraction,0.003*pow(times_fitfraction,(2/3)),color='k',alpha = 0.5)
ax2.set_yscale('log')

ax1.set_title('4096 walkers arranged on a 1024*1024 lattice, beta = 0.10')
fig.tight_layout()

	
#plt.scatter(data['Time'],data['MSD'],s=1)
#plt.scatter(data['Time'],data['Time'],s=1)
#plt.xscale('log')
#plt.yscale('log')
#plt.xlabel('Time')
#plt.ylabel('Mean squared displacement')

plt.show()
