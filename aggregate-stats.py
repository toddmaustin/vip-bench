
import sys
import re

# Aggregate stats for benchmark MODE=mode 
def aggregate( dir, iters, mode ):
    # Parse results file
    fp = open(dir+'/bin/'+dir+'-'+mode+'.results', 'r')
    line = fp.readline() # Skip header
    line = fp.readline() # Skip header
    line = fp.readline() # Skip newline
    line = fp.readline()
    runtime_ms = 0
    runtime_us = 0
    cycles = 0
    insns = 0
    while line:
        values = line.split('\t')
        runtime_ms += float(values[1].strip())
        runtime_us += float(values[2].strip())
        cycles += float(values[3].strip())
        insns += float(values[4].strip())
        line = fp.readline()
    fp.close()

    # Parse mem file
    fp = open(dir+'/bin/'+dir+'-'+mode+'.mem', 'r')
    line = fp.readline() # Skip header
    line = fp.readline() # Skip header
    line = fp.readline() # Skip newline
    line = fp.readline()
    this_pid = 0
    this_vsz = 0
    this_rss = 0
    this_cnt = 1
    vsz = 0
    rss = 0
    while line:
        line = re.sub('\s+',' ',line) # Clean up ps prints
        if "0 0" not in line:
            values = line.split(' ')
            if (int(values[0])!= this_pid):
                # Moving to next pid, record vsz and rss average for this pid
                vsz += (this_vsz/this_cnt)
                rss += (this_rss/this_cnt)
                # Move to next pid and save current line
                this_pid = int(values[0])
                this_vsz = float(values[1])
                this_rss = float(values[2])
                this_cnt = 1
            else:
                this_vsz += float(values[1])
                this_rss += float(values[2])
                this_cnt += 1
        line = fp.readline()
    vsz += (this_vsz/this_cnt)
    rss += (this_rss/this_cnt)
    fp.close()

    # Average results over iters
    runtime_ms = runtime_ms/iters
    runtime_us = runtime_us/iters
    cycles = cycles/iters
    insns = insns/iters
    vsz = vsz/iters
    rss = rss/iters
    # print results in table format
    print(dir + '-MODE=' + mode, end='')
    print(",\t{0},\t{1}".format(runtime_ms, runtime_us), end='')
    print(",\t{0},\t{1}".format(cycles, insns), end='')
    print(",\t{0},\t{1}".format(vsz, rss), end='\n')
    # print("\t{0}\t{1}\t{2}\t{3}".format(runtime_ms, runtime_us, cycles, insns))
    # print('\t'+ runtime_ms +'\t'+ runtime_us +'\t'+ cycles +'\t'+ insns, end='')
    # print('\t'+ vsz +'\t'+ rss +'\n', end='')




dir = sys.argv[1]
iters = float(sys.argv[2])
aggregate(dir, iters, "na")
aggregate(dir, iters, "do")
aggregate(dir, iters, "enc")
