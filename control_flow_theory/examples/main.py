n = int(input())
a = [int(input()) for i in range(n)]
for i in range(n - 1):
    for j in range(n - i - 1):
        if a[j] > a[j + 1]:
            x = a[j]
            a[j] = a[j + 1]
            a[j + 1] = x
for i in a:
    print(i, end=' ')