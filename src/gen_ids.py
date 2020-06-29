filename = "genid"

f = open(filename, "w")

a = []
s = 0

for m in range(0, 3):
    for n in range(0, 3):
        a.append("//Cell {} {}\n".format(m,n))
        for i in range(0, 3):
            for j in range(0, 3):
                a.append('"subcell_{}_{}_{}_{}",//#{}\n'.format(m, n, i, j, s))
                s+=1

f.writelines(a)
