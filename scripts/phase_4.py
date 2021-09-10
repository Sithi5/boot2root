def func4(input):
    if input < 2:
        second_number = 1
    else:
        first_number = func4(input - 1)
        second_number = func4(input - 2)
        second_number = second_number + first_number
    return second_number


print("Try getting 55 from func4 :")

i = 10
while i > 0:
    ret = func4(i)
    print("i = ", i)
    print("func4 result : ", ret)
    if ret == 55:
        print("The good input is : ", i)
        break
    i -= 1
