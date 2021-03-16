#!/usr/bin/env python

import sys
import numpy as np
from root_numpy import root2array

rootfiles = sys.argv[1:]
npzfile = '.npz'
for rootfile in rootfiles:
    npzfile = rootfile+'.'+npzfile
print ('Processing ROOT file', rootfile)
print ('Output npz file', npzfile)


#print data[0]

ed = []
tes = []
tea = []
pp = []
pm = []
pdg = []

for rootfile in rootfiles:
    data = root2array (rootfile, 'ecalNT')
    matrixSize = int(np.sqrt (len(data[0][0])))
    for row in data:
        ed.append (np.array(row[0]).reshape ((matrixSize, matrixSize,-1)))
        tes.append (row[1])
        tea.append (row[2])
        pp.append (np.array(row[3]))
        pm.append (np.array(row[4]))
        pdg.append (row[5])

EnergyDeposit = np.array (ed)
TotalEnergySci = np.array (tes)
TotalEnergyAbsorb = np.array (tea)
ParticlePoint = np.array (pp)
ParticleMomentum = np.array (pm)
ParticlePDG = np.array (pdg)

print (EnergyDeposit.shape)
print (TotalEnergySci[0])
print (TotalEnergyAbsorb[0])
print (ParticlePoint[0])
print (ParticleMomentum[0])
print (ParticlePDG[0])



np.savez_compressed (npzfile, EnergyDeposit=EnergyDeposit, TotalEnergySci=TotalEnergySci, TotalEnergyAbsorb=TotalEnergyAbsorb, ParticlePoint=ParticlePoint, ParticleMomentum=ParticleMomentum, ParticlePDG=ParticlePDG)




