package main

import (
	"fmt"
	"os"
	"strconv"
)

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
func calc(operator int, values []int) int {

	var sign string
	var result int

	switch operator {
	case 1:
		sign = "+"
	case 2:
		sign = "-"
	case 3:
		sign = "*"
	}

	result = values[0]
	fmt.Printf("%d %s ", values[0], sign)
	for i, n := range values[1:] {
		switch operator {
		case 1:
			result += n
		case 2:
			result -= n
		case 3:
			result *= n
		}
		if i+1 < len(values)-1 {
			fmt.Printf("%d %s ", n, sign)
		}
	}

	fmt.Printf("%d = %d\n", values[len(values)-1], result)

	return result
}

func argsToNumArray(vals []string) []int {
	var nums = []int{}

	for _, i := range vals {
		j, err := strconv.Atoi(i)
		if err != nil {
			fmt.Print("Invalid number used\n")
		}
		nums = append(nums, j)
	}

	return nums
}

func main() {

	if len(os.Args) < 3 {
		fmt.Print("Please use valid arguments ./calculator <operator> <numbers separated by space>\n")
		return
	}

	var operator int
	switch os.Args[1] {
	case "add":
		operator = 1
	case "sub":
		operator = 2
	case "mult":
		operator = 3
	default:
		fmt.Print("Please use a valid operator\n")
		return
	}

	calc(operator, argsToNumArray(os.Args[2:]))
}
