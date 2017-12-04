import pandas as pd 
import numpy as np 

data = pd.read_csv('treeTreads.csv', header=None)
data = data.as_matrix()

print(data[0][1])
print '\n'
for i in range(1,4):
	for j in range(0,8):
		data[i][j] = data[0][j]/data[i][j]

np.savetxt('TREE.dat', data, delimiter=' ') 
        
print(data)
