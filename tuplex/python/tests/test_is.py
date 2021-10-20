import unittest
from tuplex import *

c = Context()
li = [True, False]
result = c.parallelize(li).map(lambda x: x is True).collect()
print(result)
assert result == li

