import json
jstring = open("backoff_results").read()
d=json.loads(jstring)
MIN = 1000000000000000000000000
for i in d['mindelays']:
    if str(i) not in d:
        continue
    for j in d['maxdelays']:
        if str(j) not in d[str(i)]:
            continue
        x=d[str(i)][str(j)]
        if "mean" in x and x['mean'] < MIN:
                MIN = x['mean']
                print MIN, i, j
