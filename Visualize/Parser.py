#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri May 30 14:59:20 2025

@author: hazel
"""

import pandas as pd
import PySimpleGUI as sg

#tuples representing locations of boxes on graph
offset = (27, 115)
takeoff = (70+offset[0],309+offset[1])
sticks = (355+offset[0],389+offset[1])
knobs = (510+offset[0],389+offset[1])
wp = (290+offset[0],279+offset[1])
nmpc = (435+offset[0],279+offset[1])
daa = (580+offset[0],279+offset[1])
hover = (745+offset[0],309+offset[1])
nohover = (855+offset[0],309+offset[1])
pilottakeover = (435+offset[0],59+offset[1])

heloStates = ['TakeOff', 'Sticks', 'Knobs', 'WP', 'NMPC', 'DAA', 'LandingNoHover', 'LandingHover', 'PilotTakeover']
heloCompLocs = {'TakeOff':takeoff, 'Sticks':sticks, 'Knobs':knobs, 'WP':wp, 'NMPC':nmpc, 'DAA':daa, 'LandingNoHover':nohover, 'LandingHover':hover, 'PilotTakeover':pilottakeover}





filepathBase = '../simulation_results/'

#filepathRemainder = input("Enter the csv file name: ")

filepath = filepathBase + 'results.csv'

simulation_results = pd.read_csv(filepath, header=1)

heloRows = simulation_results.loc[(simulation_results['model_name'] == 'Helo') &
                                  (simulation_results['port_name'].isnull())]
heloRows = heloRows.reset_index()

dataAndTime = heloRows[['time', 'data']]
states = dataAndTime['data'].str.extractall('(?P<states>[<].+[>])').reset_index(drop = True)

states = states['states'].str.replace('<', '').str.replace('>', '').str.split(";", expand = True)
states.columns = heloStates
statesAndTime = pd.concat([dataAndTime['time'], states], axis=1)
timeText = 'TIME = '
time = 0
layout = [[sg.Graph((1000,700),(0,0),(1000,700),key = 'Graph')],
          [sg.Slider(range = (0,len(statesAndTime)-1), orientation='horizontal', enable_events = True, key = 'Slider')],
          [sg.Text(timeText+str(time), font = ("Calibri", 45), key='TimeDisplay')]]
simViewer = sg.Window("Simple Helicopter Sim", layout, finalize=True)
simViewer['Graph'].draw_image(filename = 'SimpleHelo.png', location = (25,675))
simViewer['Graph'].draw_image(filename = 'on.png', location = takeoff)
simViewer['Graph'].draw_image(filename = 'off.png', location = sticks)
simViewer['Graph'].draw_image(filename = 'off.png', location = knobs)
simViewer['Graph'].draw_image(filename = 'off.png', location = wp)
simViewer['Graph'].draw_image(filename = 'off.png', location = nmpc)
simViewer['Graph'].draw_image(filename = 'off.png', location = daa)
simViewer['Graph'].draw_image(filename = 'off.png', location = hover)
simViewer['Graph'].draw_image(filename = 'off.png', location = nohover)
simViewer['Graph'].draw_image(filename = 'off.png', location = pilottakeover)

while True:
    event, values = simViewer.read()
    
    if (event == 'Exit') or (event == sg.WINDOW_CLOSED):
        break
    elif (event == 'Slider'):
        curr = statesAndTime.iloc[int(values['Slider'])]
        time = int(curr['time'])
        simViewer["TimeDisplay"].update(timeText+str(time))
        for comp in heloStates:
            loc = (0,0)
            loc = heloCompLocs.get(comp)
            simViewer.Finalize()
            if int(curr[comp]) == 1:
                simViewer['Graph'].draw_image(filename = 'on.png', location = loc)
            elif int(curr[comp]) == 0:
                simViewer['Graph'].draw_image(filename = 'off.png', location = loc)
            elif int(curr[comp]) == -1:
                simViewer['Graph'].draw_image(filename = 'broken.png', location = loc)
            else:
                assert False, "At time: " + str(time) + " the comp " + comp + " is in an invalid state"

simViewer.close()