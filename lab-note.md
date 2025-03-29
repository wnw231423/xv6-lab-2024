# trace
1. sys_call num start with 1 not 0, so add one "" at the beginning of name array.
2. when printing return value of syscall, you may find "%d" doesn't work since it's uint64, make sure using "%ld" but not "%lu" since -1 is an valid return value.

I speed more than 1 hours to detect these problems, that's why I make a record here. Hopefully this can help a careless guy like me in the future.

# attack
