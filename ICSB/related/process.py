import sys
def percent(inlist):

    inlist.sort()
    length = len(inlist)
    if(length < 10):
       return 
    per_50 = int(length*0.5)
    per_75 = int(length*0.75)
    per_95 = int(length*0.95)
    per_99 = int(length*0.99)
    
    count = 0
    for i in inlist:
        count+=1
	if count == per_50:
	    print "%50:",i
	elif count == per_75:
	    print "%75:",i
	elif count == per_95:
	    print "%95:",i
	elif count == per_99:
	    print "%99:",i
    print "average:",round(reduce(lambda x,y:x+y,inlist)/float(len(inlist)),3)
    print "min:",inlist[0],"max:",inlist[length-1]

if __name__ == "__main__":
    name = sys.argv[1]
    x = open(name,'r')
    inlist=list()
    for item in x:
        item = item.strip()
        item = int(item)
        inlist.append(item)
    percent(inlist)
    x.close()
