import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

inputfile = 'data/msd_beta075_256lattice.csv'

data = pd.read_csv(inputfile,names=['Time','MSD','Sites','Connected'])
total_sites = 256*256

data['Fraction'] = data['Sites']/total_sites

connected = data[data['Connected']==1]

print(connected.head())
