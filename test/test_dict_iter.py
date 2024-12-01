d = {1 : "a", 2 : "b"}

for k in d:
    print(k, d[k])

print(d.keys())
for k in d.keys():
    print(k, d[k])

for v in d.values():
    print(v)

print(d.items())
for k, v in d.items():
    print(k, v)

#print(d.iterkeys())
#for k in d.iterkeys():
#    print(k, d[k])

#print(d.itervalues())
#for v in d.itervalues():
#    print(v)

#print(d.iteritems())
#for k, v in d.iteritems():
#    print(k, v)

for k in d:
    print(k, d[k])
print(1 in d.keys())
#print(1 in d.iterkeys())