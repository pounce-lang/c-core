true 0 [1 +] 100000 times 100000 = && ["all tests pass"] ["failure"] if-else


# drop test
"2" [3 3] drop drop
true &&
# dip test
2 3 [1 +] dip
3 = [3 =] dip && &&
# crouch test
[line1] [[m b] [m * b +] crouch] compose
1 2 3 line1 play 5 = &&

["all tests pass"] ["failure"] if-else
