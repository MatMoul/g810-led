#!/usr/bin/python2

import sys
import subprocess
import time


if len(sys.argv) > 1:
	if sys.argv[1] == '--help':
		print 'k2000 [speed (default:0.01] [colorOff] [colorOn] [colorFade1] [colorFade2]'
		sys.exit()


speed = 0.01
colorOff = '000000'
colorOn = 'ff0000'
colorFade1 = 'aa0000'
colorFade2 = '550000'

if len(sys.argv) > 1:
	try:
		speed = float(sys.argv[1])
	except:
		print 'Speed arg error'
if len(sys.argv) > 2:
	try:
		colorOff = sys.argv[2]
	except:
		print 'colorOff arg error'
if len(sys.argv) > 3:
	try:
		colorOn = sys.argv[3]
	except:
		print 'colorOn arg error'
if len(sys.argv) > 4:
	try:
		colorFade1 = sys.argv[4]
	except:
		print 'colorFade1 arg error'
if len(sys.argv) > 5:
	try:
		colorFade2 = sys.argv[5]
	except:
		print 'colorFade2 arg error'


def setKeys(keys):
	pipeValue = ''
	for index in range(len(keys)):
		if index == 0:
			pipeValue = pipeValue + 'k ' + keys[index] + ' ' + colorOn + '\\n'
		elif index == 1:
			pipeValue = pipeValue + 'k ' + keys[index] + ' ' + colorFade1 + '\\n'
		elif index == 2:
			pipeValue = pipeValue + 'k ' + keys[index] + ' ' + colorFade2 + '\\n'
		elif index == 3:
			pipeValue = pipeValue + 'k ' + keys[index] + ' ' + colorOff + '\\n'
	pipeValue = pipeValue + 'c'
	subprocess.call('echo -e "' + pipeValue + '" | g810-led -pp', shell=True)
	time.sleep(speed)


subprocess.call('g810-led -gn fkeys ' + colorOff, shell=True)

setKeys(['F1'])
setKeys(['F2'])
setKeys(['F3'])
setKeys(['F4'])
setKeys(['F5','F1'])
setKeys(['F6','F2','F1'])
setKeys(['F7','F3','F2','F1'])
setKeys(['F8','F4','F3','F2'])
setKeys(['F9','F5','F4','F3'])
setKeys(['F10','F6','F5','F4'])
setKeys(['F11','F7','F6','F5'])
setKeys(['F12','F8','F7','F6'])
setKeys(['F12','F9','F8','F7'])
setKeys(['F12','F10','F9','F8'])
setKeys(['F12','F11','F10','F9'])
setKeys(['F11','F10','F10','F10'])
setKeys(['F10'])
setKeys(['F9'])
setKeys(['F8','F12'])
setKeys(['F7','F11','F12'])
setKeys(['F6','F10','F11','F12'])
setKeys(['F5','F9','F10','F11'])
setKeys(['F4','F8','F9','F10'])
setKeys(['F3','F7','F8','F9'])
setKeys(['F2','F6','F7','F8'])
setKeys(['F1','F5','F6','F7'])
setKeys(['F1','F4','F5','F6'])
setKeys(['F1','F3','F4','F5'])
setKeys(['F1','F2','F3','F4'])
setKeys(['F1','F1','F2','F3'])
setKeys(['F1','F1','F1','F2'])
setKeys(['F1','F1','F1','F1'])
