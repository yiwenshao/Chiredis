def process_one_file(file_des,file_name):
  pass
  sum = 0
  higher_than_10 = 0
  higher_than_50 = 0
  negative = 0
  for i in file_des:      
      i=i.strip()
      if i[0] == 'w':
         break
      else:
         i=int(i)
         if i <0:
           negative+=1
         else:
           sum+=i
         if i > 10000:
           higher_than_10+=1
         if i > 500000:
           higher_than_50+=1
  print "file name: ",file_name,":"
  print 'average: ',float(sum)/200000
  print ">10ms: ",str(float(higher_than_10)/2000)+" %"
  print ">50ms: ",str(float(higher_than_50)/2000)+" %"
  print "negative ",negative
  print " "

if __name__ == "__main__":
  file_list = list()
  for i in range(1,17):
     file_name = "fileid="+str(i)
     file_list.append((open(file_name),file_name))
  print len(file_list)
  for item_file in file_list:
      process_one_file(item_file[0],item_file[1])

