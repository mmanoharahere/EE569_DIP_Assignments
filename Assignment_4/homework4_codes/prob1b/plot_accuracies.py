#Code submitted by Manasa Manohara 3722587777 04/29/2018
#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Fri Apr 26 15:41:43 2018

@author: mansi
"""

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from os import listdir

input_filepath = '/Users/mansi/Projects/CNN/results/'
output_filepath = '/Users/mansi/Projects/CNN/Plots/'

files_list = listdir(input_filepath)

for i in range(0,len(files_list)):
    print "---", files_list[i]
    if files_list[i][-4:] == '.csv':
        df = pd.read_csv(input_filepath+files_list[i])
        
        plt.figure()
        plt.plot(df['acc'], label='Training Accuracy')
        plt.plot(df['val_acc'], label='Testing Accuracy')
        plt.legend(loc='lower right')
        lasthypen = files_list[i].find('_',-9)
        add_title = files_list[i][lasthypen+1:-4]
        plt.title('Epoch-Accuracy Plots for Training and Testing - '+ add_title)
        plt.xlabel('Epochs/Iterations')
        plt.ylabel('Accuracy')
        plt.savefig(output_filepath+files_list[i][:-4]+'_plot')
        plt.close('all')
