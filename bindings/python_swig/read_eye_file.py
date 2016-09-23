
from pyeye import *
import argparse
from os import strerror 

parser= argparse.ArgumentParser()
parser.add_argument('file', nargs=1, help='The input to the converter')
parser.add_argument('-b', action="store_true", help="Write the input as binary. the -o argument must be specified.")
parser.add_argument('--output-file','-o', type=str, help="The output filename, will be overwritten if exists.")

args = parser.parse_args()

log = EyeLog()
ret = log.read(args.file[0]) 
if ret != 0:
    exit ('Unable to open {}: {}'.format(args.file[0], strerror(ret)))

print "size of enties = ", len(log.getEntries())

if args.b:
    if not args.output_file:
        exit('When writing binary specify an output name (-o --output-file).')
    else:
        ret = log.open(args.output_file)
        if ret != 0:
            exit ('Unable to open {}: {}'.format(args.file[0], strerror(ret)))
        ret = log.write()
        if ret != 0:
            exit ('Unable to write log: {}'.format(strerror(ret)));
else :
    if args.output_file:
        f = open(args.output_file, 'wb')
        for i in log.getEntries():
            f.write(i.toString() + '\n')
    else:
        for i in log.getEntries():
            print i.toString() + '\n',
        
        

        
