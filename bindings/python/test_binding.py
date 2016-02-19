#!/usr/bin/env python

from pyeye import *

log = EyeLog()
log.addEntry(MessageEntry(0, r"Hallo Wereld!"))
entry = GazeEntry(LGAZE, 1, 2, 3, 4)
entry2 = GazeEntry(LGAZE, 1, 2, 3, 4)
entry3 = GazeEntry(LGAZE, 1, 2, 3, 4)
log.addEntry(entry)
log.addEntry(entry2)
log.addEntry(entry3)
for i in log.getEntries():
    print i.toString()
