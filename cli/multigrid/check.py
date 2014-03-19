import sys

print "########## INPUT CHECK ############"

f = open(sys.argv[1])

################################
# CHECK THE INPUT FOR THE GRID #
################################
for j in range(2):
	line = f.readline()

	# MAKE SURE THE LINE ENDS WITH A NEW LINE
	if (line[-1:] != '\n'):
		print "LINE "+str(j+1)+" :NO NEW LINE AT THE END!"
		print ""
		sys.exit(1)

	# SPLIT THE ELEMENTS INTO MANY STRINGS
	elements = line[:-1].split()

	# MAKE SURE THAT THERE ARE 3 ELEMENTS PER LINE
	if (len(elements) != 3):
		print "LINE "+str(j+1)+": THE GRID PROPERIES NEED 3 NUMBERS!"
		print ""
		sys.exit(1)
	
	# MAKE SURE THAT THAT EACH ELEMENT IS AN INT
	for i in range(len(elements)):
		try:
			int(elements[i])
		except:
			print "LINE "+str(j+1)+",ELEMENT "+str(i+1)+": ELEMENT "+elements[i]+" IS NOT AN INT!"
			print ""
			sys.exit(1)
	
	# THE SIZE OF THE GRID SHOUD BE A POSITIVE NUMBER
	try:
		test_sign = int(elements[2])
		if (abs(test_sign) != test_sign):
			print "LINE "+str(j+1)+",ELEMENT 3: THE SIZE OF THE GRID "+elements[i]+" SHOULD BE POSITIVE!" 
			print ""
			sys.exit(1)
	except:
		pass

##################################################
# CHECK THE BOUNDARY CONDITIONS OF THE RECTANGLE #
##################################################
line = f.readline()

# MAKE SURE THE LINE ENDS WITH A NEW LINE
if (line[-1:] != '\n'):
	print "LINE "+str(j+1)+" :NO NEW LINE AT THE END!"
	print ""
	sys.exit(1)

# SPLIT THE ELEMENTS INTO MANY STRINGS
elements = line[:-1].split()

# THE BOUNDARY CONDITIONS HAVE 4 ELEMENTS
if (len(elements) != 4):
	print "LINE "+str(j+1)+": THE GRID PROPERIES NEED 3 NUMBERS!"
	print ""
	sys.exit(1)

# MAKE SURE THAT THAT EACH ELEMENT IS AN INT
for i in range(len(elements)):
	try:
		int(elements[i])
	except:
		print "LINE 3,ELEMENT "+str(i+1)+": ELEMENT "+elements[i]+" IS NOT AN INT!"
		print ""
		sys.exit(1)


##########################################
# CHECK THE CIRCULAR BOUNDARY CONDITIONS #
##########################################
# SET THE LINE NUMBER COUNTER TO 0
j = 0
while (True):
	line = f.readline()
	
	# END THE PROGRAM IF THE END OF THE FILE IS REACHED
	if (line == '' or line.isspace()):
		break

	# MAKE SURE THE LINE ENDS WITH A NEW LINE
	if (line[-1:] != '\n'):
		print "LINE "+str(j+3)+" :NO NEW LINE AT THE END!"
		print ""
		sys.exit(1)

	# SPLIT THE ELEMENTS INTO MANY STRINGS
	elements = line[:-1].split()

	# MAKE SURE THAT THAT EACH ELEMENT IS AN INT
	if (len(elements) != 6):
		print "LINE "+str(j+3)+": THE GRID PROPERIES NEED 3 NUMBERS!"
		print ""
		sys.exit(1)
	for i in range(len(elements)):
		try:
			int(elements[i])
		except:
			print "LINE "+str(j+3)+",ELEMENT "+str(i+1)+": ELEMENT "+elements[i]+" IS NOT AN INT!"
			print ""
			sys.exit(1)

	# INCREMENT THE LINE NUMBER
	j += 1

print "NO ERRORS FOUND!"
print "###### INPUT CHECK FINISHED #######"
print ""
