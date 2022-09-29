.text
.global sum

sum:
    add x0, x0, x1
    b sum
    ret