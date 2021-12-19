true
# times test
0 [1 +] 100000 times 
100000 = &&
# drop test
"2" [3 3] drop drop
true &&
# dip test
2 3 [1 +] dip
3 = [3 =] dip && &&

["all tests pass"] ["failure"] if-else
