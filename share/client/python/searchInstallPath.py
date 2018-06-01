import sys
import re
for s in sys.path:
    if re.search(r'site-packages$', s) != None:
        print s
        break
