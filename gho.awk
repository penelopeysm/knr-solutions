BEGIN {
    printf(">> ")
}

{
    split($0, a, FS)
    asort(a)
    for (i = 1; i <= NF; i++) {
        printf("%s ", a[i])
    }
    printf("\n")
    printf(">> ")
}

END {
    printf("\n")
}
