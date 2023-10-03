import csv
import matplotlib.pyplot as plt
import matplotlib.patches as patches

filename = "data/test.csv"

xs_indices = []
sites_indices = []

def getPosition(index,gridLength):
	y = index // gridLength
	x = index - y*gridLength
	return [x,y]
	
def getPositions(indices):
	positions = []
	for row in indices:
		positions_temp = []
		for index in row:
			positions_temp.append(getPosition(int(index),100))
		positions.append(positions_temp)
	return positions

# This separates out the walker data and the sites data.
with open(filename) as csvfile:

	readCSV = csv.reader(csvfile, delimiter=",")
	for row in readCSV:
		xs_indices.append([row[0],row[1]])
		sites_indices.append(row[2:-1])

xs_positions = getPositions(xs_indices)
sites_positions = getPositions(sites_indices)

# tmax for the animation ?
tmax = len(xs_positions)

# now I want to find the limits I will need on my axes to have a nice zoom on the situation.
minX = 100
maxX = 0
minY = 100
maxY = 0
for xs in xs_positions:
	for x in xs:
		maxX = max(maxX,x[0])
		minX = min(minX,x[0])
		maxY = max(maxY,x[1])
		minY = min(minY,x[1])

for i in range(tmax):
	fig = plt.figure()
	ax = plt.axes(xlim = (minX-2,maxX + 2),ylim = (minY - 2,maxY+2))
	for site in sites_positions[i]:
		rect = plt.Rectangle((site[0]-0.5,site[1]-0.5),1,1,fc = 'gray',ec='gray')
		plt.gca().add_patch(rect)
	for x in xs_positions[i]:
		circle = plt.Circle((x[0],x[1]),0.5,fc='red',ec='red')
		plt.gca().add_patch(circle)
	plt.gca().set_aspect('equal')
	plt.savefig(f'data/testfigs/sats-{i}.png')
	plt.close()
