BEGIN {
    for (i = 0; i < MAX; i++) {
	prod[i] = 0
	cons[i] = 0
    }
}

{
    if ($1 == "Produced" && $2 < MAX) {
	prod[$2]++;
    }
    else if ($1 == "Consumed" && $2 < MAX) {
	cons[$2]++;
    }
    
}

END {
	count=0;
	for (i = 0; i < MAX; i++) {
		if (prod[i] == 1 && cons[i] == 1) {
			count++;
		}
		else {
#			printf("%d\t%d\t%d\n", i, prod[i], cons[i]);	
		}
	}
	if (count == MAX) {
		printf("OK. All test cases passed!\n");
	}
	else {
		printf("Please check your code. %d/%d test cases failed!\n", MAX - count, MAX);
	}
}
